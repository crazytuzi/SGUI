#include "Simple_window.h"	// 访问window库
#include "Graph.h"	// 访问图形库工具

int main()
{
	using namespace Graph_lib; // 图形库工具在Graph_lib中

	const Point tl{100, 100}; // 将起点置于窗口左上角

	Simple_window win{tl, 600, 400, "Canvas"}; // 生成一个简单的窗口

	Graph_lib::Polygon poly; // 生成一个shape(一个polygon)

	poly.add(Point{300, 200}); // 加入一个点
	poly.add(Point{350, 100}); // 加入另一个点
	poly.add(Point{400, 200}); // 加入第三个点

	poly.set_color(Color::red); // 调整poly的属性

	win.attach(poly); // 将ploy关联到窗口

	win.wait_for_button(); // 将控制权交给显示引擎

	return 0;
}
