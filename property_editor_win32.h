#ifdef _WIN32

#pragma once
#include "property_editor_impl_base.h"
#include "widget_win32.h"
#include <windows.h>

namespace gui_imp{
	class property_editor_impl_win32 : public virtual widget_win32, public virtual gui::property_editor_impl_base{
	public:
		virtual void make(const gui::property_editor_initializer &a_Initializer);

		virtual void clear();

		virtual size_t add_item(const gui::property_item &);
		virtual void remove_item(size_t idx);

		virtual size_t get_item_count();

		virtual size_t get_selected_item();
		virtual void set_selected_item(size_t idx);
	};
}

#endif