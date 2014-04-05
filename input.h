#pragma once
#include "keys.h"
#include "widget.h" // TODO: REMOVE
#include <list>

namespace gui{
	enum mouse_button{
		btn_left,
		btn_middle,
		btn_right
	};

	enum mouse_scroll{
		scroll_up,
		scroll_down,
		scroll_none
	};

	class keyboard_listener{
	public:
		virtual ~keyboard_listener() {}

		virtual void key_down(keys_e a_Key) {}
		virtual void key_up(keys_e a_Key) {}
	};

	class mouse_listener{
	public:
		virtual ~mouse_listener() {}

		virtual void button_down(mouse_button a_Button) {}
		virtual void button_up(mouse_button a_Button) {}
		virtual void scroll_wheel(mouse_scroll a_Scroll) {}
		virtual void mouse_move_abs(math::point a_Position) {}
	};

	class input_manager;

	class keyboard{
	public:
		keyboard();
		~keyboard();

		bool key_pressed(keys_e a_Key);
	private:
		class keyboard_imp;
		keyboard_imp *m_Pimpl;
	};

	class mouse{
	public:
		mouse();
		~mouse();

		bool button_pressed(mouse_button a_Button);
		mouse_scroll scroll_wheel();
		math::point mouse_move_abs();
	private:
		class mouse_imp;
		mouse_imp *m_Pimpl;
	};

	class input_manager{
	public:
		virtual ~input_manager() {}

		void attach_mouse(mouse_listener *a_Mouse);
		void attach_keyboard(keyboard_listener *a_Mouse);

		mouse* create_mouse_device();
		keyboard* create_keyboard_device();

		static input_manager *instance();

	protected:
		typedef std::list<mouse_listener *> mouse_list_t;
		typedef std::list<keyboard_listener *> keyboard_list_t;

		mouse_list_t m_MouseListeners;
		keyboard_list_t m_KeyboardListeners;

		void register_instance(input_manager *a_Manager);
	private:
		static input_manager *m_Instance;
	};
}
