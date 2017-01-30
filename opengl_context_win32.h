#ifdef _WIN32

#pragma once
#include "opengl_context_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class opengl_context_impl_win32 : public virtual widget_win32, public virtual gui::opengl_context_impl_base{
	public:
		virtual void make(const gui::opengl_context_initializer &a_Initializer);
		virtual void set_text(const std::string &a_Text);
	};
}

#endif