#include "event_loop_win32.h"
#include <windows.h>

using namespace gui_imp;

void event_loop_impl_win32::update(){
	MSG l_Message;
	//if (PeekMessage(&l_Message, NULL, 0, 0, PM_REMOVE)){
	if(GetMessage(&l_Message, NULL, 0, 0)){
		if (l_Message.message == WM_QUIT){
			exit(0);
		}else{
			TranslateMessage(&l_Message);
			DispatchMessage(&l_Message);
		}
	}
}