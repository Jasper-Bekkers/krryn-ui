#pragma once
#include "widget.h"
#include "row_impl_base.h"
#include "window.h"

namespace gui{
	class row  : public widget{
	public:
		typedef row_impl_base impl;
		typedef row_initializer with;

		row(const with &a_Initialize);

		virtual void *handle();
		virtual ~row() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class row_initializer : public widget_initializer<row_initializer>{
	public:
		row_initializer();
		row* create();
	private:
		widget* m_Parent;
	};
}