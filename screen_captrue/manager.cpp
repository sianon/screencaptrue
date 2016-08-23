#include "manager.h"
#include "res_singleton.h"
#include <map>
#include <WinSock2.h>
#include <IPHlpApi.h>
#include <sstream>

#pragma comment(lib,"Iphlpapi.lib")

Manager::Manager()
{
}

Manager::~Manager()
{
}

LRESULT Manager::OnInit()
{
	engine_.InitStreamInfo();

	OptTabInit();
	ServeTabInit();
	AVSTabInit();

	audio_panel_.CreateWithDefaultStyle(m_hWnd);
	PostMessage(kAM_Init, 0, 0);
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

	bool is_start_serve = engine_.GetIsServerState();
	static_cast<PDUI_RADIOBOX>(m_PaintManager.FindControl(_T("serve_radio")))->Selected(is_start_serve);
	static_cast<PDUI_RADIOBOX>(m_PaintManager.FindControl(_T("push_radio")))->Selected(!is_start_serve);
	m_PaintManager.FindControl(_T("live_address"))->SetEnabled(is_start_serve);

	m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(is_start_serve);
	m_PaintManager.FindControl(_T("live_port_edit"))->SetText(engine_.GetPort());

	m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(is_start_serve);
	m_PaintManager.FindControl(_T("live_dir_edit"))->SetText(engine_.GetDir());

	m_PaintManager.FindControl(_T("push_address"))->SetEnabled(!is_start_serve);
	m_PaintManager.FindControl(_T("push_address"))->SetText(engine_.GetIpaddr(true));

	m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(!is_start_serve);
	m_PaintManager.FindControl(_T("push_port_edit"))->SetText(engine_.GetPort(true));

	m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(!is_start_serve);
	m_PaintManager.FindControl(_T("push_dir_edit"))->SetText(engine_.GetDir(true));
}

void Manager::AVSTabInit()
{
	FillFPSAndQuality();
}

LRESULT Manager::OnInitMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	ToTray();
	this->ShowWindow(SW_HIDE);
	return LRESULT();
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
	else if (uMsg == kAM_MainForPop)
		this->ShowWindow(true);
	else if (uMsg == kAM_BeginForPop) {
		engine_.StartServe();
		m_PaintManager.FindControl(_T("auto_start"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("min_start"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live_and_videos"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("begin_btn"))->SetEnabled(false);
	}
	else if (uMsg == kAM_EndForPop) {
		engine_.OnDestory();
		m_PaintManager.FindControl(_T("auto_start"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("min_start"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live_and_videos"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("begin_btn"))->SetEnabled(true);
	}

	return LRESULT();
}

LRESULT Manager::OnFastKey(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled)
{
	switch (wparam)
	{
	case 0x5a:
	{
		engine_.StartServe();
	}
	break;
	case 0x58:
	{
		engine_.OnDestory();
	}
	default:
		break;
	}

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
	engine_.StartServe();
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	m_PaintManager.FindControl(_T("auto_start"))->SetEnabled(false);
	m_PaintManager.FindControl(_T("min_start"))->SetEnabled(false);
	m_PaintManager.FindControl(_T("live"))->SetEnabled(false);
	m_PaintManager.FindControl(_T("live_and_videos"))->SetEnabled(false);
	m_PaintManager.FindControl(_T("begin_btn"))->SetEnabled(false);
}

void Manager::OnClickEndBtn(TNotifyUI & msg, bool & handled)
{
	engine_.OnDestory();
	m_PaintManager.FindControl(_T("auto_start"))->SetEnabled(true);
	m_PaintManager.FindControl(_T("min_start"))->SetEnabled(true);
	m_PaintManager.FindControl(_T("live"))->SetEnabled(true);
	m_PaintManager.FindControl(_T("live_and_videos"))->SetEnabled(true);
	m_PaintManager.FindControl(_T("begin_btn"))->SetEnabled(true);
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
		engine_.SetIsStartServe(true);
		m_PaintManager.FindControl(_T("live_address"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_address"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(false);
		FillFPSAndQuality();
	} else if (name == _T("push_radio")) {
		engine_.SetIsStartServe(false);
		m_PaintManager.FindControl(_T("live_address"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live_port_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("live_dir_edit"))->SetEnabled(false);
		m_PaintManager.FindControl(_T("push_address"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_port_edit"))->SetEnabled(true);
		m_PaintManager.FindControl(_T("push_dir_edit"))->SetEnabled(true);
		FillFPSAndQuality();
	} else if (name == _T("live_address")) {
		engine_.SetIpaddr(msg.pSender->GetText());
	} else if (name == _T("live_port_edit")) {
		engine_.SetPort(msg.pSender->GetText());
	} else if (name == _T("live_dir_edit")) {
		engine_.SetDir(msg.pSender->GetText());
	} else if (name == _T("push_address")) {
		engine_.SetIpaddr(msg.pSender->GetText(), true);
	} else if (name == _T("push_port_edit")) {
		engine_.SetPort(msg.pSender->GetText(), true);
	} else if (name == _T("push_dir_edit")) {
		engine_.SetDir(msg.pSender->GetText(), true);
	}
}

void Manager::OnTabAVSChanged(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("frame_rate")) {
		engine_.SetFPS(_ttoi(msg.pSender->GetText()), !engine_.GetIsServerState());
	} else if (name == _T("quality")) {
		engine_.SetQuality(msg.pSender->GetText(), !engine_.GetIsServerState());
	}
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

void Manager::FillFPSAndQuality()
{
	int index = 0;
	switch (engine_.GetFPS(!engine_.GetIsServerState()))
	{
		case 5: index = 0; break;
		case 10:index = 1; break;
		case 15:index = 2; break;
		case 20:index = 3; break;
		case 25:index = 4; break;
		case 30:index = 5; break;
		default:index = 4; break;
	}
	static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("frame_rate")))->SelectItem(index);

	switch (_ttoi(engine_.GetQuality(!engine_.GetIsServerState())))
	{
		case 1: index = 0; break;
		case 2: index = 1; break;
		case 3: index = 2; break;
		case 4: index = 3; break;
		case 5: index = 4; break;
		case 6: index = 5; break;
		case 7: index = 6; break;
		case 8: index = 7; break;
		case 9: index = 8; break;
		case 10: index = 9; break;
		default: index = 7; break;
	}
	static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("quality")))->SelectItem(index);
}

LRESULT Manager::OnClose(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled)
{
	::DestroyWindow(m_hWnd);
	return 0;
}
