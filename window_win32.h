#ifdef _WIN32

#pragma once
#include "window_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class window_impl_win32 : public virtual widget_win32, public virtual gui::window_impl_base{
	public:
		virtual void make(const gui::window_initializer &a_Initializer);
	protected:
		virtual void on_detach();
	private:
		static int m_WindowCounter;
	};
}

#endif