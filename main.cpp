#include "Simple_window.h"	// 访问window库
#include "Graph.h"	// 访问图形库工具
using namespace Graph_lib; // 图形库工具在Graph_lib中

/* 有四种方法可以令按钮向四处移动 */
/* 显示/隐藏、改变位置、创建一个新按钮以及附着/分离 */
class W7 : public SWindow
{
public:
	W7(int w, int h, const string& t): SWindow{w, h, t},
	                                   sh_left{true}, mv_left{true}, cd_left{true}, ad_left{true}
	{
		p1 = new Button{Point{100, 100}, 50, 20, "show", cb_sh};

		p2 = new Button{Point{200, 100}, 50, 20, "hide", cb_sh};

		mvp = new Button{Point{100, 200}, 50, 20, "move", cb_mv};

		cdp = new Button{Point{100, 300}, 50, 20, "create", cb_cd};

		adp1 = new Button{Point{100, 400}, 50, 20, "activate", cb_ad};

		adp2 = new Button{Point{200, 400}, 50, 20, "deactivate", cb_ad};

		attach(*p1);

		attach(*p2);

		attach(*mvp);

		attach(*cdp);

		p2->hide();

		attach(*adp1);
	}

	/* 显示/隐藏 */
	Button* p1;

	Button* p2;

	bool sh_left;

	/* 移动 */
	Button* mvp;

	bool mv_left;

	/* 创建/销毁 */
	Button* cdp;

	bool cd_left;

	/* 激活/吊销 */
	Button* adp1;

	Button* adp2;

	bool ad_left;

	/* 动作 */
	/* 隐藏一个按钮,显示另外一个 */
	void sh()
	{
		if (sh_left)
		{
			p1->hide();

			p2->show();
		}
		else
		{
			p1->show();

			p2->hide();
		}

		sh_left = !sh_left;
	}

	/* 移动按钮 */
	void mv()
	{
		if (mv_left)
		{
			mvp->move(100, 0);
		}
		else
		{
			mvp->move(-100, 0);
		}

		mv_left = !mv_left;
	}

	/* 释放按钮并创建一个新的 */
	void cd()
	{
		cdp->hide();

		delete cdp;

		string lab = "create";

		auto x = 100;

		if (cd_left)
		{
			lab = "delete";

			x = 200;
		}

		cdp = new Button{Point{x, 300}, 50, 20, lab, cb_cd};

		attach(*cdp);

		cd_left = !cd_left;
	}

	/* 将按钮与窗口分离并将其替代者附着于窗口 */
	void ad()
	{
		if (ad_left)
		{
			detach(*adp1);

			attach(*adp2);
		}
		else
		{
			detach(*adp2);

			attach(*adp1);
		}

		ad_left = !ad_left;
	}

	/* 回调函数 */
	static void cb_sh(Address, Address addr)
	{
		reference_to<W7>(addr).sh();
	}

	static void cb_mv(Address, Address addr)
	{
		reference_to<W7>(addr).mv();
	}

	static void cb_cd(Address, Address addr)
	{
		reference_to<W7>(addr).cd();
	}

	static void cb_ad(Address, Address addr)
	{
		reference_to<W7>(addr).ad();
	}
};

int main()
{
	try
	{
		W7 w{400, 500, "move"};

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
