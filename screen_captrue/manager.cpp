#include "manager.h"
#include <map>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

Manager::Manager()
	: is_start_serve_(true)
	, is_start_client_(false)
	, screen_fps_(15)
	, screen_fps_old_(0)
	, screen_quality_(L"0.8")
	, screen_quality_old_(L"")
	, vlc_(nullptr)
	, media_name_("screen")
	, dir_name_(L"default")
	, port_(L"554")
{
}

Manager::~Manager()
{
}

LRESULT Manager::OnInit()
{
	vector<wstring> ip_addrs;
	GetLocalIPAddr(ip_addrs);
	PDUI_COMBO live_addr_combo = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("live_address")));
	PDUI_LISTLABELELEM elemen;
	for (auto iter : ip_addrs) {
		elemen = new CListLabelElementUI;
		elemen->SetText(iter.c_str());
		live_addr_combo->Add(elemen);
	}

	audio_panel_.CreateWithDefaultStyle(m_hWnd);
	
	return 0;
}

LRESULT Manager::OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	if (wparam != IDR_MAINFRAME)
		return 1;
	switch (lparam)
	{
		case WM_RBUTTONUP: {
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);
			audio_panel_.PopupWindow(lpoint);
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			this->ShowWindow(SW_SHOW);
		}
		break;
	}
	return LRESULT();
}

LRESULT Manager::OnPopMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	if (uMsg == kAM_ExitForPop)
		Close();

	return LRESULT();
}

void Manager::OnClickSysBtn(TNotifyUI & msg, bool & handled)
{
	if (msg.pSender->GetName() == _T("closebtn")) {
		ToTray();
		return;
	}

	OnSysClick(msg);
}

void Manager::OnClickBeginBtn(TNotifyUI & msg, bool & handled)
{
	PDUI_COMBO frame_rate = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("frame_rate")));

	if (!frame_rate)
		return;
	screen_fps_old_ = screen_fps_;
	screen_fps_ = _ttoi(frame_rate->GetText());

	PDUI_COMBO screen_quality = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("quality")));

	if (!screen_quality->GetText())
		return;
	screen_quality_old_ = screen_quality_;
	CDuiString temp = screen_quality->GetText();
	screen_quality_ = _T("0.7");

	if (is_start_serve_) {
		ScreenServe();
	} else {
		ScreenPush();
	}
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void Manager::OnClickEndBtn(TNotifyUI & msg, bool & handled)
{
}

void Manager::OnSelectChanged(TNotifyUI & msg, bool & handled)
{
	PDUI_TABLAYOUT tlayout = static_cast<PDUI_TABLAYOUT>(m_PaintManager.FindControl(_T("main_opt_tab")));
	if (msg.pSender->GetName() == _T("options_opt")) {
		tlayout->SelectItem(0);
	} else if (msg.pSender->GetName() == _T("serve_opt")) {
		tlayout->SelectItem(1);
	} else if (msg.pSender->GetName() == _T("AVS_opt")) {
		tlayout->SelectItem(2);
	} else if (msg.pSender->GetName() == _T("aboutme_opt")) {
		tlayout->SelectItem(3);
	}
}

void Manager::ScreenServe()
{
	if (is_start_serve_) {
		if (!vlc_) {
			Play();
		} else {
			if (!(screen_fps_old_ == screen_fps_) || !(_tcscmp(screen_quality_, screen_quality_old_) == 0)) {
				OnExit();
				Play();
			}
		}
	}

	is_start_serve_ = true;
}

void Manager::ScreenPush()
{
	if (is_start_serve_) {
		OnExit();
	}

	is_start_serve_ = false;

	const char *sout = "#transcode{vcodec=mp4v,vb=32}:duplicate{dst=rtp{access=rtsp,dst=18.18.8.5:554}}";
	const char* url = "Screen://";

	vlc_ = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, sout, 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);
}

void Manager::OnExit()
{
	libvlc_vlm_stop_media(vlc_, media_name_);
	libvlc_vlm_release(vlc_);
}

void Manager::Play()
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
	wstring scale = screen_quality_.GetData();
	wstring third_part = L"}:duplicate{dst=rtp{sdp=rtsp://:";
	wstring port = port_.GetData();
	wstring last_part = dir_name_.GetData();
	wstring sout = first_part + scale + third_part + port + L"/" + last_part + L"}}";
	//wstring sout = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2, scale=0.8}:duplicate{dst=rtp{sdp=rtsp://:554/screen}}";

	wstring::size_type scint = sout.find(_T("/screen"));
	wstring::size_type xc = sout.find(L"scale");

	//wstring screen_quality = screen_quality_;
	//sout.erase(xc + 6, 3);
	//sout.insert(xc + 6, screen_quality_);

	std::string str_quality(sout.length(), L'');
	std::copy(sout.begin(), sout.end(), str_quality.begin());

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, str_quality.c_str(), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);
}

void Manager::ToTray()
{
	NOTIFYICONDATA wnd_to_tray;

	wnd_to_tray.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	wnd_to_tray.hWnd = this->m_hWnd;
	wnd_to_tray.uID = IDR_MAINFRAME;
	wnd_to_tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	wnd_to_tray.uCallbackMessage = kAM_ShowTaskMsg;
	wnd_to_tray.hIcon = (HICON)LoadImage(NULL, L"screen_captrue.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

	wcscpy_s(wnd_to_tray.szTip, L"成都天狐威视IVGA");
	Shell_NotifyIcon(NIM_ADD, &wnd_to_tray);
	ShowWindow(SW_HIDE);
}

void Manager::SetAutoRun(bool bautorun)
{
	HKEY hkey = nullptr;
	LPCTSTR str_reg_path = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	if (bautorun) {
		if (RegOpenKeyEx(HKEY_CURRENT_USER, str_reg_path, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) {
			TCHAR szmodule[_MAX_PATH];
			GetModuleFileName(NULL, szmodule, _MAX_PATH);
			RegSetValueEx(hkey, L"screen_capture", 0, REG_SZ, (const BYTE*)(LPCTSTR)szmodule, lstrlen(szmodule) * 2);
			RegCloseKey(hkey);
		} else {
			MessageBox(NULL, L"设置自启动失败", L"error", 0);
		}
	} else {
		RegDeleteKeyValue(hkey, str_reg_path, L"screen_captrue");
	}
}

void Manager::GetLocalIPAddr(vector<wstring> & ip_addr)
{
	
}
