#pragma once
#include "widget.h"
#include "label_impl_base.h"
#include "window.h"

namespace gui{
	class label : public widget{
	public:
		typedef label_impl_base impl;
		typedef label_initializer with;

		label(const with &a_Initialize);

		void set_text(const std::string &a_Text);

		virtual void *handle();
		virtual ~label() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class label_initializer : public widget_initializer<label_initializer>{
	public:
		label_initializer();

		label_initializer &text(const std::string &a_Text);

		std::string get_text() const { return m_Text; }

		label* create();
	private:
		std::string m_Text;
	};
}