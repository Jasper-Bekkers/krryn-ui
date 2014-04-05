#include "label.h"
#include "widget_factory.h"

using namespace gui;

label::label(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void label::set_text(const std::string &a_Text){
	m_Pimpl->set_text(a_Text);
}

void *label::handle(){
	return m_Pimpl->handle();
}

label_initializer::label_initializer() : m_Text("label") {
	width(75);
	height(23);
}

label_initializer &label_initializer::text(const std::string &a_Text) { m_Text = a_Text; return *this; }

label* label_initializer::create(){
	return new label(*this);
}