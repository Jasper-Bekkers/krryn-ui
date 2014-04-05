#pragma once
#include "widget_impl_base.h"

namespace gui{
	class label_initializer;

	class label_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const label_initializer &a_Initializer) = 0;
		virtual void set_text(const std::string &a_Text) = 0;
	};
}
