#pragma once

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

namespace gui_imp{ class widget_win32; }
namespace gui{
	// Normally we don't get the messages of windows
	// that are actually widgets (eg EDIT or BUTTON et cetera)
	// so we subclass them; allowing us to receive their messages
	class widget_hook_win32{
	public:
		widget_hook_win32(gui_imp::widget_win32 *a_Widget);
		~widget_hook_win32();
		static LRESULT CALLBACK proc(INT code, WPARAM wParam, LPARAM lParam);
	private:
		//! Warning, this is NOT threadsafe
		static gui_imp::widget_win32 *m_Widget;
		static HHOOK m_Hook;
	};

	class widget_helper_win32{
	public:
		static TCHAR *window_class();

		static void load_common_controls();
		static HFONT fix_font(HWND a_hWnd);

		static void set_visible(HWND a_hWnd, bool a_IsVisible);

		template<typename T>
		static RECT get_window_rect(const T &a_Initializer){
			RECT l_WindowRect;
			l_WindowRect.left	= (LONG)a_Initializer.get_left();
			l_WindowRect.right	= (LONG)a_Initializer.get_width() + a_Initializer.get_left();
			l_WindowRect.top	= (LONG)a_Initializer.get_top();
			l_WindowRect.bottom	= (LONG)a_Initializer.get_height() + a_Initializer.get_top();
			return l_WindowRect;
		}

		static void track_mouseleave(HWND hWnd){
			TRACKMOUSEEVENT evt;
			
			evt.cbSize = sizeof(TRACKMOUSEEVENT);
			evt.dwFlags = TME_LEAVE;
			evt.hwndTrack = hWnd;

			_TrackMouseEvent(&evt);
		}


		//template<typename T>
		//static HWND create_control(const std::string &a_ClassName, const std::string &a_WindowName, DWORD a_Style, T &a_Initializer){
		//	HWND hWnd = CreateWindow(
		//		a_ClassName.c_str(), a_WindowName.c_str(),
		//		a_Style, 
		//		a_Initializer.get_left(), a_Initializer.get_top(), 
		//		a_Initializer.get_width(), a_Initializer.get_height(),
		//		NULL, NULL, NULL, NULL);
		//}

		template<typename T>
		static DWORD visible(T &a_Initializer){
			return a_Initializer.get_visible() ? WS_VISIBLE : 0;
		}

		template<typename T>
		static void create_window(T &a_Initializer, HWND &a_hWnd, HDC &a_hDC){
			DWORD l_ExtraStyle = 0;
			DWORD l_Style = 0;
			HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

			RECT l_WindowRect = get_window_rect(a_Initializer);

			if(a_Initializer.get_fullscreen()){
				DEVMODE l_dmScreenSettings;
				memset(&l_dmScreenSettings, 0, sizeof(l_dmScreenSettings));
				l_dmScreenSettings.dmSize = sizeof(l_dmScreenSettings);
				l_dmScreenSettings.dmPelsWidth = a_Initializer.get_width();
				l_dmScreenSettings.dmPelsHeight = a_Initializer.get_height();
				l_dmScreenSettings.dmBitsPerPel	= a_Initializer.get_bits_per_pixel();
				l_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				auto res = ChangeDisplaySettings(&l_dmScreenSettings, CDS_FULLSCREEN);
				if(res != DISP_CHANGE_SUCCESSFUL){
					throw widget_exception("Can't change to fullscreen mode");
				}
			}else{
				l_Style = visible(a_Initializer) | WS_CHILD;
			}

			AdjustWindowRectEx(&l_WindowRect, l_Style, FALSE, l_ExtraStyle);
			
			a_hWnd = CreateWindowEx(
				l_ExtraStyle, widget_helper_win32::window_class(),
				"software_context",
				l_Style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				l_WindowRect.left, l_WindowRect.top,
				l_WindowRect.right - l_WindowRect.left,
				l_WindowRect.bottom - l_WindowRect.top,
				l_ParentWnd, NULL, NULL, NULL);

			if (!a_hWnd){
				throw widget_exception("Failed to create window");
			}

			if (a_Initializer.get_fullscreen()) {
				// set window always on top and 
				// hides it's border / title bar / cursor et cetera
				SetWindowPos(
					a_hWnd, HWND_TOPMOST,
					0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
				SetWindowLong(a_hWnd, GWL_STYLE, visible(a_Initializer) | WS_POPUP);
				SetWindowLong(a_hWnd, GWL_EXSTYLE, NULL);
				ShowCursor(FALSE);

				l_Style = visible(a_Initializer) | WS_CHILD;
			}

			a_hDC = GetDC(a_hWnd);
			if(!a_hDC){
				throw widget_exception("Can't create opengl context");
			}
		}
	};
}

#undef _WIN32_WINNT