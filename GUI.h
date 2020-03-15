#pragma once

//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef GUI_GUARD
#define GUI_GUARD

#include "SWindow.h"
#include "Graph.h"

namespace Graph_lib
{
	//------------------------------------------------------------------------------

	typedef void* Address; // Address is a synonym for void*
	typedef void (*Callback)(Address, Address); // FLTK's required function type for all callbacks

	//------------------------------------------------------------------------------

	template <class W>
	W& reference_to(Address pw)
	// treat an address as a reference to a W
	{
		return *static_cast<W*>(pw);
	}

	//------------------------------------------------------------------------------

	/*
	 * Widget是一个Fl_widget的句柄,不是Fl_widget
	 * 我们尽量使接口类独立于FLTK
	 */
	class Widget
	{
		// Widget is a handle to an Fl_widget - it is *not* an Fl_widget
		// We try to keep our interface classes at arm's length from FLTK
	public:
		Widget(Point xy, int w, int h, const string& s, Callback cb)
			: loc(xy), width(w), height(h), label(s), do_it(cb)
		{
		}

		virtual void move(int dx, int dy)
		{
			hide();
			pw->position(loc.x += dx, loc.y += dy);
			show();
		}

		virtual void hide() { pw->hide(); }
		virtual void show() { pw->show(); }
		virtual void attach(SWindow&) = 0;

		Point loc;
		int width;
		int height;
		string label;
		Callback do_it;

		virtual ~Widget()
		{
		}

	protected:
		/* 每个Widget都属于一个Window */
		SWindow* own; // every Widget belongs to a SWindow
		/* 连接到FLTK Widget */
		Fl_Widget* pw; // connection to the FLTK Widget
	private:
		Widget& operator=(const Widget&); // don't copy Widgets
		Widget(const Widget&);
	};

	//------------------------------------------------------------------------------

	struct Button : Widget
	{
		Button(Point xy, int w, int h, const string& label, Callback cb)
			: Widget(xy, w, h, label, cb)
		{
		}

		void attach(SWindow&) override;
	};

	//------------------------------------------------------------------------------

	struct In_box : Widget
	{
		In_box(Point xy, int w, int h, const string& s)
			: Widget(xy, w, h, s, nullptr)
		{
		}

		int get_int();
		string get_string();

		void attach(SWindow& win) override;
	};

	//------------------------------------------------------------------------------

	struct Out_box : Widget
	{
		Out_box(Point xy, int w, int h, const string& s)
			: Widget(xy, w, h, s, nullptr)
		{
		}

		void put(int);
		void put(const string&);

		void attach(SWindow& win) override;
	};

	//------------------------------------------------------------------------------

	struct Menu : Widget
	{
		enum Kind { horizontal, vertical };

		Menu(Point xy, int w, int h, Kind kk, const string& label)
			: Widget(xy, w, h, label, nullptr), k(kk), offset(0)
		{
		}

		Vector_ref<Button> selection;
		Kind k;
		int offset;
		/* 将Button添加到Menu */
		int attach(Button& b); // Menu does not delete &b
		/* 添加新的Button到Menu */
		int attach(Button* p); // Menu deletes p
		/* 显示所有的Button */
		void show() override
		// show all buttons
		{
			for (unsigned int i = 0; i < selection.size(); ++i)
				selection[i].show();
		}
		/* 隐藏所有的Button */
		void hide() override
		// hide all buttons
		{
			for (unsigned int i = 0; i < selection.size(); ++i)
				selection[i].hide();
		}

		/* 移动所有的Button */
		void move(int dx, int dy) override
		// move all buttons
		{
			for (unsigned int i = 0; i < selection.size(); ++i)
				selection[i].move(dx, dy);
		}

		/* 将所有的Button添加到窗口win中 */
		void attach(SWindow& win) override
		// attach all buttons
		{
			for (int i = 0; i < selection.size(); ++i) win.attach(selection[i]);
			own = &win;
		}
	};

	//------------------------------------------------------------------------------
} // of namespace Graph_lib

#endif // GUI_GUARD
