#pragma once
#include "widget.h"
#include "code_highlighter_impl_base.h"

namespace gui{
	class code_highlighter_impl_base;
	class code_highlighter : public widget{
	public:
		typedef code_highlighter_impl_base impl;
		typedef code_highlighter_initializer with;

		code_highlighter(const with &a_Initializer);

		void set_text(const std::string &a_Text);

		void *handle();
	private:
		impl *m_Pimpl;
	};

	class code_highlighter_initializer : public widget_initializer<code_highlighter_initializer>{
	public:
		code_highlighter_initializer();

		code_highlighter* create();
	};
}
