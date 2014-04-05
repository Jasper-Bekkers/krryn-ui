#pragma once
#include "widget_impl_base.h"

namespace gui{
	class textbox_initializer;

	class textbox_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const textbox_initializer &a_Initializer) = 0;

		virtual void set_text(const std::string &a_Text) = 0;
		virtual std::string get_text() = 0;
	};
}
