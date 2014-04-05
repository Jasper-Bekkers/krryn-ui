#ifdef _WIN32

#pragma once
#include "label_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class label_impl_win32 : public virtual widget_win32, public virtual gui::label_impl_base{
	public:
		virtual void make(const gui::label_initializer &a_Initializer);
		virtual void set_text(const std::string &a_Text);
	};
}

#endif