#pragma once
#include "widget_win32.h"
#include "code_highlighter_impl_base.h"

namespace gui_imp{
	const COLORREF white = RGB(0xff,0xff,0xff);

	class code_highlighter_impl_win32 : public virtual widget_win32, public virtual gui::code_highlighter_impl_base{
	public:
		virtual void make(const gui::code_highlighter_initializer &a_Initializer);
		virtual void set_text(const std::string &a_Text);
	private:
		void send(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0);
		void code_highlighter_impl_win32::set_style(int style, COLORREF fore, COLORREF back=white, int size=-1, const char *face=0);
	};
}
