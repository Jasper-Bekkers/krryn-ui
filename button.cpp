#include "button.h"
#include "widget_factory.h"

using namespace gui;

button::button(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void *button::handle(){
	return m_Pimpl->handle();
}

button_initializer::button_initializer() : m_Text("button") {
	width(75);
	height(23);
}

button_initializer &button_initializer::text(const std::string &a_Text) { m_Text = a_Text; return *this; }

button* button_initializer::create(){
	return new button(*this);
}