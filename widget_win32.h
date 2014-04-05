#pragma once
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#undef _WIN32_WINNT
#include "widget_impl_base.h"

namespace gui_imp{
	class widget_win32 : public virtual gui::widget_impl_base{
	public:
		void attach(HWND hWnd);
		void detach(HWND hWnd);
		static widget_win32 *get_from_hwnd(HWND hWnd);
		void set_old_proc(WNDPROC a_Proc) { m_OldProc = a_Proc; }
		WNDPROC get_old_proc() { return m_OldProc; }
		static LRESULT proc(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void set_visible(bool a_IsVisible);
		virtual void set_size(int a_Width, int a_Height);
		virtual void set_pos(int a_X, int a_Y);
		virtual math::point get_pos();
		virtual math::size get_size();
		virtual bool get_visible();

		virtual void* handle(){return m_hWnd;}
		virtual void destroy() { ::DestroyWindow(m_hWnd); }

	protected:
		HWND m_hWnd;
		virtual void on_detach() {}
		virtual void on_attach() {}

	private:
		WNDPROC m_OldProc;
	};
}
