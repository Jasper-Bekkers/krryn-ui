#pragma once
#include <string>
#include "widget.h"
#include "window_impl_base.h"

namespace gui{
	class window : public widget{
	public:
		typedef window_impl_base impl;
		typedef window_initializer with;

		window(const with &a_Initializer);

		virtual void *handle();
		virtual ~window() { m_Pimpl->destroy(); }

	private:
		impl *m_Pimpl;
	};

	class window_initializer
		: public widget_initializer<window_initializer>{
	public:
		window_initializer();

		window_initializer &title(const std::string &a_Title);

		std::string get_title() const;

		window* create();

	private:
		std::string m_Title;
	};
}
