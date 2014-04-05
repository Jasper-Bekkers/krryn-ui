#include "tabs.h"
#include "tabs_win32.h"
#include "widget_helper_win32.h"
#include "signal_slot.h"

#include <iostream>
#include <unordered_set>

using namespace math;
using namespace gui_imp;
using namespace gui;
using namespace Gdiplus;

// TODO:
//  - Sort by MRU (increment counter when tab is activated)
//  - Sort by when a tab has been opened
//
//  - Make it look good
//  - Allow right clicking the tab bar
//	   - Display a menu to close, close right, close all but this one and some context sensitive info.
//
//  - How do we handle overflowing the tab?
//     - Dropdown
//     - Left / right buttons
//  - Do we implement docking? If so, how?
//  - Do we allow the tab to be inside a new window? (Multi montior support)
//     - If we do that, we probably need to relocate the HWND to another window
//       this means adding support for a non-destructive remove_child() function
//     - Could we also do this internally and create our own window?

// DONE:
//  - Draw tab bar
//     - Highlight active tab
//     - Close button
//  - Also, all childs need a title, how are we going to manage that?

namespace{
	char* tabs_class(){
		static bool m_IsRegistered = false;

		if(!m_IsRegistered){
			WNDCLASS l_WindowClass;
			memset(&l_WindowClass, 0, sizeof(l_WindowClass));

			l_WindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			l_WindowClass.lpfnWndProc	= (WNDPROC)DefWindowProc;
			l_WindowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
			l_WindowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
			l_WindowClass.lpszClassName	= TEXT("tabs_class");

			if(!RegisterClass(&l_WindowClass)){
				throw widget_exception("Can't register window class");
			}else{
				m_IsRegistered = true;
			}
		}

		return "tabs_class";
	}

	const unsigned int g_TabHeight = 28;

	RECT rectf_to_rect(RectF r){
		RECT t = {(INT)r.X, (INT)r.Y, (INT)r.X + r.Width, (INT)r.Y + r.Height};
		return t;
	}
}

void tab_panel_impl_win32::make(tabs *a_Parent){
	widget_hook_win32(this);

	size l_ParentSize = a_Parent->get_size();

	m_hWnd = CreateWindow(
		TEXT("STATIC"), 
		TEXT(""),
		WS_CHILD, 
		0, 0, 
		l_ParentSize.m_Width, l_ParentSize.m_Height,
		(HWND)a_Parent->handle(), 0, 0, 0);
}

LRESULT tab_panel_impl_win32::process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(uMsg == WM_COMMAND){
		widget_win32 *l_Widget = widget_win32::get_from_hwnd((HWND)lParam);

		if(l_Widget){
			l_Widget->process_message(hWnd, uMsg, wParam, lParam);
		}
	}

	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}

void tabs_impl_win32::make(const tabs_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();
	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{

		math::size l_ParentSize = a_Initializer.get_parent()->get_size();

		a_Initializer.get_parent()->m_OnResize += slot(this, &tabs_impl_win32::on_parent_resize);
		
		{
			widget_hook_win32(this);

			m_hWndTabs = CreateWindowEx(
				WS_EX_COMPOSITED,
				tabs_class(),
				TEXT(""), 
				widget_helper_win32::visible(a_Initializer) | WS_CHILD,
				0, 0,
				l_ParentSize.m_Width, g_TabHeight,
				l_ParentWnd, 0, 0, 0);
		}

		{
			widget_hook_win32(this);

			m_hWnd = CreateWindow(
				TEXT("STATIC"), 
				TEXT(""),
				widget_helper_win32::visible(a_Initializer) | WS_CHILD, 
				0, g_TabHeight + a_Initializer.get_top(), 
				l_ParentSize.m_Width, l_ParentSize.m_Height - g_TabHeight, 
				l_ParentWnd, 0, 0, 0);
		}
	}
}

void tabs_impl_win32::on_parent_resize(resize_args *a_Args){
	::SetWindowPos(m_hWndTabs, NULL, 0, 0, a_Args->m_Width, g_TabHeight, SWP_NOMOVE | SWP_NOZORDER);
	set_size(a_Args->m_Width, a_Args->m_Height - g_TabHeight);

	for(unsigned int i = 0; i < count(); i++){
		get_child(i).set_size(a_Args->m_Width, a_Args->m_Height - g_TabHeight);
	}

	InvalidateRect(m_hWnd, NULL, TRUE);
}

