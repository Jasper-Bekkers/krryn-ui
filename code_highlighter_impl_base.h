#pragma once
#include "widget_impl_base.h"

namespace gui{
	class code_highlighter_initializer;

	class code_highlighter_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const code_highlighter_initializer &a_Initializer) = 0;
		virtual void set_text(const std::string &a_Text) = 0;
	};
}
