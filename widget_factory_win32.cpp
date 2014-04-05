#ifdef _WIN32

#include "widget_factory.h"
#include "window_win32.h"
#include "button_win32.h"
#include "column_win32.h"
#include "label_win32.h"
#include "row_win32.h"
#include "tabs_win32.h"
#include "textbox_win32.h"
#include "event_loop_win32.h"
#include "code_highlighter_win32.h"
#include "diagram_win32.h"
#include "property_editor_win32.h"

namespace gui{
	IMPL_WIDGET(window_impl, win32);
	IMPL_WIDGET(event_loop_impl, win32);
	IMPL_WIDGET(tab_panel_impl, win32);
	IMPL_WIDGET(button_impl, win32);
	IMPL_WIDGET(diagram_impl, win32);
	IMPL_WIDGET(column_impl, win32);
	IMPL_WIDGET(label_impl, win32);
	IMPL_WIDGET(row_impl, win32);
	IMPL_WIDGET(tabs_impl, win32);
	IMPL_WIDGET(textbox_impl, win32);
	IMPL_WIDGET(code_highlighter_impl, win32);
	IMPL_WIDGET(property_editor_impl, win32);
}

#endif
