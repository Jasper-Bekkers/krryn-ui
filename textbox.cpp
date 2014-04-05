#include "textbox.h"
#include "widget_factory.h"

using namespace gui;

textbox::textbox(const with &a_Initializer)
: widget(create_widget_base<impl>()){
	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void textbox::set_text(const std::string &a_Text){
	m_Pimpl->set_text(a_Text);
}

std::string textbox::get_text(){
	return m_Pimpl->get_text();
}

void *textbox::handle(){
	return m_Pimpl->handle();
}

textbox_initializer::textbox_initializer() {
	width(175);
	height(23);
}

textbox* textbox_initializer::create(){
	return new textbox(*this);
}