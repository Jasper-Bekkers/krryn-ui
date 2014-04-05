#include "input.h"
#include <map>
#include <algorithm>

using namespace gui;
using namespace math;

class keyboard::keyboard_imp : public keyboard_listener{
public:
	typedef std::map<keys_e, bool> keybuffer_t;

	keyboard_imp(){}

	virtual void key_down(keys_e a_Key){
		m_Pressed[a_Key] = true;
	}

	virtual void key_up(keys_e a_Key){
		m_Pressed.erase(a_Key);
	}

	bool is_pressed(keys_e a_Key){
		keybuffer_t::iterator l_Iterator = m_Pressed.find(a_Key);

		if(l_Iterator != m_Pressed.end()){
			return l_Iterator->second == true;
		}

		return false;
	}

	// std::map has O(log N) performance issues but the map 
	// should remain relatively small N < 10 meaning that
	// in this case it's still O(1) (log 10 = 1).
	keybuffer_t m_Pressed;
};

class mouse::mouse_imp : public mouse_listener{
public:
	mouse_imp(){
		memset(m_Pressed, 0, sizeof(bool) * 3);
		m_Scroll = scroll_none;
	}

	virtual void button_down(mouse_button a_Button){
		m_Pressed[a_Button] = true;
	}

	virtual void button_up(mouse_button a_Button){
		m_Pressed[a_Button] = false;
	}

	virtual void scroll_wheel(mouse_scroll a_Scroll){
		m_Scroll = a_Scroll;
	}

	virtual void mouse_move_abs(math::point a_Position){
		m_Abs = a_Position;
	}

	bool m_Pressed[3];
	mouse_scroll m_Scroll;
	point m_Abs;
	point m_Rel;
};

keyboard::keyboard(){
	m_Pimpl = new keyboard_imp();
	input_manager::instance()->attach_keyboard(m_Pimpl);
}

keyboard::~keyboard(){
	// TODO: remove from a_InputManager
	delete m_Pimpl; m_Pimpl = 0;
}

bool keyboard::key_pressed(keys_e a_Key){
	return m_Pimpl->is_pressed(a_Key);
}

mouse::mouse(){
	m_Pimpl = new mouse_imp();
	input_manager::instance()->attach_mouse(m_Pimpl);
}

mouse::~mouse(){
	// TODO: remove from a_InputManager
	delete m_Pimpl; m_Pimpl = 0;
}

bool mouse::button_pressed(mouse_button a_Button){
	return m_Pimpl->m_Pressed[a_Button];
}

point mouse::mouse_move_abs(){
	return m_Pimpl->m_Abs;
}

mouse_scroll mouse::scroll_wheel(){
	mouse_scroll l_Scroll = m_Pimpl->m_Scroll;
	m_Pimpl->m_Scroll = scroll_none;
	return l_Scroll;
}

void input_manager::attach_keyboard(keyboard_listener *a_Keyboard){
	m_KeyboardListeners.push_back(a_Keyboard);
}

void input_manager::attach_mouse(mouse_listener *a_Mouse){
	m_MouseListeners.push_back(a_Mouse);
}

keyboard *input_manager::create_keyboard_device(){
	return new keyboard();
}

mouse *input_manager::create_mouse_device(){
	return new mouse();
}

void input_manager::register_instance(input_manager *a_Manager){
	if(!m_Instance){
		m_Instance = a_Manager;
	}else{
		throw std::runtime_error("Already have an input manager attached");
	}
}

input_manager *input_manager::m_Instance = 0;

input_manager* input_manager::instance(){
	return m_Instance;
}