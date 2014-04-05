#pragma once
#include "widget_impl_base.h"

namespace gui{
	class column_initializer;

	class column_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const column_initializer &a_Initializer) = 0;
	};
}
