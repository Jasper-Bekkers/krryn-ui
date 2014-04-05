#pragma once
#include "widget_impl_base.h"

namespace gui{
	class button_initializer;

	class button_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const button_initializer &a_Initializer) = 0;
	};
}
