#include "widget_helper_win32.h"
#include "widget.h"
#include "widget_win32.h"
#include "input_win32.h"
#include <windows.h>
#include <iostream>

#include "gdiplus.h"

using namespace gui;
using namespace gui_imp;

// Enable visual styles
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

widget_win32 *widget_hook_win32::m_Widget = 0;
HHOOK widget_hook_win32::m_Hook = 0;

static struct gdiplus_init{
	gdiplus_init(){
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
	}

	~gdiplus_init(){
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	ULONG_PTR gdiplusToken;

} g_Init;

widget_hook_win32::widget_hook_win32(widget_win32 *a_Widget){
	if(m_Widget || m_Hook){
		throw std::runtime_error("Can't create window at this time (window hook still in progress)");
	}

	m_Widget = a_Widget;
	m_Hook = ::SetWindowsHookEx(
		WH_CBT, widget_hook_win32::proc, NULL, ::GetCurrentThreadId());

	if(!m_Hook){
		throw std::runtime_error("Can't hook window");
	}
}

widget_hook_win32::~widget_hook_win32(){
	//! FIXME: Sometimes proc() is called after the destructor

	//SetWindowLong((HWND)m_Widget->handle(), GWL_HWNDPARENT, (LONG)(HWND)m_Widget->handle());

	/*if(m_Hook){
		::UnhookWindowsHookEx(m_Hook);
	}

	m_Widget = 0;*/
}

LRESULT CALLBACK widget_hook_win32::proc(int code, WPARAM wParam, LPARAM lParam){
	if(code == HCBT_CREATEWND){
		HWND hWnd = (HWND)wParam;

		if(m_Widget){
			m_Widget->attach(hWnd);

			m_Widget->set_old_proc(
				(WNDPROC)(LONG_PTR)::SetWindowLongPtr(
				hWnd, GWL_WNDPROC, (LONG_PTR) widget_win32::proc));

			if(!::UnhookWindowsHookEx(m_Hook)){
				throw std::runtime_error("Can't unhook window");
			}else{
				m_Hook = 0;
				m_Widget = 0;
			}
		}

		return 0;
	}

	return ::CallNextHookEx(m_Hook, code, wParam, lParam);
}

HFONT widget_helper_win32::fix_font(HWND a_hWnd)
{
	LOGFONT lf;
	HFONT font;

	::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
	font = ::CreateFontIndirect(&lf);

	SendMessage(a_hWnd, WM_SETFONT, (WPARAM)font, TRUE);
	return font;
}

void widget_helper_win32::set_visible(HWND a_hWnd, bool a_IsVisible){
	DWORD style = GetWindowLong(a_hWnd, GWL_STYLE) & ~WS_VISIBLE;
	SetWindowLong(a_hWnd, GWL_STYLE, style | (a_IsVisible ? WS_VISIBLE : 0));
}

void widget_helper_win32::load_common_controls(){
	static bool l_Loaded = false;

	if(!l_Loaded)
	{
		INITCOMMONCONTROLSEX l_Ctrl;
		// simply load all of the common controls :-)
		l_Ctrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
		l_Ctrl.dwICC = ICC_ANIMATE_CLASS
					| ICC_BAR_CLASSES
					| ICC_COOL_CLASSES
					| ICC_DATE_CLASSES
					| ICC_HOTKEY_CLASS
					| ICC_INTERNET_CLASSES
					| ICC_LINK_CLASS
					| ICC_LISTVIEW_CLASSES
					| ICC_NATIVEFNTCTL_CLASS
					| ICC_PAGESCROLLER_CLASS
					| ICC_PROGRESS_CLASS
					| ICC_STANDARD_CLASSES
					| ICC_TAB_CLASSES
					| ICC_TREEVIEW_CLASSES
					| ICC_UPDOWN_CLASS
					| ICC_USEREX_CLASSES
					| ICC_WIN95_CLASSES;

		BOOL l_Success = InitCommonControlsEx(&l_Ctrl);
		
		if(!l_Success){
			std::clog << "Failed to load common controls" << std::endl;
		}

		l_Loaded = true;
	}
}

LRESULT CALLBACK default_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if(uMsg == WM_COMMAND){
		widget_win32 *l_Widget = widget_win32::get_from_hwnd((HWND)lParam);

		if(l_Widget){
			l_Widget->process_message(hWnd, uMsg, wParam, lParam);
		}
	}else if(uMsg == WM_SIZE){
		resize_args s;
		s.m_Width = LOWORD(lParam);
		s.m_Height = HIWORD(lParam);

		widget_win32 *l_Win32Widget = widget_win32::get_from_hwnd(hWnd);

		if(l_Win32Widget){
			widget *l_Widget = l_Win32Widget->get_widget();

			if(l_Widget){
				l_Widget->m_OnResize.trigger(&s);
			}
		}
	}else if(uMsg == WM_DESTROY){
		widget_win32 *l_Win32Widget = widget_win32::get_from_hwnd(hWnd);
		l_Win32Widget->detach(hWnd);
	}

	((input_manager_win32*)gui::input_manager::instance())->wnd_proc_input(hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

TCHAR * widget_helper_win32::window_class(){
	static bool m_IsRegistered = false;

	if(!m_IsRegistered){
		WNDCLASS l_WindowClass;
		memset(&l_WindowClass, 0, sizeof(l_WindowClass));

		l_WindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		l_WindowClass.lpfnWndProc	= (WNDPROC)default_window_proc;
		l_WindowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		l_WindowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		l_WindowClass.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
		l_WindowClass.lpszClassName	= TEXT("window_class");

		if(!RegisterClass(&l_WindowClass)){
			throw widget_exception("Can't register window class");
		}else{
			m_IsRegistered = true;
		}
	}

	return TEXT("window_class");
}
