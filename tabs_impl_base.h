#pragma once
#include "widget_impl_base.h"

namespace gui{
	class tabs;
	class tabs_initializer;

	class tab_panel_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(tabs *a_Parent) = 0;
	};

	class tabs_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const tabs_initializer &a_Initializer) = 0;
	protected:
		std::string get_child_name(unsigned int a_Index);
	};
}