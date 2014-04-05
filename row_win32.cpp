#include "row.h"
#include "row_win32.h"
#include "widget_helper_win32.h"
#include "signal_slot.h"

#include <iostream>

using namespace math;
using namespace gui_imp;
using namespace gui;

void row_impl_win32::make(const row_initializer &a_Initializer){
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

void row_impl_win32::on_child_resize(resize_args *a_Args){
	int l_Width = m_Padding;
	int l_Height = 0;

	for(size_t i = 0; i < count(); i++){
		size l_Size = get_child(i).get_size();

		if(l_Size.m_Height > l_Height) l_Height = l_Size.m_Height;

		get_child(i).set_pos(l_Width, m_Padding);

		l_Width += l_Size.m_Width + (2 * m_Padding);
	}

	set_size(m_Padding + l_Width, m_Padding + l_Height + (2 * m_Padding));
}

void row_impl_win32::add_child(widget &a_Widget){
	row_impl_base::add_child(a_Widget);
	a_Widget.m_OnResize += slot(this, &row_impl_win32::on_child_resize);

	a_Widget.set_pos(m_Padding + m_MyWidth, m_Padding );
	size l_Size = a_Widget.get_size();

	if(m_MyHeight < l_Size.m_Height + (2 * m_Padding))
		m_MyHeight = l_Size.m_Height + (2 * m_Padding);

	m_MyWidth += l_Size.m_Width + (2 * m_Padding);

	set_size(m_Padding + m_MyWidth, m_Padding + m_MyHeight);
}

LRESULT row_impl_win32::process_message( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}