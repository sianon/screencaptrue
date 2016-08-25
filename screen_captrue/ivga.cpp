#include "stdafx.h"
#include "manager.h"
#include "ivga_engine.h"
#include "hook.h"
#include "crypt_key_utils/CryptKeyUtils.h"
#pragma comment(lib, "CryptKeyUtils.lib")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));	
	//CPaintManagerUI::SetResourceZip(_T("default.x"));

	DuiLib::CDuiString default_x_path = CPaintManagerUI::GetResourcePath() + L"default.x";
	if (PathFileExists(default_x_path)) {
		FILE *file = nullptr;
		if (!_tfopen_s(&file, default_x_path, _T("rb"))) {
			BYTE buf[256 * 1024];
			unsigned len = fread(buf, sizeof(BYTE), 256 * 1024, file);
			len = Crypt("KINGHOO", buf, len, buf, 256 * 1024, FALSE);
			fclose(file);
			CPaintManagerUI::SetResourceZip(buf, len);
			CPaintManagerUI::SetForceUseLocalResourceFile(true, false, true);
		} else {
			return false;
		}
	}

	Manager manager;
	IVGAHook ivga_;

	manager.Create(NULL, _T("iVGA"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	ivga_.SetViewer(manager);
	ivga_.SetIvgaInterceptMsg(true);
	ivga_.SetIvgaKeyboardHook();
	manager.CenterWindow();
	manager.ShowModal();
	//manager.ShowWindow(SW_HIDE);
	/**/
	return 0;
}
