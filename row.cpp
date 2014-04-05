#include "row.h"
#include "widget_factory.h"

using namespace gui;

row::row(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void *row::handle(){
	return m_Pimpl->handle();
}

row_initializer::row_initializer(){
	width(100);
	height(100);
}

row* row_initializer::create(){
	return new row(*this);
}