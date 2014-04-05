#pragma once
#include "widget_impl_base.h"

namespace gui{
	class window_initializer;
	
	class window_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const window_initializer &a_Initializer) = 0;
	};
}
