#include "opengl_context.h"
#include "widget_factory.h"

using namespace gui;

opengl_context::opengl_context(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void opengl_context::swap_buffers(){
	m_Pimpl->swap_buffers();
}

void *opengl_context::handle(){
	return m_Pimpl->handle();
}

opengl_context_initializer::opengl_context_initializer() {
	width(75);
	height(23);
}
opengl_context_initializer &opengl_context_initializer::fullscreen(bool a_Fullscreen) { m_Fullscreen = a_Fullscreen; return *this; }
opengl_context_initializer &opengl_context_initializer::bits_per_pixel(int a_BitsPerPixel) { m_BitsPerPixel = a_BitsPerPixel; return *this; }

opengl_context* opengl_context_initializer::create(){
	return new opengl_context(*this);
}