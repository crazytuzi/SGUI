#include "Graph.h"
#include "FL/Fl_JPEG_Image.H"
#include "FL/Fl_GIF_Image.H"
#include<map>

namespace Graph_lib
{
	void Shape::draw_lines() const
	{
		if (color().visibility() && 1 < points.size()) // draw sole pixel?
			for (unsigned int i = 1; i < points.size(); ++i)
				fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
	}

	void Shape::draw() const
	{
		Fl_Color oldc = fl_color();
		// there is no good portable way of retrieving the current style
		/* 没有好的可移植方式来获取当前的线型 */
		/* 设置颜色 */
		fl_color(lcolor.as_int());
		/* 设置线型 */
		fl_line_style(ls.style(), ls.width());
		draw_lines();
		/* 重置颜色(改为绘制之前颜色) */
		fl_color(oldc); // reset color (to pevious) and style (to default)
		/* 重置线型为默认值 */
		fl_line_style(0);
	}


	// does two lines (p1,p2) and (p3,p4) intersect?
	// if se return the distance of the intersect point as distances from p1
	inline pair<double, double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel)
	{
		double x1 = p1.x;
		double x2 = p2.x;
		double x3 = p3.x;
		double x4 = p4.x;
		double y1 = p1.y;
		double y2 = p2.y;
		double y3 = p3.y;
		double y4 = p4.y;

		double denom = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
		if (denom == 0)
		{
			parallel = true;
			return pair<double, double>(0, 0);
		}
		parallel = false;
		return pair<double, double>(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom,
		                            ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom);
	}


	//intersection between two line segments
	//Returns true if the two segments intersect,
	//in which case intersection is set to the point of intersection
	bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection)
	{
		bool parallel;
		pair<double, double> u = line_intersect(p1, p2, p3, p4, parallel);
		if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
		intersection.x = p1.x + u.first * (p2.x - p1.x);
		intersection.y = p1.y + u.first * (p2.y - p1.y);
		return true;
	}

	void Polygon::add(Point p)
	{
		/* 检查新的线没有和现有的线交叉 */

		int np = number_of_points();

		if (1 < np)
		{
			// check that thenew line isn't parallel to the previous one
			if (p == point(np - 1)) error("polygon point equal to previous point");
			bool parallel;
			line_intersect(point(np - 1), p, point(np - 2), point(np - 1), parallel);
			if (parallel)
				error("two polygon points lie in a straight line");
		}

		for (int i = 1; i < np - 1; ++i)
		{
			// check that new segment doesn't interset and old point
			Point ignore(0, 0);
			if (line_segment_intersect(point(np - 1), p, point(i - 1), point(i), ignore))
				error("intersect in polygon");
		}


		Closed_polyline::add(p);
	}


	void Polygon::draw_lines() const
	{
		if (number_of_points() < 3) error("less than 3 points in a Polygon");
		Closed_polyline::draw_lines();
	}

	void Open_polyline::draw_lines() const
	{
		if (fill_color().visibility())
		{
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for (int i = 0; i < number_of_points(); ++i)
			{
				fl_vertex(point(i).x, point(i).y);
			}
			fl_end_complex_polygon();
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility())
			Shape::draw_lines();
	}


	void Closed_polyline::draw_lines() const
	{
		/* 先画"开放的多线条部分" */
		Open_polyline::draw_lines();

		/* 然后画闭合线 */
		if (color().visibility()) // draw closing line:
			fl_line(point(number_of_points() - 1).x, point(number_of_points() - 1).y, point(0).x, point(0).y);
	}

	void Shape::move(int dx, int dy)
	{
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			points[i].x += dx;
			points[i].y += dy;
		}
	}

	void Lines::draw_lines() const
	{
		//	if (number_of_points()%2==1) error("odd number of points in set of lines");
		if (color().visibility())
			for (int i = 1; i < number_of_points(); i += 2)
				fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
	}

	void Text::draw_lines() const
	{
		int ofnt = fl_font();
		int osz = fl_size();
		fl_font(fnt.as_int(), fnt_sz);
		fl_draw(lab.c_str(), point(0).x, point(0).y);
		fl_font(ofnt, osz);
	}

	/*
	 * 将(0,0)置于xy,绘制由count条线段组成的f(x)函数图,x取值为[r1,r2)
	 * x坐标和y坐标分别由xscale和yscale进行比例设定
	 */
	Function::Function(Fct f, double r1, double r2, Point xy, int count, double xscale, double yscale)
	// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
	// x coordinates are scaled by xscale and y coordinates scaled by yscale
	{
		if (r2 - r1 <= 0) error("bad graphing range");
		if (count <= 0) error("non-positive graphing count");
		double dist = (r2 - r1) / count;
		double r = r1;
		for (int i = 0; i < count; ++i)
		{
			add(Point(xy.x + int(r * xscale), xy.y - int(f(r) * yscale)));
			r += dist;
		}
	}

	void Rectangle::draw_lines() const
	{
		/* 填充 */
		if (fill_color().visibility())
		{
			// fill
			fl_color(fill_color().as_int());
			fl_rectf(point(0).x, point(0).y, w, h);
			fl_color(color().as_int()); // reset color
		}

		/* 填充区域上层的线 */
		if (color().visibility())
		{
			// edge on top of fill
			fl_color(color().as_int());
			fl_rect(point(0).x, point(0).y, w, h);
		}
	}


	Axis::Axis(Orientation d, Point xy, int length, int n, string lab)
		: label(Point(0, 0), lab)
	{
		if (length < 0) error("bad axis length");
		switch (d)
		{
		case x:
			{
				/* 坐标线 */
				add(xy); // axis line
				add(Point(xy.x + length, xy.y)); // axis line
				/* 添加刻度 */
				if (1 < n)
				{
					int dist = length / n;
					int x = xy.x + dist;
					for (int i = 0; i < n; ++i)
					{
						notches.add(Point(x, xy.y), Point(x, xy.y - 5));
						x += dist;
					}
				}
				// label under the line
				/* 在线的下方放置标签 */
				label.move(length / 3, xy.y + 20);
				break;
			}
		case y:
			{
				/* 生成一个y轴 */
				add(xy); // a y-axis goes up
				add(Point(xy.x, xy.y - length));
				if (1 < n)
				{
					/* 添加刻度 */
					int dist = length / n;
					int y = xy.y - dist;
					for (int i = 0; i < n; ++i)
					{
						notches.add(Point(xy.x, y), Point(xy.x + 5, y));
						y -= dist;
					}
				}
				// label at top
				/* 在顶部放置标签 */
				label.move(xy.x - 10, xy.y - length - 10);
				break;
			}
		case z:
			error("z axis not implemented");
		}
	}

	void Axis::draw_lines() const
	{
		Shape::draw_lines(); // the line
		/* 刻度可能和线具有不同的颜色 */
		notches.draw(); // the notches may have a different color from the line
		/* 标签可能和线具有不同的颜色 */
		label.draw(); // the label may have a different color from the line
	}


	void Axis::set_color(Color c)
	{
		Shape::set_color(c);
		notches.set_color(c);
		label.set_color(c);
	}

	void Axis::move(int dx, int dy)
	{
		Shape::move(dx, dy);
		notches.move(dx, dy);
		label.move(dx, dy);
	}

	void Circle::draw_lines() const
	{
		if (fill_color().visibility())
		{
			// fill
			fl_color(fill_color().as_int());
			fl_pie(point(0).x, point(0).y, r + r - 1, r + r - 1, 0, 360);
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility())
		{
			fl_color(color().as_int());
			fl_arc(
				point(0).x, point(0).y, /* 左上角 */
				r + r, r + r, /* 外接矩形的宽度和高度 */
				0, 360 /* 绘制的起止角度 */
			);
		}
	}


	void Ellipse::draw_lines() const
	{
		if (fill_color().visibility())
		{
			// fill
			fl_color(fill_color().as_int());
			fl_pie(point(0).x, point(0).y, w + w - 1, h + h - 1, 0, 360);
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility())
		{
			fl_color(color().as_int());
			fl_arc(point(0).x, point(0).y, w + w, h + h, 0, 360);
		}
	}

	void draw_mark(Point xy, char c)
	{
		/* 用来使字符居中显示 */
		static const int dx = 4;
		static const int dy = 4;
		/* 含有单个字符c的字符串 */
		string m(1, c);
		fl_draw(m.c_str(), xy.x - dx, xy.y + dy);
	}

	void Marked_polyline::draw_lines() const
	{
		/* 划线 */
		Open_polyline::draw_lines();
		for (int i = 0; i < number_of_points(); ++i)
			draw_mark(point(i), mark[i % mark.size()]);
	}

	/*
	void Marks::draw_lines() const
	{
		for (int i=0; i<number_of_points(); ++i)
			fl_draw(mark.c_str(),point(i).x-4,point(i).y+4);
	}
	*/


	std::map<string, Suffix> suffix_map;

	int init_suffix_map()
	{
		suffix_map["jpg"] = Suffix::jpg;
		suffix_map["JPG"] = Suffix::jpg;
		suffix_map["jpeg"] = Suffix::jpg;
		suffix_map["JPEG"] = Suffix::jpg;
		suffix_map["gif"] = Suffix::gif;
		suffix_map["GIF"] = Suffix::gif;
		suffix_map["bmp"] = Suffix::bmp;
		suffix_map["BMP"] = Suffix::bmp;
		return 0;
	}

	Suffix get_encoding(const string& s)
	// try to deduce type from file name using a lookup table
	{
		static int x = init_suffix_map();

		string::const_iterator p = find(s.begin(), s.end(), '.');
		if (p == s.end()) return Suffix::none; // no suffix

		string suf(p + 1, s.end());
		return suffix_map[suf];
	}

	/* 检测名为s的文件是否存在以及是否可打开进行读操作 */
	bool can_open(const string& s)
	// check if a file named s exists and can be opened for reading
	{
		ifstream ff(s.c_str());
		return ff.is_open();
	}


	// somewhat overelaborate constructor
	// because errors related to image files can be such a pain to debug
	Image::Image(Point xy, string s, Suffix e)
		: w(0), h(0), fn(xy, "")
	{
		add(xy);

		/* 我们能打开s吗? */
		if (!can_open(s))
		{
			fn.set_label("cannot open \"" + s + '\"');
			/* 显示error Image */
			p = new Bad_image(30, 20); // the "error image"
			return;
		}

		if (e == Suffix::none) e = get_encoding(s);

		/* 检查是否为已知编码格式 */
		switch (e)
		{
		case Suffix::jpg:
			p = new Fl_JPEG_Image(s.c_str());
			break;
		case Suffix::gif:
			p = new Fl_GIF_Image(s.c_str());
			break;
			//	case Suffix::bmp:
			//		p = new Fl_BMP_Image(s.c_str());
			//		break;
		default: // Unsupported image encoding /* 未知的编码格式 */
			fn.set_label("unsupported file type \"" + s + '\"');
			/* 显示error Image */
			p = new Bad_image(30, 20); // the "error image"
		}
	}

	void Image::draw_lines() const
	{
		if (fn.label() != "") fn.draw_lines();

		if (w && h)
			p->draw(point(0).x, point(0).y, w, h, cx, cy);
		else
			p->draw(point(0).x, point(0).y);
	}
} // Graph
