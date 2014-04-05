#pragma once
#include "widget.h"
#include "textbox_impl_base.h"
#include "window.h"

namespace gui{
	class textbox : public widget{
	public:
		typedef textbox_impl_base impl;
		typedef textbox_initializer with;

		textbox(const with &a_Initialize);

		void set_text(const std::string &a_Text);
		std::string get_text();

		signal<event_args*> m_OnTextChange;

		virtual void *handle();
		virtual ~textbox() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class textbox_initializer : public widget_initializer<textbox_initializer>{
	public:
		textbox_initializer();
		textbox* create();
	private:
	};
}
