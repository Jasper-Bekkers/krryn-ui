#include "widget.h"
#include "widget_impl_base.h"

using namespace gui;
using namespace math;

widget::widget(impl *a_Impl) : m_Pimpl(a_Impl) { m_Pimpl->set_widget(this); }

widget::~widget() {
	m_Pimpl->destroy();
}

void widget::add_child(widget& a_Widget) {
	m_Pimpl->add_child(a_Widget);
}

widget& widget::get_child(size_t i) {
	return m_Pimpl->get_child(i);
}

size_t widget::count() const {
	return m_Pimpl->count();
}

void widget::set_visible(bool a_IsVisible){
	m_Pimpl->set_visible(a_IsVisible);
}

bool widget::get_visible(){
	return m_Pimpl->get_visible();
}

void widget::set_size(int a_Width, int a_Height){
	m_Pimpl->set_size(a_Width, a_Height);
}

void widget::set_pos(int a_X, int a_Y){
	m_Pimpl->set_pos(a_X, a_Y);
}

point widget::get_pos(){
	return m_Pimpl->get_pos();
}

size widget::get_size(){
	return m_Pimpl->get_size();
}

void *widget::handle() {
	return m_Pimpl->handle();
}