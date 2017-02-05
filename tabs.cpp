#include "tabs.h"
#include "widget_factory.h"

using namespace gui;

tab_panel::tab_panel(const std::string &a_Name, tabs *a_Parent) : widget(create_widget_base<impl>()), m_Name(a_Name){
	get_impl<impl>()->make(a_Parent);
	get_impl<impl>()->post_make(*this, a_Parent);
}

tabs::tabs(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

tabs::~tabs(){
	for(name_panels_t::iterator i = m_NamedPanels.begin(); i != m_NamedPanels.end(); i++){
		delete *i;
	}

	m_Pimpl->destroy();
}

tab_panel *tabs::named_panel(const std::string &a_Name){
	tab_panel *panel = new tab_panel(a_Name, this);
	m_NamedPanels.push_back(panel);
	return panel;
}

void *tabs::handle(){
	return m_Pimpl->handle();
}

tabs_initializer::tabs_initializer(){
	width(100);
	height(100);
}

tabs* tabs_initializer::create(){
	return new tabs(*this);
}