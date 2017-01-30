#pragma once
#include "widget_impl_base.h"

namespace gui{
	class opengl_context_initializer;

	class opengl_context_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const opengl_context_initializer &a_Initializer) = 0;
		virtual void set_text(const std::string &a_Text) = 0;
	};
}
