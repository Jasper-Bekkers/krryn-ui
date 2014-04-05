#pragma once
#include "widget_impl_base.h"

namespace gui{
	class property_editor_initializer;
	class property_item;

	class property_editor_impl_base : public virtual widget_impl_base{
	public:
		virtual void make(const property_editor_initializer &a_Initializer) = 0;
		
		virtual void clear() = 0;

		virtual size_t add_item(const property_item &) = 0;
		virtual void remove_item(size_t idx) = 0;

		virtual size_t get_item_count() = 0;

		virtual size_t get_selected_item() = 0;
		virtual void set_selected_item(size_t idx) = 0;
	};
}
