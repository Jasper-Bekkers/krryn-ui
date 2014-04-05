#pragma once
#include "widget.h"
#include "property_editor_impl_base.h"
#include "window.h"

namespace gui{
	class property_item{
	public:
		// manages PROPGRIDITEM, PROPGRIDFONTITEM & PROPGRIDFDITEM
		enum property_item_type{
			Text,
			Combobox,
			EditableCombobox,
			StaticText,
			Color,
			Font,
			File,
			Folder,
			Checkbox,
			IpAddress,
			Date,
			Time,
			DateAndTime,
			Header
		};

		void set_type(property_item_type a_Type);
		void set_category(const std::string &a_Category);
		void set_name(const std::string &a_Name);
		void set_desc(const std::string &a_Desc);

		// lots of overloads
		void set_value(const std::string &a_Value);
		void set_value(view::pixel_t a_Color); // etc

		void set_value(std::vector<std::string> &); // combo? lpszzCmbItems
		void set_value(const std::string &a_FilePath, const std::string &a_Filter, const std::string &a_DefaultExt);
	};

	class property_editor : public widget{
	public:
		typedef property_editor_impl_base impl;
		typedef property_editor_initializer with;

		property_editor(const with &a_Initialize);

		void clear();

		size_t add_item(const property_item &);
		void remove_item(size_t idx);

		// get_item / set_item?

		size_t get_item_count();

		size_t get_selected_item();
		void set_selected_item(size_t idx);

		virtual void *handle();
		virtual ~property_editor() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class property_editor_initializer : public widget_initializer<property_editor_initializer>{
	public:
		property_editor_initializer();

		property_editor* create();
	private:
		std::string m_Text;
	};
}
