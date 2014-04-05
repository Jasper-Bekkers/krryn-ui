#include "button.h"
#include "button_win32.h"
#include "widget_helper_win32.h"

using namespace gui_imp;
using namespace gui;

void button_impl_win32::make(const button_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();
	
	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{
		widget_hook_win32(this);

		m_hWnd = CreateWindow(
			TEXT("BUTTON"), 
			a_Initializer.get_text().c_str(), 
			WS_TABSTOP | widget_helper_win32::visible(a_Initializer) | WS_CHILD | BS_PUSHBUTTON, 
			a_Initializer.get_left(), a_Initializer.get_top(), 
			a_Initializer.get_width(), a_Initializer.get_height(), 
			l_ParentWnd, 0, 0, 0);
	}
}

LRESULT button_impl_win32::process_message( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(uMsg == WM_COMMAND){
		WORD l_Cmd = HIWORD(wParam);

		if(l_Cmd == BN_CLICKED){
			event_args a;
			((button*)get_widget())->m_OnClick.trigger(&a);
		}
	}

	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}