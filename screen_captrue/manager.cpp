#include "manager.h"
#include "res_singleton.h"
#include <map>
#include <WinSock2.h>
#include <IPHlpApi.h>
#include <sstream>

#pragma comment(lib,"Iphlpapi.lib")

Manager::Manager()
	: vlc_(nullptr) 
{
	stream_data_info_.is_start_serve = true;
	stream_data_info_.is_start_client = false;
	stream_data_info_.screen_fps = 25;
	stream_data_info_.screen_fps_old = 0;
	stream_data_info_.screen_quality = _T("0.8");
	stream_data_info_.screen_quality_old = _T("0");
	stream_data_info_.media_name = "screen";
	stream_data_info_.dir_name = _T("live123");
	stream_data_info_.port = _T("554");
	stream_data_info_.ip_server = _T("");
	stream_data_info_.ip_push = _T("10.18.3.61");
}

Manager::~Manager()
{
}

LRESULT Manager::OnInit()
{
	OptTabInit();
	ServeTabInit();
	AVSTabInit();

	audio_panel_.CreateWithDefaultStyle(m_hWnd);
	
	return 0;
}

void Manager::OptTabInit()
{
}

void Manager::ServeTabInit()
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

	static_cast<PDUI_RADIOBOX>(m_PaintManager.FindControl(_T("serve_radio")))->Selected(stream_data_info_.is_start_serve);
	static_cast<PDUI_RADIOBOX>(m_PaintManager.FindControl(_T("push_radio")))->Selected(stream_data_info_.is_start_client);
	m_PaintManager.FindControl(_T("live_address"))->SetEnabled(stream_data_info_.is_start_serve);

	m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(stream_data_info_.is_start_serve);
	m_PaintManager.FindControl(_T("live_port_edit"))->SetText(stream_data_info_.port);

	m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(stream_data_info_.is_start_serve);
	m_PaintManager.FindControl(_T("live_dir_edit"))->SetText(stream_data_info_.dir_name);

	m_PaintManager.FindControl(_T("push_address"))->SetEnabled(stream_data_info_.is_start_client);
	m_PaintManager.FindControl(_T("push_address"))->SetText(stream_data_info_.ip_push);

	m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(stream_data_info_.is_start_client);
	m_PaintManager.FindControl(_T("push_port_edit"))->SetText(stream_data_info_.port);

	m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(stream_data_info_.is_start_client);
	m_PaintManager.FindControl(_T("push_dir_edit"))->SetText(stream_data_info_.dir_name);
}

void Manager::AVSTabInit()
{
	static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("frame_rate")))->SelectItem(stream_data_info_.screen_fps / 5 - 1);
	static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("quality")))->SelectItem(7);
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
			this->ShowWindow(true);
		}
		break;
	}
	return LRESULT();
}

LRESULT Manager::OnPopMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	if (uMsg == kAM_ExitForPop)
		PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);

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

	if (stream_data_info_.is_start_serve) {
		ScreenServe();
	} else {
		ScreenPush();
	}
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void Manager::OnClickEndBtn(TNotifyUI & msg, bool & handled)
{
}

void Manager::OnTabSelectChanged(TNotifyUI & msg, bool & handled)
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

void Manager::OnTabOptionsChanged(TNotifyUI & msg, bool & handled)
{

}

