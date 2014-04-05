#pragma once

namespace gui{
	class event_loop_impl_base;

	class event_loop{
	public:
		event_loop();
		void update();
	private:
		event_loop_impl_base *m_Pimpl;
	};
}