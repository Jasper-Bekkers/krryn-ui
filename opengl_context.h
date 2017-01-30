#pragma once
#include "widget.h"
#include "opengl_context_impl_base.h"
#include "window.h"

namespace gui{
	class opengl_context : public widget{
	public:
		typedef opengl_context_impl_base impl;
		typedef opengl_context_initializer with;

		opengl_context(const with &a_Initialize);

		void set_text(const std::string &a_Text);

		virtual void *handle();
		virtual ~opengl_context() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class opengl_context_initializer : public widget_initializer<opengl_context_initializer>{
	public:
		opengl_context_initializer();

		opengl_context_initializer &text(const std::string &a_Text);

		std::string get_text() const { return m_Text; }

		opengl_context* create();
	private:
		std::string m_Text;
	};
}