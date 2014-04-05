#include "window.h"
#include "widget_factory.h"

using namespace gui;

window::window(const with &a_Initializer)
: widget(create_widget_base<impl>()) {
	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void *window::handle(){
	return m_Pimpl->handle();
}

window_initializer::window_initializer()
	: m_Title("window")
{
	width(640);
	height(480);

	top(100);
	left(100);
}

window_initializer &window_initializer::title(const std::string &a_Title){
	m_Title = a_Title;
	return *this;
}

std::string window_initializer::get_title() const{
	return m_Title;
}

window* window_initializer::create(){
	return new window(*this);
}