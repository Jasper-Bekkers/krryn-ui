#ifdef _WIN32
#include "input_win32.h"
#include <windows.h>
#include <cctype>

using namespace gui_imp;
using namespace gui;

namespace{
	static input_manager_win32 g_Register;
}

input_manager_win32::input_manager_win32(){
	load_keys();

	register_instance(this);
}

LRESULT CALLBACK input_manager_win32::wnd_proc_input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				//if((wParam >= 'A' && wParam <= 'Z') || (wParam >= '0' && wParam <= '9')){
					if(uMsg == WM_KEYUP){
						post_keyup_message(m_KeyMap[std::tolower((int)wParam)]);
					}else{
						post_keydown_message(m_KeyMap[std::tolower((int)wParam)]);
					}
				//}
			}
			break;
		case WM_MOUSEMOVE:
			{
				math::point l_Point(LOWORD(lParam), HIWORD(lParam));
				post_mouse_move(l_Point);
			}
			break;
		case WM_LBUTTONUP:
			post_mouse_up(btn_left);
			break;
		case WM_MBUTTONUP:
			post_mouse_up(btn_middle);
			break;
		case WM_RBUTTONUP:
			post_mouse_up(btn_right);
			break;
		case WM_LBUTTONDOWN:
			post_mouse_down(btn_left);
			break;
		case WM_MBUTTONDOWN:
			post_mouse_down(btn_middle);
			break;
		case WM_RBUTTONDOWN:
			post_mouse_down(btn_right);
			break;
		case WM_MOUSEWHEEL:
			post_mouse_scroll(
				GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? scroll_up : scroll_down, 
				(int)math::abs(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA);
			break;
	}

	return 0;
}

void input_manager_win32::post_keyup_message(gui::keys_e a_Key){
	for(keyboard_list_t::iterator i = m_KeyboardListeners.begin(); i != m_KeyboardListeners.end(); i++){
		(*i)->key_up(a_Key);
	}
}

void input_manager_win32::post_keydown_message(gui::keys_e a_Key){
	for(keyboard_list_t::iterator i = m_KeyboardListeners.begin(); i != m_KeyboardListeners.end(); i++){
		(*i)->key_down(a_Key);
	}
}

void input_manager_win32::post_mouse_move(math::point a_Point){
	for(mouse_list_t::iterator i = m_MouseListeners.begin(); i != m_MouseListeners.end(); i++){
		(*i)->mouse_move_abs(a_Point);
	}
}

void input_manager_win32::post_mouse_up(mouse_button a_Button){
	for(mouse_list_t::iterator i = m_MouseListeners.begin(); i != m_MouseListeners.end(); i++){
		(*i)->button_up(a_Button);
	}
}

void input_manager_win32::post_mouse_down(mouse_button a_Button){
	for(mouse_list_t::iterator i = m_MouseListeners.begin(); i != m_MouseListeners.end(); i++){
		(*i)->button_down(a_Button);
	}
}

void input_manager_win32::post_mouse_scroll(mouse_scroll a_Scroll, int a_Amount){
	for(mouse_list_t::iterator i = m_MouseListeners.begin(); i != m_MouseListeners.end(); i++){
		for(int t = 0; t < a_Amount; t++){
			(*i)->scroll_wheel(a_Scroll);
		}
	}
}

