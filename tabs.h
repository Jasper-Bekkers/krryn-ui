#pragma once
#include "widget.h"
#include "tabs_impl_base.h"

namespace gui{
	class tabs;
	class tab_panel : public widget{
	public:
		typedef tab_panel_impl_base impl;
		tab_panel(const std::string &a_Name, tabs *a_Parent);
		std::string get_name() const { return m_Name; }
		virtual ~tab_panel() { get_impl<impl>()->destroy(); }
	private:
		std::string m_Name;
	};

	class tabs : public widget{
	public:
		typedef tabs_impl_base impl;
		typedef tabs_initializer with;

		tabs(const with &a_Initialize);

		tab_panel *named_panel(const std::string &a_Name);

		virtual void *handle();
		virtual ~tabs();
	private:
		typedef std::vector<tab_panel *> name_panels_t;
		name_panels_t m_NamedPanels;
		impl *m_Pimpl;
	};

	class tabs_initializer : public widget_initializer<tabs_initializer>{
	public:
		tabs_initializer();

		tabs* create();
	private:
		std::string m_Text;
	};
}