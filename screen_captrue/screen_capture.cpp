#include "screen_captrue.h"

ScreenCapture::ScreenCapture()
	:screen_fps_(15)
	, screen_fps_old_(0)
	, screen_quality_(L"0.8")
	, screen_quality_old_(L"")
	, vlc_(nullptr)
	, media_name_("screen")
	, is_start_serve_(true)
	, is_start_client_(FALSE)
	, port_(L"554")
	, dir_name_(L"default")
{
	//xml_manager_->LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
}

ScreenCapture::~ScreenCapture()
{
}

//LRESULT ScreenCapture::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	if (uMsg == WM_KEYDOWN)
//	{
//		switch (wParam)
//		{
//		case VK_ESCAPE:
//			return ResponseDefaultKeyEvent(wParam);
//		default:
//			break;
//		}
//	}
//	return FALSE;
//}

void ScreenCapture::OnUserClick(const TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();

	if (sCtrlName == _T("ok")){
		OnClickOk();
		if (is_start_serve_){
			ScreenServe();
		}
		else {
			ScreenPush();
		}
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else if (sCtrlName == _T("radio_serve")){
		PDUI_RADIOBOX radiobox = static_cast<PDUI_RADIOBOX>(msg.pSender);
		radiobox->Selected(true);
		if (radiobox->IsSelected()){
			is_start_serve_ = true;
		}
	}
	else if (sCtrlName == _T("radio_client")){
		PDUI_RADIOBOX radiobox = static_cast<PDUI_RADIOBOX>(msg.pSender);
		radiobox->Selected(true);
		if (radiobox->IsSelected()){
			is_start_serve_ = false;
		}
	}
	else if (sCtrlName == _T("Cancel")) {
		SetAutoRun(false);
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}

BOOL ScreenCapture::OnSysClick(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if (sCtrlName == _T("closebtn"))
	{
		ToTray();
		return TRUE;
	}
	else if (sCtrlName == _T("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return TRUE;
	}
	else if (sCtrlName == _T("maxbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		return TRUE;
	}
	else if (sCtrlName == _T("restorebtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return TRUE;
	}
	return FALSE;
}

void ScreenCapture::ScreenServe()
{
	if (is_start_serve_){
		if (!vlc_) {
			Play();
		}
		else {
			if (!(screen_fps_old_ == screen_fps_) || !(_tcscmp(screen_quality_, screen_quality_old_) == 0)){
				OnExit();
				Play();
			}
		}
	}

	is_start_serve_ = true;
}
void ScreenCapture::ScreenPush()
{
	if (is_start_serve_){
		OnExit();
	}

	is_start_serve_ = false;

	const char *sout = "#transcode{vcodec=mp4v,vb=32}:duplicate{dst=rtp{access=rtsp,dst=18.18.8.5:554}}";
	const char* url = "Screen://";

	vlc_ = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, sout, 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);

}

void ScreenCapture::OnClickOk()
{
	PDUI_EDIT screen_fps = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("screen_fps_edit")));

	if (!screen_fps->GetText())
	{
		return;
	}
	else if (_ttoi(screen_fps->GetText()) > 0 && _ttoi(screen_fps->GetText()) <= 120)
	{
		screen_fps_old_ = screen_fps_;
		screen_fps_ = _ttoi(screen_fps->GetText());
	}

	PDUI_EDIT screen_quality = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("screen_quality")));

	if (!screen_quality->GetText()){
		return;
	}
	else if (_ttof(screen_quality->GetText()) > 0 && _ttof(screen_quality->GetText()) <= 10)
	{
		screen_quality_old_ = screen_quality_;
		screen_quality_ = screen_quality->GetText();
	}
}

void ScreenCapture::OnExit()
{
	libvlc_vlm_stop_media(vlc_, media_name_);
	libvlc_vlm_release(vlc_);
}

void ScreenCapture::Play()
{
	string screen_fps("--screen-fps=");
	char fps[3];
	_itoa_s(screen_fps_, fps, 3, 10);
	screen_fps.append(fps);

	const char * const argv[] = {
		screen_fps.c_str(),
		"--screen-follow-mouse",
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = screen_quality_;
	wstring third_part = L"}:duplicate{dst=rtp{sdp=rtsp://:";
	wstring port = port_;
	wstring last_part = dir_name_;
	wstring sout = first_part + scale + third_part + port + L"/" + last_part + L"}}";

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, CW2A(sout.c_str()), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);
}

void ScreenCapture::SetAutoRun(bool bautorun)
{
	HKEY hkey = nullptr;
	LPCTSTR str_reg_path = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	if (bautorun){
		if (RegOpenKeyEx(HKEY_CURRENT_USER, str_reg_path, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS){
			TCHAR szmodule[_MAX_PATH];
			GetModuleFileName(NULL, szmodule, _MAX_PATH);
			RegSetValueEx(hkey, L"screen_capture", 0, REG_SZ, (const BYTE*)(LPCTSTR)szmodule, lstrlen(szmodule) * 2);
			RegCloseKey(hkey);
		}
		else{
			MessageBox(NULL, L"设置自启动失败", L"error", 0);
		}
	}
	else{
		RegDeleteKeyValue(hkey, str_reg_path, L"screen_captrue");
	}
}

CDuiString ScreenCapture::GetSkinFolder()
{ 
	return _T("skin"); 
}
CDuiString ScreenCapture::GetSkinFile()
{ 
	return _T("screen_capture_wnd.xml");
}
LPCTSTR ScreenCapture::GetWindowClassName(void)const
{
	return _T("ScreenCapture");
}
void ScreenCapture::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")){
		if (!OnSysClick(msg))
			OnUserClick(msg);
	}else if (msg.sType == _T("selectshanged")){

	}
}

void ScreenCapture::ToTray()
{
	NOTIFYICONDATA wnd_to_tray;

	wnd_to_tray.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	wnd_to_tray.hWnd = this->m_hWnd;
	wnd_to_tray.uID = IDR_MAINFRAME;
	wnd_to_tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	wnd_to_tray.uCallbackMessage = WM_SHOWTASK_1;
	wnd_to_tray.hIcon = (HICON)LoadImage(NULL, L"screen_captrue.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

	wcscpy_s(wnd_to_tray.szTip, L"成都天狐威视IVGA");
	Shell_NotifyIcon(NIM_ADD, &wnd_to_tray);
	ShowWindow(SW_HIDE);
}

LRESULT ScreenCapture::OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled)
{
	if (wparam != IDR_MAINFRAME)
		return 1;
	switch (lparam)
	{
	case WM_RBUTTONUP:{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);
		//CMenu menu;
		//menu.CreatePopupMenu();
		//menu.AppendMenu(MF_STRING, WM_DESTROY, "退出");
		//menu.AppendMenu(MF_STRING, WM_SHOWWINDOW, "打开面板");
		//menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);

		//HMENU hmenu = menu.Detach();
		//menu.DestroyMenu();
		//delete lpoint;
	}
		break;
	case WM_LBUTTONDBLCLK:
	{
		this->ShowWindow(true);
	}
		break;
	}
	return 0;
}