void input_manager_win32::load_keys(){
	m_KeyMap[0x30] = key_0;
	m_KeyMap[0x31] = key_1;
	m_KeyMap[0x32] = key_2;
	m_KeyMap[0x33] = key_3;
	m_KeyMap[0x34] = key_4;
	m_KeyMap[0x35] = key_5;
	m_KeyMap[0x36] = key_6;
	m_KeyMap[0x37] = key_7;
	m_KeyMap[0x38] = key_8;
	m_KeyMap[0x39] = key_9;
	m_KeyMap[0x41] = key_a;
	m_KeyMap[0x42] = key_b;
	m_KeyMap[VK_OEM_102] = key_backslash;
	m_KeyMap[VK_BACK] = key_backspace;
	m_KeyMap[0x43] = key_c;
	m_KeyMap[VK_CAPITAL] = key_caps_lock;
	m_KeyMap[VK_OEM_COMMA] = key_comma;
	m_KeyMap[0x44] = key_d;
	m_KeyMap[VK_DELETE] = key_delete;
	m_KeyMap[VK_DOWN] = key_down;
	m_KeyMap[0x45] = key_e;
	m_KeyMap[VK_END] = key_end;
	//m_KeyMap[] = key_equals;
	m_KeyMap[VK_ESCAPE] = key_escape;
	m_KeyMap[0x46] = key_f;
	m_KeyMap[VK_F1] = key_f1;
	m_KeyMap[VK_F10] = key_f10;
	m_KeyMap[VK_F11] = key_f11;
	m_KeyMap[VK_F12] = key_f12;
	m_KeyMap[VK_F2] = key_f2;
	m_KeyMap[VK_F3] = key_f3;
	m_KeyMap[VK_F4] = key_f4;
	m_KeyMap[VK_F5] = key_f5;
	m_KeyMap[VK_F6] = key_f6;
	m_KeyMap[VK_F7] = key_f7;
	m_KeyMap[VK_F8] = key_f8;
	m_KeyMap[VK_F9] = key_f9;
	m_KeyMap[0x47] = key_g;
	m_KeyMap[VK_OEM_3] = key_grave;
	m_KeyMap[0x48] = key_h;
	m_KeyMap[VK_HOME] = key_home;
	m_KeyMap[0x49] = key_i;
	m_KeyMap[VK_INSERT] = key_insert;
	m_KeyMap[0x4A] = key_j;
	m_KeyMap[0x4B] = key_k;
	m_KeyMap[0x4C] = key_l;
	m_KeyMap[VK_LEFT] = key_left;
	m_KeyMap[VK_LMENU] = key_left_alt;
	//m_KeyMap[] = key_left_bracket;
	m_KeyMap[VK_LCONTROL] = key_left_control;
	m_KeyMap[VK_LSHIFT] = key_left_shift;
	m_KeyMap[0x4D] = key_m;
	m_KeyMap[VK_OEM_MINUS] = key_minus;
	m_KeyMap[0x4E] = key_n;
	m_KeyMap[VK_NUMPAD0] = key_num_0;
	m_KeyMap[VK_NUMPAD1] = key_num_1;
	m_KeyMap[VK_NUMPAD2] = key_num_2;
	m_KeyMap[VK_NUMPAD3] = key_num_3;
	m_KeyMap[VK_NUMPAD4] = key_num_4;
	m_KeyMap[VK_NUMPAD5] = key_num_5;
	m_KeyMap[VK_NUMPAD6] = key_num_6;
	m_KeyMap[VK_NUMPAD7] = key_num_7;
	m_KeyMap[VK_NUMPAD8] = key_num_8;
	m_KeyMap[VK_NUMPAD9] = key_num_9;
	m_KeyMap[VK_ADD] = key_num_add;
	m_KeyMap[VK_DECIMAL] = key_num_decimal;
	m_KeyMap[VK_DIVIDE] = key_num_divide;
	m_KeyMap[VK_RETURN] = key_num_enter;
	m_KeyMap[VK_MULTIPLY] = key_num_multiply;
	m_KeyMap[VK_SUBTRACT] = key_num_subtract;
	m_KeyMap[VK_NUMLOCK] = key_num_lock;
	m_KeyMap[0x4F] = key_o;
	m_KeyMap[0x50] = key_p;
	m_KeyMap[VK_NEXT] = key_page_down;
	m_KeyMap[VK_PRIOR] = key_page_up;
	m_KeyMap[VK_PAUSE] = key_pause;
	m_KeyMap[VK_OEM_PERIOD] = key_period;
	m_KeyMap[VK_SNAPSHOT] = key_print_screen;
	m_KeyMap[0x51] = key_q;
	m_KeyMap[0x52] = key_r;
	m_KeyMap[VK_RETURN] = key_return;
	m_KeyMap[VK_RIGHT] = key_right;
	m_KeyMap[VK_RMENU] = key_right_alt;
	//m_KeyMap[] = key_right_bracket;
	m_KeyMap[VK_RCONTROL] = key_right_control;
	m_KeyMap[VK_RSHIFT] = key_right_shift;
	m_KeyMap[0x53] = key_s;
	m_KeyMap[VK_SCROLL] = key_scroll_lock;
	m_KeyMap[VK_OEM_1] = key_semicolon;
	//m_KeyMap[] = key_slash;
	m_KeyMap[VK_SLEEP] = key_sleep;
	m_KeyMap[VK_SPACE] = key_space;
	m_KeyMap[0x54] = key_t;
	m_KeyMap[VK_TAB] = key_tab;
	m_KeyMap[0x55] = key_u;
	m_KeyMap[VK_UP] = key_up;
	m_KeyMap[0x56] = key_v;
	m_KeyMap[0x57] = key_w;
	m_KeyMap[0x58] = key_x;
	m_KeyMap[0x59] = key_y;
	m_KeyMap[0x5A] = key_z;
}

#endif