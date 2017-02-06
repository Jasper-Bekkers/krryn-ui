#include "opengl_context.h"
#include "opengl_context_win32.h"
#include "widget_helper_win32.h"

#include <gl/gl.h>

using namespace gui_imp;
using namespace gui;

#pragma comment(lib, "OpenGL32.lib")

void opengl_context_impl_win32::make(const opengl_context_initializer &a_Initializer){
	GLuint l_PixelFormat = 0;
	widget_helper_win32::load_common_controls();

	{
		widget_hook_win32(this);

		widget_helper_win32::create_window(a_Initializer, m_hWnd, m_hDC);
	}

	PIXELFORMATDESCRIPTOR l_Pfd = { sizeof(PIXELFORMATDESCRIPTOR) };

	l_Pfd.nVersion = 1;
	l_Pfd.dwFlags = PFD_DRAW_TO_WINDOW + PFD_SUPPORT_OPENGL + PFD_DOUBLEBUFFER;
	l_Pfd.iPixelType = PFD_TYPE_RGBA;
	l_Pfd.cColorBits = (BYTE)a_Initializer.get_bits_per_pixel();
	l_Pfd.cDepthBits = 16;
	l_Pfd.dwLayerMask = PFD_MAIN_PLANE;

	l_PixelFormat = ChoosePixelFormat(m_hDC, &l_Pfd);

	if (!l_PixelFormat) {
		throw widget_exception("No matching pixel format descriptor");
	}

	if (!SetPixelFormat(m_hDC, l_PixelFormat, &l_Pfd)) {
		throw widget_exception("Can't set the pixel format");
	}

	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) {
		throw widget_exception("Can't create rendering context");
	}

	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		wglMakeCurrent(NULL, NULL);
	}
}

void opengl_context_impl_win32::swap_buffers()
{
	SwapBuffers(m_hDC);
}
