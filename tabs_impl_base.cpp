#include "tabs_impl_base.h"
#include "tabs.h"
#include <string>

using namespace gui;

std::string tabs_impl_base::get_child_name(unsigned int a_Index){
	widget *l_Widget = &get_child(a_Index);
	tab_panel *l_Tab = dynamic_cast<tab_panel*>(l_Widget);

	if(l_Tab){
		return l_Tab->get_name();
	}

	return std::string("Tab ") + to_string(a_Index + 1);
}