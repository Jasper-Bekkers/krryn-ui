#pragma once
#include "event_loop_impl_base.h"

namespace gui_imp{
	class event_loop_impl_win32 : public gui::event_loop_impl_base{
	public:
		virtual void update();
	};
}
