#include "stdafx.h"
#include "manager.h"
#include "ivga_engine.h"
#include "hook.h"
#include "encrypt.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));	

	Manager manager;
	IVGAHook ivga_;

	manager.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	ivga_.SetViewer(manager);
	ivga_.SetIvgaInterceptMsg(true);
	ivga_.SetIvgaKeyboardHook();
	manager.CenterWindow();
	manager.ShowModal();
	//manager.ShowWindow(SW_HIDE);
	/**/

	return 0;
}
