#pragma once
#include "widget.h"
#include "column_impl_base.h"
#include "window.h"

namespace gui{
	class column  : public widget{
	public:
		typedef column_impl_base impl;
		typedef column_initializer with;

		column(const with &a_Initialize);

		virtual void *handle();
		virtual ~column() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class column_initializer : public widget_initializer<column_initializer>{
	public:
		column_initializer();

		column* create();
	};
}
