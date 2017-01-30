#pragma once
#include "widget.h"
#include "opengl_context_impl_base.h"
#include "window.h"

namespace gui{
	class opengl_context : public widget{
	public:
		typedef opengl_context_impl_base impl;
		typedef opengl_context_initializer with;

		opengl_context(const with &a_Initialize);

		void swap_buffers();

		virtual void *handle();
		virtual ~opengl_context() { m_Pimpl->destroy(); }
	private:
		impl *m_Pimpl;
	};

	class opengl_context_initializer : public widget_initializer<opengl_context_initializer>{
	public:
		opengl_context_initializer();

		opengl_context_initializer &fullscreen(bool a_Fullscreen);
		opengl_context_initializer &bits_per_pixel(int a_BitsPerPixel);

		bool get_fullscreen() const { return m_Fullscreen; }
		int get_bits_per_pixel() const { return m_BitsPerPixel; }

		opengl_context* create();
	private:
		bool m_Fullscreen = false;
		int m_BitsPerPixel = 32;
		std::string m_Text;
	};
}