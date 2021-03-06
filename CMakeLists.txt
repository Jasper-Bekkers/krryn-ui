cmake_minimum_required(VERSION 3.1)

project(krryn)

add_definitions(/W4 /WX)

# 'class1' : inherits 'class2::member' via dominance
add_definitions(/wd4250)

# 'unreferenced formal parameter'
add_definitions(/wd4100) 

# nonstandard extension used: nameless struct/union
add_definitions(/wd4201)

include_directories(
	"scintilla/include"
	"propertyGrid"
	)


add_subdirectory("krryn-ui-test")
add_subdirectory("propertygrid")

add_library(krryn-ui
	animation_diagram.h
	button.cpp
	button.h
	button_impl_base.h
	button_win32.cpp
	button_win32.h
	code_highlighter.cpp
	code_highlighter.h
	code_highlighter_impl_base.h
	code_highlighter_win32.cpp
	code_highlighter_win32.h
	column.cpp
	column.h
	column_impl_base.h
	column_win32.cpp
	column_win32.h
	diagram.cpp
	diagram.h
	diagram_impl_base.h
	diagram_win32.cpp
	diagram_win32.h
	event_loop.cpp
	event_loop.h
	event_loop_impl_base.h
	event_loop_win32.cpp
	event_loop_win32.h
	file_watcher.cpp
	file_watcher.h
	input.cpp
	input.h
	input_opengl.h
	input_win32.cpp
	input_win32.h
	keys.h
	label.cpp
	label.h
	label_impl_base.h
	label_win32.cpp
	label_win32.h
	lexical_cast.cpp
	lexical_cast.h
	opengl_context.h
	opengl_context_impl_base.h
	opengl_context.cpp
	opengl_context_win32.cpp
	opengl_context_win32.h
	property_editor.cpp
	property_editor.h
	property_editor_impl_base.h
	property_editor_win32.cpp
	property_editor_win32.h
	row.cpp
	row.h
	row_impl_base.h
	row_win32.cpp
	row_win32.h
	signal_slot.h
	string_util.cpp
	string_util.h
	tabs.cpp
	tabs.h
	tabs_impl_base.cpp
	tabs_impl_base.h
	tabs_win32.cpp
	tabs_win32.h
	textbox.cpp
	textbox.h
	textbox_impl_base.h
	textbox_win32.cpp
	textbox_win32.h
	widget.cpp
	widget.h
	widget_factory.h
	widget_factory_win32.cpp
	widget_helper_win32.cpp
	widget_helper_win32.h
	widget_impl_base.h
	widget_win32.cpp
	widget_win32.h
	window.cpp
	window.h
	window_impl_base.h
	window_win32.cpp
	window_win32.h
	)

target_link_libraries(krryn-ui propertyGrid)