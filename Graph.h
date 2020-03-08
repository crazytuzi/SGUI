#pragma once

#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

#include "Point.h"
#include "FL/Fl.H"
#include "FL/Fl_Image.H"
#include "FL/fl_draw.H"
#include "std_lib_facilities.h"

namespace Graph_lib
{
	// defense against ill-behaved Linux macros:
#undef major
#undef minor

	/* 颜色类型 */
	struct Color
	{
		enum Color_type
		{
			red = FL_RED,
			blue = FL_BLUE,
			green = FL_GREEN,
			yellow = FL_YELLOW,
			white = FL_WHITE,
			black = FL_BLACK,
			magenta = FL_MAGENTA,
			cyan = FL_CYAN,
			dark_red = FL_DARK_RED,
			dark_green = FL_DARK_GREEN,
			dark_yellow = FL_DARK_YELLOW,
			dark_blue = FL_DARK_BLUE,
			dark_magenta = FL_DARK_MAGENTA,
			dark_cyan = FL_DARK_CYAN
		};

		/* 颜色的透明性 */
		enum Transparency { invisible = 0, visible = 255 };

		Color(Color_type cc) : v(visible), c(Fl_Color(cc))
		{
		}

		Color(Color_type cc, Transparency vv) : v(vv), c(Fl_Color(cc))
		{
		}

		Color(int cc) : v(visible), c(Fl_Color(cc))
		{
		}

		/* 默认颜色 */
		Color(Transparency vv) : v(vv), c(Fl_Color())
		{
		}

		int as_int() const { return c; }
		char visibility() const { return v; }
		void set_visibility(Transparency vv) { v = vv; }
	private:
		/* 当前不可见或者可见 */
		unsigned char v; // 0 or 1 for now
		Fl_Color c;
	};

	/* 描述线的外形的一种模式 */
	struct Line_style
	{
		enum Line_style_type
		{
			// -------
			solid = FL_SOLID,
			// - - - -
			dash = FL_DASH,
			// ....... 
			dot = FL_DOT,
			// - . - . 
			dashdot = FL_DASHDOT,
			// -..-..
			dashdotdot = FL_DASHDOTDOT,
		};

		Line_style(Line_style_type ss) : s(ss), w(0)
		{
		}

		Line_style(Line_style_type lst, int ww) : s(lst), w(ww)
		{
		}

		Line_style(int ss) : s(ss), w(0)
		{
		}

		int width() const { return w; }
		int style() const { return s; }
	private:
		/* 模式(如虚线或实线) */
		int s;
		/* 宽度(线的粗细) */
		int w;
	};

	/* 字符的字体 */
	class Font
	{
	public:
		enum Font_type
		{
			helvetica = FL_HELVETICA,
			helvetica_bold = FL_HELVETICA_BOLD,
			helvetica_italic = FL_HELVETICA_ITALIC,
			helvetica_bold_italic = FL_HELVETICA_BOLD_ITALIC,
			courier = FL_COURIER,
			courier_bold = FL_COURIER_BOLD,
			courier_italic = FL_COURIER_ITALIC,
			courier_bold_italic = FL_COURIER_BOLD_ITALIC,
			times = FL_TIMES,
			times_bold = FL_TIMES_BOLD,
			times_italic = FL_TIMES_ITALIC,
			times_bold_italic = FL_TIMES_BOLD_ITALIC,
			symbol = FL_SYMBOL,
			screen = FL_SCREEN,
			screen_bold = FL_SCREEN_BOLD,
			zapf_dingbats = FL_ZAPF_DINGBATS
		};

		Font(Font_type ff) : f(ff)
		{
		}

		Font(int ff) : f(ff)
		{
		}

		int as_int() const { return f; }
	private:
		int f;
	};

	template <class T>
	class Vector_ref
	{
		vector<T*> v;
		vector<T*> owned;
	public:
		Vector_ref()
		{
		}

