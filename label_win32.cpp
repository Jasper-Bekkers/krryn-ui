#include "label.h"
#include "label_win32.h"
#include "widget_helper_win32.h"

using namespace gui_imp;
using namespace gui;

void label_impl_win32::make(const label_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();

	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{
		widget_hook_win32(this);

		m_hWnd = CreateWindow(
			TEXT("STATIC"), 
			a_Initializer.get_text().c_str(), 
			WS_TABSTOP | widget_helper_win32::visible(a_Initializer) | WS_CHILD, 
			a_Initializer.get_left(), a_Initializer.get_top(), 
			a_Initializer.get_width(), a_Initializer.get_height(), 
			l_ParentWnd, 0, 0, 0);
	}
}

void label_impl_win32::set_text(const std::string &a_Text){
	SetWindowTextA(m_hWnd, a_Text.c_str());
}
