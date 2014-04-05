#include "widget_win32.h"
#include "widget_helper_win32.h"
#include <iostream>

using namespace gui_imp;
using namespace gui;
using namespace math;

widget_win32 * widget_win32::get_from_hwnd(HWND hWnd){
	return (widget_win32*)::GetProp(hWnd, TEXT("widget_ptr"));
}

void widget_win32::attach(HWND hWnd){
	if(!::SetProp(hWnd, TEXT("widget_ptr"), (widget_win32*)this)){
		std::cout << "setProp failed" << std::endl;
	}else{
		on_attach();
	}
}

void widget_win32::detach(HWND hWnd){
	::RemoveProp(hWnd, TEXT("widget_ptr"));
	on_detach();
}

LRESULT widget_win32::proc(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam){
	widget_win32 *l_Widget = get_from_hwnd(hWnd);

	if(l_Widget){
		return l_Widget->process_message(hWnd, uMsg, wParam, lParam);
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT widget_win32::process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_CREATE:
			widget_helper_win32::fix_font(hWnd);
			break;
		case WM_SIZE:
			{
				resize_args s;
				s.m_Width = LOWORD(lParam);
				s.m_Height = HIWORD(lParam);

				get_widget()->m_OnResize.trigger(&s);
			}
			break;
		case WM_DESTROY:
			detach(hWnd);
			break;
	}

	if(get_old_proc()){
		return ::CallWindowProc(get_old_proc(), hWnd, uMsg, wParam, lParam);
	}else{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void widget_win32::set_visible(bool a_IsVisible){
	widget_helper_win32::set_visible(m_hWnd, a_IsVisible);
}

bool widget_win32::get_visible(){
	return (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VISIBLE) == WS_VISIBLE;
}

void widget_win32::set_size(int a_Width, int a_Height){
	::SetWindowPos(m_hWnd, NULL, 0, 0, a_Width, a_Height, SWP_NOMOVE | SWP_NOZORDER);
}

void widget_win32::set_pos(int a_X, int a_Y){
	::SetWindowPos(m_hWnd, NULL, a_X, a_Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

point widget_win32::get_pos(){
	RECT l_Rect;
	::GetWindowRect(m_hWnd, &l_Rect);

	return point(l_Rect.left, l_Rect.top);
}

size widget_win32::get_size(){
	RECT l_Rect;
	::GetWindowRect(m_hWnd, &l_Rect);

	return size(l_Rect.right - l_Rect.left, l_Rect.bottom - l_Rect.top);
}