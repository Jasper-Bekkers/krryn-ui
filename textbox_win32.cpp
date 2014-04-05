#include "textbox.h"
#include "textbox_win32.h"
#include "widget_helper_win32.h"

using namespace gui_imp;
using namespace gui;

void textbox_impl_win32::make(const textbox_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();

	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	widget_hook_win32(this);

	m_hWnd = CreateWindow(
		TEXT("EDIT"), 
		NULL, 
		WS_TABSTOP | widget_helper_win32::visible(a_Initializer) | WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
		a_Initializer.get_left(), a_Initializer.get_top(), 
		a_Initializer.get_width(), a_Initializer.get_height(), 
		l_ParentWnd, 0, 0, 0);

	widget_helper_win32::fix_font(m_hWnd);
}

void textbox_impl_win32::set_text(const std::string &a_Text){
	SetWindowTextA(m_hWnd, a_Text.c_str());
	SendMessage(m_hWnd, WM_COMMAND, (WPARAM)(DWORD)EN_UPDATE << 16, (LPARAM)m_hWnd);
}

std::string textbox_impl_win32::get_text(){
	int l_Length = GetWindowTextLength(m_hWnd) + 1;
	char *l_String = (char*)alloca(l_Length);
	GetWindowTextA(m_hWnd, l_String, l_Length);
	return std::string(l_String, l_Length - 1);
}

LRESULT textbox_impl_win32::process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if(uMsg == WM_COMMAND){
		WORD l_Cmd = HIWORD(wParam);

		if(l_Cmd == EN_UPDATE){
			event_args e;
			((textbox*)get_widget())->m_OnTextChange.trigger(&e);
		}
	}

	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}