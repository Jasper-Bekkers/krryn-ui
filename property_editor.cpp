#include "property_editor.h"
#include "widget_factory.h"

using namespace gui;

property_editor::property_editor(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void *property_editor::handle(){
	return m_Pimpl->handle();
}

property_editor_initializer::property_editor_initializer(){
	width(400);
	height(600);
}

property_editor* property_editor_initializer::create(){
	return new property_editor(*this);
}