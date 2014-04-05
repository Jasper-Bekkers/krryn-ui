#ifdef _WIN32

#pragma once
#include "textbox_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class textbox_impl_win32 : public virtual widget_win32, public virtual gui::textbox_impl_base{
	public:
		virtual void make(const gui::textbox_initializer &a_Initializer);
		virtual void *handle() { return m_hWnd; }

		virtual void set_text(const std::string &a_Text);
		virtual std::string get_text();

		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}

#endif