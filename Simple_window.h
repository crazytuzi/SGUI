#pragma once

#include "GUI.h"	// for Simple_window only (doesn't really belong in SWindow.h)

using namespace Graph_lib;

// Simple_window is basic scaffolding for ultra-simple interaction with graphics
// it provides one window with one "next" button for ultra-simple animation

struct Simple_window : SWindow
{
	Simple_window(Point xy, int w, int h, const string& title)
		: SWindow(xy, w, h, title),
		  next_button(Point(x_max() - 70, 0), 70, 20, "Next", cb_next),
		  button_pushed(false)
	{
		attach(next_button);
	}

	/* 简单的事件循环 */
	void wait_for_button()
	// modified event loop
	// handle all events (as per default), but quit when button_pushed becomes true
	// this allows graphics without control inversion
	{
		/*
		 * 修改事件循环
		 * 处理所有事件(按每个事件的默认方式),当button_pushed为true时退出
		 * 这条语句实现绘图而不会有控制流反转
		 */
		while (!button_pushed) Fl::wait();
		button_pushed = false;
		Fl::redraw();
	}

	/* Next按钮 */
	Button next_button;
private:
	/* 实现细节 */
	bool button_pushed;

	/* next_button的回调 */
	/* 对位于addr的窗口调用Simple_window::next() */
	static void cb_next(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		/* 为位于pw处的窗口调用Simple_window::next() */
		static_cast<Simple_window*>(addr)->next();
	}

	/* next_button被按下时将执行相关动作 */
	void next() { button_pushed = true; }
};


struct Lines_window : SWindow
{
	Lines_window(Point xy, int w, int h, const string& title)
		: SWindow(xy, w, h, title),
		  next_button{Point{x_max() - 150, 0}, 70, 20, "Next point", cb_next},
		  quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
		  next_x{Point{x_max() - 310, 0}, 50, 20, "next x:"},
		  next_y{Point{x_max() - 210, 0}, 50, 20, "next y:"},
		  xy_out{Point{100, 0}, 100, 20, "current(x,y):"},
		  color_menu{Point(x_max() - 70, 30), 70, 20, Menu::vertical, "color"},
		  menu_button{Point{x_max() - 80, 30}, 80, 20, "color menu", cb_menu}
	{
		attach(next_button);
		attach(quit_button);
		attach(next_x);
		attach(next_y);
		attach(xy_out);
		xy_out.put("no point");

		color_menu.attach(new Button{Point{0, 0}, 0, 0, "red", cb_red});
		color_menu.attach(new Button{Point{0, 0}, 0, 0, "blue", cb_blue});
		color_menu.attach(new Button{Point{0, 0}, 0, 0, "black", cb_black});
		attach(color_menu);
		color_menu.hide();

		attach(menu_button);

		attach(lines);
	}

private:
	/* 数据 */
	Open_polyline lines;

	/* 构件 */
	/* 将(next_x,next_y)添加到线 */
	Button next_button;
	Button quit_button;
	In_box next_x;
	In_box next_y;
	Out_box xy_out;
	Menu color_menu;
	Button menu_button;

	void next()
	{
		const auto x = next_x.get_int();
		const auto y = next_y.get_int();
		lines.add(Point{x, y});

		/* 更新当前的位置读数 */
		ostringstream ss;
		ss << '(' << x << ',' << y << ')';
		xy_out.put(ss.str());

		redraw();
	}

	void hide_menu()
	{
		color_menu.hide();

		menu_button.show();
	}

	void quit()
	{
		/* 用于删除窗口的FLTK奇怪特性 */
		hide();
	}

	void change(Color c)
	{
		lines.set_color(c);
	}

	/* 由回调函数激活的动作 */
	void red_pressed()
	{
		change(Color::red);
		hide_menu();
	}

	void blue_pressed()
	{
		change(Color::blue);
		hide_menu();
	}

	void black_pressed()
	{
		change(Color::black);
		hide_menu();
	}

	void menu_pressed()
	{
		menu_button.hide();

		color_menu.show();
	}

	/* 红色按钮的回调 */
	static void cb_red(Address, Address pw)
	{
		reference_to<Lines_window>(pw).red_pressed();
	}

	/* 蓝色按钮的回调 */
	static void cb_blue(Address, Address pw)
	{
		reference_to<Lines_window>(pw).blue_pressed();
	}

	/* 黑色按钮的回调 */
	static void cb_black(Address, Address pw)
	{
		reference_to<Lines_window>(pw).black_pressed();
	}

	static void cb_menu(Address, Address pw)
	{
		reference_to<Lines_window>(pw).menu_pressed();
	}

	static void cb_next(Address, Address pw)
	{
		reference_to<Lines_window>(pw).next();
	}

	static void cb_quit(Address, Address pw)
	{
		reference_to<Lines_window>(pw).quit();
	}
};