		Vector_ref(T* a, T* b = nullptr, T* c = nullptr, T* d = nullptr)
		{
			if (a) push_back(a);
			if (b) push_back(b);
			if (c) push_back(c);
			if (d) push_back(d);
		}

		~Vector_ref() { for (int i = 0; i < owned.size(); ++i) delete owned[i]; }

		/* 加入一个已命名对象 */
		void push_back(T& s) { v.push_back(&s); }

		/* 加入一个未命名对象 */
		void push_back(T* p)
		{
			v.push_back(p);
			owned.push_back(p);
		}

		// ???void erase(???)

		/* 加下标:便于读写访问 */
		T& operator[](int i) { return *v[i]; }
		const T& operator[](int i) const { return *v[i]; }
		int size() const { return v.size(); }
	};

	typedef double Fct(double);

	class Shape
	{
		// deals with color and style, and holds sequence of lines
	protected:
		Shape()
		{
		}

		Shape(std::initializer_list<Point> lst); // add() the Points to this Shape

		//	Shape() : lcolor(fl_color()),
		//		ls(0),
		//		fcolor(Color::invisible) { }

		void add(Point p) { points.push_back(p); }
		void set_point(int i, Point p) { points[i] = p; }
	public:
		void draw() const; // deal with color and draw_lines
	protected:
		virtual void draw_lines() const; // simply draw the appropriate lines
	public:
		virtual void move(int dx, int dy); // move the shape +=dx and +=dy

		void set_color(Color col) { lcolor = col; }
		Color color() const { return lcolor; }

		void set_style(Line_style sty) { ls = sty; }
		Line_style style() const { return ls; }

		void set_fill_color(Color col) { fcolor = col; }
		Color fill_color() const { return fcolor; }

		Point point(int i) const { return points[i]; }
		int number_of_points() const { return int(points.size()); }

		virtual ~Shape()
		{
		}

		/*
		struct SWindow* attached;
		Shape(const Shape& a)
			:attached(a.attached), points(a.points), line_color(a.line_color), ls(a.ls)
		{
			if (a.attached)error("attempt to copy attached shape");
		}
		*/
		Shape(const Shape&) = delete;
		Shape& operator=(const Shape&) = delete;
	private:
		vector<Point> points; // not used by all shapes
		Color lcolor{fl_color()};
		Line_style ls{0};
		Color fcolor{Color::invisible};

		//	Shape(const Shape&);
		//	Shape& operator=(const Shape&);
	};

	struct Function : Shape
	{
		// the function parameters are not stored
		Function(Fct f, double r1, double r2, Point orig, int count = 100, double xscale = 25, double yscale = 25);
		//Function(Point orig, Fct f, double r1, double r2, int count, double xscale = 1, double yscale = 1);	
	};

	struct Fill
	{
		Fill() : no_fill(true), fcolor(0)
		{
		}

		Fill(Color c) : no_fill(false), fcolor(c)
		{
		}

		void set_fill_color(Color col) { fcolor = col; }
		Color fill_color() { return fcolor; }
	protected:
		bool no_fill;
		Color fcolor;
	};

	/* 一个Line是由两个Point定义的一种Shape */
	struct Line : Shape
	{
		/* 由两个Point创建一个Line */
		Line(Point p1, Point p2)
		{
			add(p1);
			add(p2);
		}
	};

	struct Rectangle : Shape
	{
		/* 一个顶点(左上角)和宽度、高度 */
		Rectangle(Point xy, int ww, int hh) : h{hh}, w{ww}
		{
			if (h <= 0 || w <= 0) error("Bad rectangle: non-positive side");
			add(xy);
		}

		/* 两个顶点(左上角和右下角) */
		Rectangle(Point x, Point y) : h{y.y - x.y}, w{y.x - x.x}
		{
			if (h <= 0 || w <= 0) error("Bad rectangle: first point is not top left");
			add(x);
		}

