#pragma once
#include "widget.h"
#include "button_impl_base.h"
#include "window.h"

namespace gui{
	class button : public widget{
	public:
		typedef button_impl_base impl;
		typedef button_initializer with;

		button(const with &a_Initialize);

		virtual void *handle();
		virtual ~button() { m_Pimpl->destroy(); }

		signal<event_args*> m_OnClick;
	private:
		impl *m_Pimpl;
	};

	class button_initializer : public widget_initializer<button_initializer>{
	public:
		button_initializer();

		button_initializer &text(const std::string &a_Text);

		std::string get_text() const { return m_Text; }

		button* create();
	private:
		std::string m_Text;
	};
}
