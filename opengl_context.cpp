#include "opengl_context.h"
#include "widget_factory.h"

using namespace gui;

opengl_context::opengl_context(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void opengl_context::set_text(const std::string &a_Text){
	m_Pimpl->set_text(a_Text);
}

void *opengl_context::handle(){
	return m_Pimpl->handle();
}

opengl_context_initializer::opengl_context_initializer() : m_Text("opengl_context") {
	width(75);
	height(23);
}

opengl_context_initializer &opengl_context_initializer::text(const std::string &a_Text) { m_Text = a_Text; return *this; }

opengl_context* opengl_context_initializer::create(){
	return new opengl_context(*this);
}