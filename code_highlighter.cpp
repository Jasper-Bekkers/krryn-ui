#include "code_highlighter.h"
#include "widget_factory.h"

using namespace gui;

code_highlighter::code_highlighter(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void code_highlighter::set_text(const std::string &a_Text){
	m_Pimpl->set_text(a_Text);
}

void *code_highlighter::handle(){
	return m_Pimpl->handle();
}

code_highlighter_initializer::code_highlighter_initializer() {
	width(75);
	height(23);
}

code_highlighter* code_highlighter_initializer::create(){
	return new code_highlighter(*this);
}