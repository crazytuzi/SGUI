#include "Simple_window.h"	// 访问window库
#include "Graph.h"	// 访问图形库工具
using namespace Graph_lib; // 图形库工具在Graph_lib中

int main()
{
	try
	{
		Point tl{100, 100}; // 我们窗口的左上角

		Simple_window win{
			tl, // 屏幕坐标点tl对应左上角
			600, 400, // 窗口大小(600*400)
			"Canvas" // 标题: Canvas
		};

		/* --------------- 坐标轴 --------------- */

		Axis xa{
			// 生成一个坐标轴,Axis是一种Shape
			Axis::x, // Axis::x表示是水平的
			Point{20, 300}, // 从点(20,300)处开始
			280, // 280像素长
			10, // 10个"刻度"
			"x axis" // 坐标轴标签为"x axis"
		};

		win.attach(xa); // 将xa添加到窗口win

		Axis ya{Axis::y, Point{20, 300}, 280, 10, "y axis"};
		ya.set_color(Color::cyan); // 选择一种颜色
		ya.label.set_color(Color::dark_red); // 选择一种文本的颜色
		win.attach(ya);

		/* --------------- 函数图 --------------- */

		/*
		 * 正弦曲线
		 * 在(20,150)位置处设置坐标原点(0,0),绘制[0,100]区间上的sin()函数
		 * 使用1000个点,x值的比例乘50,y值的比例乘50
		 */
		Function sine{sin, 0, 100, Point{20, 150}, 1000, 50, 50};

		sine.set_color(Color::blue); // 我们改表正弦曲线的颜色

		win.attach(sine);

		/* --------------- Polygon --------------- */

		Graph_lib::Polygon poly; // 一个多边形,Polygon是一种Shape
		poly.add(Point{300, 200}); // 3个点构成一个三角形
		poly.add(Point{350, 100});
		poly.add(Point{400, 200});

		poly.set_color(Color::red);
		poly.set_style(Line_style::dash);

		win.attach(poly);

		/* --------------- Rectangle --------------- */

		Graph_lib::Rectangle r{
			Point{200, 200}, // 左上角
			100, // 宽度
			50 // 高度
		};

		win.attach(r);

		/* --------------- ClosedPolyLine --------------- */

		Closed_polyline poly_rect;
		poly_rect.add(Point{100, 50});
		poly_rect.add(Point{200, 50});
		poly_rect.add(Point{200, 100});
		poly_rect.add(Point{100, 100});
		poly_rect.add(Point{50, 75});

		win.attach(poly_rect);

		/* --------------- 填充 --------------- */

		r.set_fill_color(Color::yellow); // 为矩形内部填充颜色
		poly.set_style(Line_style(Line_style::dash, 4));
		poly_rect.set_style(Line_style(Line_style::dash, 2));
		poly_rect.set_fill_color(Color::green);

		/* --------------- Text --------------- */

		Text t{Point{150, 150}, "Hello,graphical world!"};
		t.set_font(Font::times_bold);
		t.set_font_size(20);

		win.attach(t);

		/* --------------- Image --------------- */

		Image ii{Point{100, 50}, "Resources/image.jpg"};
		ii.move(300, 200);

		win.attach(ii);

		/* --------------- More --------------- */

		Circle c{Point{100, 200}, 50};

		win.attach(c);

		Graph_lib::Ellipse e{Point{100, 200}, 75, 25};
		e.set_color(Color::dark_red);

		win.attach(e);

		Mark m{Point{100, 200}, 'x'};

		win.attach(m);

		ostringstream oss;
		oss << "screen size: " << x_max() << "*" << y_max()
			<< "; window size: " << win.x_max() << "*" << win.y_max();
		Text sizes{Point{100, 20}, oss.str()};

		win.attach(sizes);

		Image cal{Point{225, 225}, "Resources/wdnmd.gif"};
		cal.set_mask(Point{40, 40}, 200, 150);

		win.attach(cal);

		win.wait_for_button(); // 显示!
	}
	catch (exception& e)
	{
		// 报告一些错误
		return 1;
	}
	catch (...)
	{
		// 更多的错误报告
		return 2;
	}

	return 0;
}
