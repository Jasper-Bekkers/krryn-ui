#ifdef _WIN32

#pragma once
#include "tabs_impl_base.h"
#include "widget_win32.h"
#include <windows.h>
#include <set>
#include <map>

namespace gui { class widget; }
namespace gui_imp{
	class tab_panel_impl_win32 : public virtual widget_win32, public virtual gui::tab_panel_impl_base{
	public:
		virtual void make(gui::tabs *a_Parent);
		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class tabs_impl_win32 : public virtual widget_win32, public virtual gui::tabs_impl_base{
	public:
		virtual void make(const gui::tabs_initializer &a_Initializer);
		virtual void set_visible(bool a_IsVisible);
		virtual void add_child(gui::widget &a_Widget);
		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void on_parent_resize(gui::resize_args *a_Args);
		void on_child_resize(gui::resize_args *a_Args);
		virtual void remove_child(size_t i);
	private:
		struct tab_info{
		public:
			Gdiplus::RectF m_BoundingBox;
			Gdiplus::RectF m_CloseBox;
		};

		void activate_last();
		unsigned int draw_tab(Gdiplus::Graphics &g, unsigned int a_X, unsigned int a_Y, unsigned int a_Index);
		void wm_paint_tabs(HWND hWnd);
		HWND m_hWndTabs;
		unsigned long long m_MostRecentlyUsedCounter;
		unsigned int m_ActiveTab;
		unsigned int m_HoverTab;

		typedef std::map<unsigned int, tab_info> visible_tabs_t;
		visible_tabs_t m_VisibleTabs;
	};
}

#endif