#pragma once
#include <vector>
#include <algorithm>
#include "widget.h"

namespace gui{
	class widget_impl_base{
	public:
		widget_impl_base() : m_Widget(0), m_Parent(0) {}

		virtual void destroy() = 0;
		virtual void* handle() = 0;

		virtual void set_visible(bool a_IsVisible) = 0;
		virtual void set_size(int a_Width, int a_Height) = 0;
		virtual void set_pos(int a_X, int a_Y) = 0;
		virtual math::size get_size() = 0;
		virtual math::point get_pos() = 0;
		virtual bool get_visible() = 0;

		void post_make(widget &a_Child, widget *a_Parent) { m_Parent = a_Parent; if(a_Parent) a_Parent->add_child(a_Child); }

		virtual void add_child(widget &a_Widget) { m_Widgets.push_back(&a_Widget); }
		virtual void remove_child(size_t i){
			delete m_Widgets[i];
			m_Widgets.erase(m_Widgets.begin() + i);
		}

		// FIXME: Do bound-checking
		virtual widget& get_child(size_t i) { return *m_Widgets[i]; }
		virtual size_t count() const { return m_Widgets.size(); }

		void set_widget(widget *a_Widget) { m_Widget = a_Widget; }
		widget *get_widget() const { return m_Widget; }

		widget *get_parent() const { return m_Parent; }
	private:
		widget *m_Parent;
		widget *m_Widget;
		std::vector<widget*> m_Widgets;
	};
}