void Manager::OnTabServeChanged(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("serve_radio")) {
		stream_data_info_.is_start_serve = true;
		stream_data_info_.is_start_client = false;
		m_PaintManager.FindControl(_T("live_address"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_address"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(false);
	} else if (name == _T("push_radio")) {
		stream_data_info_.is_start_serve = false;
		stream_data_info_.is_start_client = true;
		m_PaintManager.FindControl(_T("live_address"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_address"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(true);
	} else if (name == _T("live_address")) {
		stream_data_info_.ip_server = msg.pSender->GetText();
	} else if (name == _T("live_port_edit")) {
		stream_data_info_.port = msg.pSender->GetText();
	} else if (name == _T("live_dir_edit")) {
		stream_data_info_.dir_name = msg.pSender->GetText();
	} else if (name == _T("push_address")) {
		stream_data_info_.ip_push = msg.pSender->GetText();
	} else if (name == _T("push_port_edit")) {
		stream_data_info_.port = msg.pSender->GetText();
	} else if (name == _T("push_dir_edit")) {
		stream_data_info_.dir_name = msg.pSender->GetText();
	}
}

void Manager::OnTabAVSChanged(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("frame_rate")) {
		stream_data_info_.screen_fps_old = stream_data_info_.screen_fps;
		stream_data_info_.screen_fps = _ttoi(msg.pSender->GetText());
	} else if (name == _T("quality")) {
		stream_data_info_.screen_quality_old = stream_data_info_.screen_quality;
		char temp[20];
		double qua = _ttoi(msg.pSender->GetText()) / 10.0;
		sprintf_s(temp, "%.2lf", qua);
		CDuiString cds_temp;
		cds_temp = temp;
		stream_data_info_.screen_quality = cds_temp.GetData();
	}
}

void Manager::ScreenServe()
{
	if (stream_data_info_.is_start_serve) {
		if (!vlc_) {
			Play();
		} else {
			if (!(stream_data_info_.screen_fps_old == stream_data_info_.screen_fps) 
				|| !(_tcscmp(stream_data_info_.screen_quality, stream_data_info_.screen_quality_old) == 0)) {
				OnExit();
				Play();
			}
		}
	}

	stream_data_info_.is_start_serve = true;
}

void Manager::ScreenPush()
{	
	if (stream_data_info_.is_start_serve) {
		OnExit();
	}

	stream_data_info_.is_start_serve = false;

	string screen_fps("--screen-fps=");
	char fps[3];
	_itoa_s(stream_data_info_.screen_fps, fps, 3, 10);
	screen_fps.append(fps);

	const char * const argv[] = {
		screen_fps.c_str(),
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = stream_data_info_.screen_quality;
	wstring third_part = L"}:standard{access=udp, mux=ts, dst=";
	wstring ip_push = stream_data_info_.ip_push;
	wstring double_dot = L":";
	wstring port = stream_data_info_.port;
	wstring last_part = stream_data_info_.dir_name;
	wstring sout = first_part + scale + third_part + ip_push + double_dot + port + L"/" + last_part + L"}";

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, stream_data_info_.media_name, url, CW2A(sout.c_str()), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, stream_data_info_.media_name);
}

void Manager::OnExit()
{
	libvlc_vlm_stop_media(vlc_, stream_data_info_.media_name);
	libvlc_vlm_release(vlc_);
}

void Manager::Play()
{
	string screen_fps("--screen-fps=");
	char fps[3];
	_itoa_s(stream_data_info_.screen_fps, fps, 3, 10);
	screen_fps.append(fps);

	const char * const argv[] = {
		screen_fps.c_str(),
		"--screen-follow-mouse",
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = stream_data_info_.screen_quality;
	wstring third_part = L"}:duplicate{dst=rtp{sdp=rtsp://";
	wstring ip_server = stream_data_info_.ip_server;
	wstring double_dot = L":";
	wstring port = stream_data_info_.port;
	wstring last_part = stream_data_info_.dir_name;
	wstring sout = first_part + scale + third_part + ip_server + double_dot + port + L"/" + last_part + L"}}";

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, stream_data_info_.media_name, url, CW2A(sout.c_str()), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, stream_data_info_.media_name);
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
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);	// 这里，stSize 既是输入量也是输出量；
	if (ERROR_BUFFER_OVERFLOW == nRel) {	
		// ERROR_BUFFER_OVERFLOW 表示 传递给 GetAdaptersInfo 的内存空间不够，同时传出的 stSize 表示所需的空间大小
		delete pIpAdapterInfo;	// 释放掉，重新分配！
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);	// 利用传出的新空间大小值，重新填充 pIpAdapterInfo
	}
	std::wostringstream oss;
	if (ERROR_SUCCESS == nRel) {
		while (pIpAdapterInfo)
		{
			auto iter = &pIpAdapterInfo->IpAddressList;
			while (iter)
			{
				oss.str(L"");
				oss << iter->IpAddress.String;
				if (_tccmp(L"0.0.0.0", oss.str().c_str()) != 0)
					ip_addr.push_back(oss.str());
				iter = iter->Next;
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
}
