#include "column.h"
#include "column_win32.h"
#include "widget_helper_win32.h"
#include "signal_slot.h"

#include <iostream>

using namespace math;
using namespace gui_imp;
using namespace gui;

void column_impl_win32::make(const column_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();

	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	m_NextClientLeft = a_Initializer.get_left();
	m_NextClientTop = a_Initializer.get_top();

	m_MyLeft = a_Initializer.get_left();
	m_MyTop  = a_Initializer.get_top();

	m_MyWidth = 1;
	m_MyHeight = 1;

	m_Padding = 1;

	{
		widget_hook_win32(this);

		m_hWnd = CreateWindow(
			TEXT("STATIC"), 
			TEXT(""),
			widget_helper_win32::visible(a_Initializer) | WS_CHILD, 
			a_Initializer.get_left(), a_Initializer.get_top(), 
			a_Initializer.get_width(), a_Initializer.get_height(), 
			l_ParentWnd, 0, 0, 0);
	}
}

void column_impl_win32::on_child_resize(resize_args *a_Args){
	int l_Width = 0;
	int l_Height = m_Padding;

	for(size_t i = 0; i < count(); i++){
		size l_Size = get_child(i).get_size();

		if(l_Size.m_Width > l_Width) l_Width = l_Size.m_Width;

		get_child(i).set_pos(m_Padding, l_Height);

		l_Height += l_Size.m_Height + (2 * m_Padding);
	}

	set_size(m_Padding + l_Width + (2 * m_Padding), m_Padding + l_Height);
}

void column_impl_win32::add_child(widget &a_Widget){
	column_impl_base::add_child(a_Widget);

	a_Widget.set_pos(m_Padding, m_Padding + m_MyHeight);
	a_Widget.m_OnResize += [this](resize_args* a_Args){
		on_child_resize(a_Args);
	};

	size l_Size = a_Widget.get_size();

	if(m_MyWidth < l_Size.m_Width + (2 * m_Padding))
		m_MyWidth = l_Size.m_Width + (2 * m_Padding);

	m_MyHeight += l_Size.m_Height + (2 * m_Padding);

	set_size(m_Padding + l_Size.m_Width, m_Padding + l_Size.m_Height);
}

LRESULT column_impl_win32::process_message( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}