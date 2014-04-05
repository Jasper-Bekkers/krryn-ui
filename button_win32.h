#ifdef _WIN32

#pragma once
#include "button_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class button_impl_win32 : public virtual widget_win32, public virtual gui::button_impl_base{
	public:
		virtual void make(const gui::button_initializer &a_Initializer);
		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}

#endif