void tabs_impl_win32::on_child_resize(resize_args *a_Args){

}

void tabs_impl_win32::add_child(widget &a_Widget){
	tabs_impl_base::add_child(a_Widget);
	activate_last();
}

void tabs_impl_win32::remove_child(size_t i){
	tabs_impl_base::remove_child(i);
	activate_last();

	visible_tabs_t::iterator e = m_VisibleTabs.find(i);
	if(e != m_VisibleTabs.end()){
		m_VisibleTabs.erase(e);
	}
}

void tabs_impl_win32::activate_last(){
	m_ActiveTab = count() - 1;
	if(count() > 1){
		get_child(count() - 2).set_visible(false);
	}
	if(count() > 0){
		get_child(count() - 1).set_visible(true);
	}
}

void tabs_impl_win32::set_visible(bool a_IsVisible){
	widget_win32::set_visible(a_IsVisible);
	widget_helper_win32::set_visible(m_hWndTabs, a_IsVisible);
}

unsigned int tabs_impl_win32::draw_tab(Graphics &g, unsigned int a_X, unsigned int a_Y, unsigned int a_Index){
	if(a_X > get_size().m_Width) return a_X;

	Color c = Color(255, 44, 61, 91);

	if(m_ActiveTab == a_Index) c = Color(255, 255, 232, 166);
	else if(m_HoverTab == a_Index) c = Color(255, 100, 110, 117);

	SolidBrush b(c);
	SolidBrush t(m_ActiveTab == a_Index ? Color(255, 255, 248, 225) : c);
	SolidBrush black(Color(255, 0, 0, 0));
	SolidBrush white(Color(255, 255, 255, 255));
	SolidBrush closeDark(Color(255, 117, 99, 61));
	SolidBrush closeLight(Color(255, 206, 212, 221));
	Pen p(Color(255, 0, 0, 0));
	Font font(L"Tahoma", 10);
	PointF origin(a_X + 5, a_Y + 3), originX(a_X + 10, a_Y + 5);

	RectF bb, close;

	std::string s = this->get_child_name(a_Index);
	std::wstring w(s.begin(), s.end());

	// draw tab
	g.MeasureString(w.c_str(), w.size(), &font, origin, &bb);
	bb.Height = 25;
	originX.X += bb.Width + 5;
	close = RectF(originX.X, originX.Y, 15.f, 15.f);
	RectF::Union(bb, bb, close);

	GraphicsPath gp;

	// draw header
	int x = a_X; int y = a_Y;
	int headerHeight = get_size().m_Height / 2;
	int arcSize = 10;
	int width = bb.Width;

	// top part = arc
	gp.AddLine(x, y + headerHeight + arcSize / 2, x, y + arcSize / 2);
	gp.AddArc(x, y, arcSize, arcSize, 180.f, 90.f);
	gp.AddArc(x + width, y, arcSize, arcSize, 270.f, 90.f);
	gp.AddLine(x + width + arcSize, y + arcSize / 2, x + width + arcSize, y + headerHeight + arcSize / 2);

	g.FillPath(&t, &gp);

	bb.Width += arcSize;

	// bottom = rect
	g.FillRectangle(&b, a_X, a_Y + headerHeight, bb.Width, 10.f);

	g.DrawString(w.c_str(), w.size(), &font, origin, m_ActiveTab == a_Index ? &black : &white);

	// draw close button
	if(m_ActiveTab == a_Index || m_HoverTab == a_Index){
		SolidBrush closeBrush(Color(190, 40, 30));
		
		Pen hoverPen(Color::White, 2);
		Pen regularPen(Color(20, 20, 20), 2);

		Pen *p = &regularPen;

		float shrink = 4;

		RectF lines = close;
		lines.X += shrink;
		lines.Y += shrink;
		lines.Width -= shrink * 2;
		lines.Height -= shrink * 2;

		// just get mouse position here instead of passing it through
		// from wm_mousemove
		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(m_hWndTabs, &mouse);
		if(close.Contains(mouse.x, mouse.y)) {
			g.FillEllipse(&closeBrush, close);
			p = &hoverPen;
		}

		g.DrawLine(p, lines.X, lines.Y, lines.X + lines.Width, lines.Y + lines.Height);
		g.DrawLine(p, lines.X + lines.Width, lines.Y, lines.X, lines.Y + lines.Height);
	}

	m_VisibleTabs[a_Index].m_BoundingBox = bb;
	m_VisibleTabs[a_Index].m_CloseBox    = close;

	return bb.Width;
}

