#include "stdafx.h"
#include "screen_captrue.h"
#include "ip_manager.h"
#include "manager.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	/*
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	// TODO: Place code here.

	ScreenCapture screen_capture_wnd;
	screen_capture_wnd.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	screen_capture_wnd.CenterWindow();
	screen_capture_wnd.ShowModal();

	delete screen_capture_wnd;
	//Initialize global strings
	*/

	
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin_1"));	

	Manager manager;
	manager.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	manager.CenterWindow();
	manager.ShowModal();
	/**/

	return 0;
}
