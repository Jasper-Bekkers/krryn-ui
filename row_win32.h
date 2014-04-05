#ifdef _WIN32

#pragma once
#include "row_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

	namespace gui { class widget; }
	namespace gui_imp{
		class row_impl_win32 : public virtual widget_win32, public virtual gui::row_impl_base{
		public:
			virtual void make(const gui::row_initializer &a_Initializer);
			virtual void add_child(gui::widget &a_Widget);
			virtual LRESULT process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void on_child_resize(gui::resize_args *a_Args);
		private:
			int m_NextClientTop;
			int m_NextClientLeft;
			int m_MyWidth;
			int m_MyHeight;

			int m_MyLeft;
			int m_MyTop;
			int m_Padding;
		};
	}

#endif