#pragma once
#include "window_impl_base.h"
#include "button_impl_base.h"
#include "column_impl_base.h"
#include "row_impl_base.h"
#include "tabs_impl_base.h"
#include "label_impl_base.h"
#include "textbox_impl_base.h"
#include "event_loop_impl_base.h"
#include "code_highlighter_impl_base.h"
#include "diagram_impl_base.h"
#include "property_editor_impl_base.h"

#define TPL_PROTOTYPE(type) \
	template<> \
	type##_base *create_widget_base<type##_base>();

#define IMPL_WIDGET(type, api) \
	template<> \
	type##_base *create_widget_base<type##_base>(){	\
		return new gui_imp::type##_##api(); \
	} 

namespace gui{
	template<typename _Type>
	_Type *create_widget_base(){
		throw std::exception("Not implemented");
	}

	TPL_PROTOTYPE(window_impl);
	TPL_PROTOTYPE(button_impl);
	TPL_PROTOTYPE(diagram_impl);
	TPL_PROTOTYPE(tab_panel_impl);
	TPL_PROTOTYPE(column_impl);
	TPL_PROTOTYPE(row_impl);
	TPL_PROTOTYPE(tabs_impl);
	TPL_PROTOTYPE(label_impl);
	TPL_PROTOTYPE(textbox_impl);
	TPL_PROTOTYPE(event_loop_impl);
	TPL_PROTOTYPE(code_highlighter_impl);
	TPL_PROTOTYPE(property_editor_impl);
}
