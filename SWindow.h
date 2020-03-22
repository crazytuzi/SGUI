#pragma once
#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1


#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "std_lib_facilities.h"

#include "Point.h"

//#include "GUI.h"

namespace Graph_lib
{
	class Shape; // "forward declare" Shape
	class Widget;

	class SWindow : public Fl_Window
	{
	public:
		/* 令系统选择位置 */
		SWindow(int w, int h, const string& title); // let the system pick the location
		// 左上角在xy处
		SWindow(Point xy, int w, int h, const string& title); // top left corner in xy
		virtual ~SWindow()
		{
		}

		int x_max() const { return w; }
		int y_max() const { return h; }

		void resize(int ww, int hh)
		{
			w = ww, h = hh;
			size(ww, hh);
		}

		void set_label(const string& s) { label(s.c_str()); }

		void attach(Shape& s);
		void attach(Widget& w);

		/* 从形状中删除w */
		void detach(Shape& s); // remove s from shapes 
		/* 从窗口中删除w(吊销回调函数) */
		void detach(Widget& w); // remove w from window (deactivate callbacks)

		/* 将一个形状放在最顶层 */
		void put_on_top(Shape& p); // put p on top of other shapes

	protected:
		void draw() override;

	private:
		/* 附属于窗口的形状 */
		vector<Shape*> shapes; // shapes attached to window
		/* 窗口大小 */
		int w, h; // window size

		void init();
	};

	int gui_main(); // invoke GUI library's main event loop

	inline int x_max() { return Fl::w(); } // width of screen in pixels
	inline int y_max() { return Fl::h(); } // height of screen in pixels
}
#endif
