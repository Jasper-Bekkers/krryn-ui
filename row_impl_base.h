#pragma once
#include "widget_impl_base.h"

namespace gui{
	class row_initializer;

	class row_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const row_initializer &a_Initializer) = 0;
	};
}
