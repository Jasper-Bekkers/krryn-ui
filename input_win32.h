#ifdef _WIN32
#pragma once
#include "input.h"
#define _WIN32_WINDOWS 0x501
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <map>

namespace gui_imp{
	class input_manager_win32 : public gui::input_manager{
	public:
		input_manager_win32();
		LRESULT CALLBACK wnd_proc_input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		void post_keyup_message(gui::keys_e a_Key);
		void post_keydown_message(gui::keys_e a_Key);
		void post_mouse_move(math::point a_Point);
		void post_mouse_up(gui::mouse_button a_Button);
		void post_mouse_down(gui::mouse_button a_Button);
		void post_mouse_scroll(gui::mouse_scroll a_Scroll, int a_Amount);

		void load_keys();

		HWND m_hWnd;
		std::map<int, gui::keys_e> m_KeyMap;
	};
}

#undef _WIN32_WINNT
#undef _WIN32_WINDOWS
#endif