		void draw_lines() const override;

		//	void set_fill_color(Color col) { fcolor = col; }
		//	Color fill_color() { return fcolor; }

		int height() const { return h; }
		int width() const { return w; }
	private:
		/* 高度 */
		int h; // height
		/* 宽度 */
		int w; // width
		//	Color fcolor;	// fill color; 0 means "no fill"
	};

	bool intersect(Point p1, Point p2, Point p3, Point p4);

	/* 线的开放序列 */
	struct Open_polyline : Shape
	{
		// open sequence of lines
		/* 使用Shape的构造函数 */
		using Shape::Shape;
		void add(Point p) { Shape::add(p); }
		void draw_lines() const override;
	};

	/* 线的闭合序列 */
	struct Closed_polyline : Open_polyline
	{
		// closed sequence of lines
		/* 使用Open_polyline的构造函数 */
		using Open_polyline::Open_polyline;
		void draw_lines() const override;

		//	void add(Point p) { Shape::add(p); }
	};

	/* 非交叉线的闭合序列 */
	struct Polygon : Closed_polyline
	{
		// closed sequence of non-intersecting lines
		/* 使用Closed_polyline的构造函数 */
		using Closed_polyline::Closed_polyline;
		void add(Point p);
		void draw_lines() const override;
	};

	/* 相关的多条线 */
	struct Lines : Shape
	{
		// indepentdent lines
		/* 空对象 */
		Lines()
		{
		}

		/* 通过一个点的列表对象进行初始化 */
		Lines(std::initializer_list<Point> lst) : Shape{lst}
		{
			if (lst.size() % 2) error("odd number of points for Lines");
		}

		Lines(std::initializer_list<pair<Point, Point>> lst)
		{
			for (const auto p : lst)
			{
				add(p.first, p.second);
			}
		}

		/* 绘制 */
		void draw_lines() const override;

		/* 加入一条由两个点定义的线 */
		void add(Point p1, Point p2)
		{
			Shape::add(p1);
			Shape::add(p2);
		}
	};

	struct Text : Shape
	{
		// the point is the bottom left of the first letter
		/* 该点在第一个字符的左下角处 */
		Text(Point x, const string& s) : lab{s} { add(x); }

		void draw_lines() const override;

		void set_label(const string& s) { lab = s; }
		string label() const { return lab; }

		void set_font(Font f) { fnt = f; }
		Font font() const { return Font(fnt); }

		void set_font_size(int s) { fnt_sz = s; }
		int font_size() const { return fnt_sz; }
	private:
		/* 标签 */
		string lab; // label
		Font fnt{fl_font()};
		int fnt_sz{(14 < fl_size()) ? fl_size() : 14}; // at least 14 point
	};


	struct Axis : Shape
	{
		// representation left public
		enum Orientation { x, y, z };

		Axis(Orientation d, Point xy, int length, int nummber_of_notches = 0, string label = "");

		void draw_lines() const override;
		void move(int dx, int dy) override;

		void set_color(Color c);

		Text label;
		Lines notches;
		//	Orientation orin;
		//	int notches;
	};

	struct Circle : Shape
	{
		/* 圆心和半径 */
		Circle(Point p, int rr) // center and radius
			: r{rr}
		{
			/* 保存圆的正方形边界的左上角的点 */
			add(Point{p.x - r, p.y - r});
		}

		void draw_lines() const override;

		Point center() const { return {point(0).x + r, point(0).y + r}; }

		void set_radius(int rr)
		{
			/* 保持圆心 */
			set_point(0, Point{center().x - rr, center().y - rr});
			r = rr;
		}

		int radius() const { return r; }
	private:
		int r;
	};


	struct Ellipse : Shape
	{
		/* 圆心,到圆心的最大和最小距离 */
		Ellipse(Point p, int ww, int hh) // center, min, and max distance from center
			: w{ww}, h{hh}
		{
			add(Point{p.x - ww, p.y - hh});
		}