void tabs_impl_win32::wm_paint_tabs(HWND hWnd){
	PAINTSTRUCT l_Paint;
	HDC hDC = BeginPaint(hWnd, &l_Paint);
	
	HDC hDCCompat = CreateCompatibleDC(hDC);
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
    HBITMAP hbmOldBitmap = (HBITMAP)SelectObject(hDCCompat, hbmScreen);

	Graphics g(hDCCompat);

	g.SetSmoothingMode(SmoothingModeHighQuality);

	size l_Size = get_size();

	// render background
	l_Paint.fErase = FALSE;
	SolidBrush b(Color(255, 44, 61, 91));
	g.FillRectangle(&b, 0, 0, l_Size.m_Width, l_Size.m_Height);
	
	
	unsigned int l_X = 0;
	m_VisibleTabs.clear();
	for(unsigned int i = 0; i < count(); i++){
		l_X += draw_tab(g, l_X, 2, i);
	}

	SolidBrush yellow(Color(255, 255, 232, 166));
	g.FillRectangle(&yellow, 0, g_TabHeight - 3, l_Size.m_Width, 3);

	BitBlt(hDC, l_Paint.rcPaint.left, l_Paint.rcPaint.top, l_Paint.rcPaint.right, l_Paint.rcPaint.bottom, hDCCompat, 0, 0, SRCCOPY);

	SelectObject(hDCCompat, hbmOldBitmap);
	DeleteObject(hbmScreen);
	DeleteDC(hDCCompat);

	EndPaint(hWnd, &l_Paint);
}

LRESULT tabs_impl_win32::process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(hWnd == m_hWndTabs){
		if(uMsg == WM_PAINT){
			wm_paint_tabs(hWnd);
			return 0;
		}else if(uMsg == WM_MOUSELEAVE){
			m_HoverTab = (unsigned int)-1;
			InvalidateRect(m_hWndTabs, NULL, TRUE);
			return 0;
		}else if(uMsg == WM_MOUSEMOVE){
			widget_helper_win32::track_mouseleave(m_hWndTabs);

			unsigned int oldHover = m_HoverTab;
			m_HoverTab = (unsigned int)-1;
			for(visible_tabs_t::iterator i = m_VisibleTabs.begin(); i != m_VisibleTabs.end(); i++){
				bool inside = i->second.m_BoundingBox.Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				if(inside){
					m_HoverTab = i->first;
					::InvalidateRect(m_hWndTabs, NULL, TRUE);
					break;
				}
			}

			return 0;
		}else if(uMsg == WM_LBUTTONDOWN){
			unsigned int oldActive = m_ActiveTab;

			for(visible_tabs_t::iterator i = m_VisibleTabs.begin(); i != m_VisibleTabs.end(); i++){
				bool inside = i->second.m_BoundingBox.Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				bool close = i->second.m_CloseBox.Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				if(close){
					remove_child(i->first);
					::InvalidateRect(m_hWndTabs, NULL, TRUE);
					::InvalidateRgn(m_hWnd, NULL, TRUE);

					// remove_child invalidates m_VisibleTabs iterator
					break;
				}else{
					get_child(i->first).set_visible(inside);

					if(inside){
						m_ActiveTab = i->first;
					}
				}
			}

			if(m_ActiveTab != oldActive){
				::InvalidateRect(m_hWnd, NULL, TRUE);
				::InvalidateRect(m_hWndTabs, NULL, TRUE);
			}

			return 0;
		}else if(uMsg == WM_MOUSELEAVE){
			m_HoverTab = (unsigned)-1;
			::InvalidateRect(m_hWndTabs, NULL, TRUE);
		}
	}

	return widget_win32::process_message(hWnd, uMsg, wParam, lParam);
}