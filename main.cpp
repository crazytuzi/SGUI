#include "Simple_window.h"	// 访问window库
#include "Graph.h"	// 访问图形库工具
using namespace Graph_lib; // 图形库工具在Graph_lib中

int main()
{
	try
	{
		Lines_window win{Point{100, 100}, 600, 400, "lines"};

		return gui_main();
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