		void draw_lines() const override;

		Point center() const { return {point(0).x + w, point(0).y + h}; }
		Point focus1() const { return {center().x + int(sqrt(w * w - h * h)), center().y}; }
		Point focus2() const { return {center().x - int(sqrt(w * w - h * h)), center().y}; }

		void set_major(int ww)
		{
			/* 保持圆心 */
			set_point(0, Point{center().x - ww, center().y - h});
			w = ww;
		}

		int major() const { return w; }

		void set_minor(int hh)
		{
			/* 保持圆心 */
			set_point(0, Point{center().x - w, center().y - hh});
			h = hh;
		}

		/* 焦点 */
		Point focus() const
		{
			if (h <= w) /* 焦点在x轴上 */
			{
				return {center().x + int(sqrt(double(w * w - h * h))), center().y};
			}
			/* 焦点在y轴上 */
			return {center().x, center().y + int(sqrt(double(h * h - w * w)))};
		}

		int minor() const { return h; }
	private:
		int w;
		int h;
	};

	/*
	struct Mark : Text {
		static const int dw = 4;
		static const int dh = 4;
		Mark(Point xy, char c) : Text(Point(xy.x-dw, xy.y+dh),string(1,c)) {}
	};
	*/

	/* "标记"开放多段线的每个点 */
	struct Marked_polyline : Open_polyline
	{
		Marked_polyline(const string& m)
			: mark{m}
		{
			/* 为了避免draw_lines()函数访问可能不存在的符号,这里需要一个对空字符串的检测 */
			if (m.empty())
			{
				mark = "*";
			}
		}

		Marked_polyline(const string& m, std::initializer_list<Point> lst)
			: Open_polyline{lst},
			  mark{m}
		{
			if (m.empty())
			{
				mark = "*";
			}
		}

		void draw_lines() const override;
	private:
		string mark;
	};

	/* 不与线关联的标记 */
	struct Marks : Marked_polyline
	{
		Marks(const string& m) : Marked_polyline(m)
		{
			/* 将Marked_polyline的线设置为不可见(invisible) */
			set_color(Color(Color::invisible));
		}

		Marks(const string& m, std::initializer_list<Point> lst): Marked_polyline{m, lst}
		{
			/* 将Marked_polyline的线设置为不可见(invisible) */
			set_color(Color(Color::invisible));
		}
	};

	/* 标记一个孤立的Point */
	struct Mark : Marks
	{
		Mark(Point xy, char c) : Marks(string(1, c)) { add(xy); }
	};

	/*

	struct Marks : Shape {
		Marks(char m) : mark(string(1,m)) { }
		void add(Point p) { Shape::add(p); }
		void draw_lines() const;
	private:
		string mark;
	};
	*/

	struct Bad_image : Fl_Image
	{
		Bad_image(int h, int w) : Fl_Image(h, w, 0)
		{
		}

		void draw(int x, int y, int, int, int, int) override { draw_empty(x, y); }
	};

	/* 图像编码 */
	enum class Suffix
	{
		none,
		jpg,
		gif,
		bmp
	};

	Suffix get_encoding(const string& s);

	struct Image : Shape
	{
		Image(Point xy, string s, Suffix e = Suffix::none);
		~Image() { delete p; }
		void draw_lines() const override;

		/* 选择要显示图像的某个子图像 */
		void set_mask(Point xy, int ww, int hh)
		{
			w = ww;
			h = hh;
			cx = xy.x;
			cy = xy.y;
		}

		void move(int dx, int dy) override
		{
			Shape::move(dx, dy);
			p->draw(point(0).x, point(0).y);
		}

	private:
		/* 为关联到(cx,cy)处的图像定义"masking box" */
		int w, h, cx, cy; // define "masking box" within image relative to position (cx,cy)
		Fl_Image* p;
		Text fn;
	};
}
#endif
