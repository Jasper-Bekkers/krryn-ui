#include "event_loop.h"
#include "event_loop_impl_base.h"
#include "widget_factory.h"

using namespace gui;

event_loop::event_loop() : m_Pimpl(create_widget_base<event_loop_impl_base>()){}

void event_loop::update(){
	m_Pimpl->update();
}