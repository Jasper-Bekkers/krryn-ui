#include "window_win32.h"
#include "window.h"
#include "widget_helper_win32.h"

#include <windows.h>
#include <iostream>

using namespace gui;
using namespace gui_imp;

int gui_imp::window_impl_win32::m_WindowCounter = 0;

void window_impl_win32::make(const gui::window_initializer &a_Initializer){
	DWORD l_ExtraStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD l_Style = WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;

	RECT l_WindowRect = widget_helper_win32::get_window_rect(a_Initializer);
	RECT l_AdjustedRect = {0, 0, (LONG)a_Initializer.get_width(), (LONG)a_Initializer.get_height()};
	AdjustWindowRectEx(&l_AdjustedRect, l_Style, FALSE, l_ExtraStyle);

	m_hWnd = CreateWindowEx(
		l_ExtraStyle, widget_helper_win32::window_class(),
		a_Initializer.get_title().c_str(),
		l_Style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		l_WindowRect.left, l_WindowRect.top,
		l_AdjustedRect.right,
		l_AdjustedRect.bottom,
		NULL, NULL, NULL, NULL);

	if (!m_hWnd){
		throw widget_exception("Failed to create window");
	}

	attach(m_hWnd);

	std::clog 
		<< "Window '" << a_Initializer.get_title()  
		<< "' created" << std::endl;

	UpdateWindow(m_hWnd);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	ShowWindow(m_hWnd, SW_SHOW);
	m_WindowCounter++;
}

void window_impl_win32::on_detach(){
	m_WindowCounter--;

	if(m_WindowCounter == 0){
		exit(0);
	}
}