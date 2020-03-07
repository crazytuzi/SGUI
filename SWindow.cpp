#include "SWindow.h"
#include "Graph.h"
#include "GUI.h"

namespace Graph_lib
{
	SWindow::SWindow(int ww, int hh, const string& title)
		: Fl_Window(ww, hh, title.c_str()), w(ww), h(hh)
	{
		init();
	}

	SWindow::SWindow(Point xy, int ww, int hh, const string& title)
		: Fl_Window(xy.x, xy.y, ww, hh, title.c_str()), w(ww), h(hh)
	{
		init();
	}

	void SWindow::init()
	{
		resizable(this);
		show();
	}

	//---------------------------------------------------- 

	void SWindow::draw()
	{
		Fl_Window::draw();
		for (unsigned int i = 0; i < shapes.size(); ++i) shapes[i]->draw();
	}

	void SWindow::attach(Widget& w)
	{
		begin(); // FTLK: begin attaching new Fl_Wigets to this window
		w.attach(*this); // let the Widget create its Fl_Wigits
		end(); // FTLK: stop attaching new Fl_Wigets to this window
	}

	void SWindow::detach(Widget& b)
	{
		b.hide();
	}

	void SWindow::attach(Shape& s)
	{
		shapes.push_back(&s);
		//		s.attached = this;
	}

	void SWindow::detach(Shape& s)
	{
		for (unsigned int i = shapes.size(); 0 < i; --i) // guess last attached will be first released
			if (shapes[i - 1] == &s)
				shapes.erase(shapes.begin() + (i - 1)); //&shapes[i-1]);
	}


	void SWindow::put_on_top(Shape& p)
	{
		for (int i = 0; i < shapes.size(); ++i)
		{
			if (&p == shapes[i])
			{
				for (++i; i < shapes.size(); ++i)
					shapes[i - 1] = shapes[i];
				shapes[shapes.size() - 1] = &p;
				return;
			}
		}
	}

	int gui_main() { return Fl::run(); }
} // Graph
