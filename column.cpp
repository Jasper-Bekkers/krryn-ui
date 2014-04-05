#include "column.h"
#include "widget_factory.h"

using namespace gui;

column::column(const with &a_Initializer)
: widget(create_widget_base<impl>()){

	m_Pimpl = get_impl<impl>();
	m_Pimpl->make(a_Initializer);
	m_Pimpl->post_make(*this, a_Initializer.get_parent());
}

void *column::handle(){
	return m_Pimpl->handle();
}

column_initializer::column_initializer(){
	width(100);
	height(100);
}

column* column_initializer::create(){
	return new column(*this);
}