#pragma once

#include "Resource.h"
#include "base_window.h"
#include "vlc/vlc.h"
#include "audio_adjust_panel.h"
#include "msg_head.h"
#include <atlbase.h>
#include <vector>

class Manager : public BaseWindow
{
public:
	Manager();
	~Manager();

	BEGIN_DUIMSG_MAP(Manager)
		DUIMSG_HANDLER(kAM_ShowTaskMsg, OnTray)
		DUIMSG_HANDLER(kAM_ExitForPop, OnPopMsg)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(Manager)
		DUINOTIFY_HANDLER(_T("minbtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("closebtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("begin_btn"), DUINOTIFY_CLICK, OnClickBeginBtn)
		DUINOTIFY_HANDLER(_T("end_btn"), DUINOTIFY_CLICK, OnClickEndBtn)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_SELECTCHANGED, OnSelectChanged)
	END_DUINOTIFY_MAP()

	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

public:
	virtual LRESULT OnInit() override;

private:
	LRESULT OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);
	LRESULT OnPopMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);

private:
	void OnClickSysBtn(TNotifyUI &msg, bool &handled);
	void OnClickBeginBtn(TNotifyUI &msg, bool &handled);
	void OnClickEndBtn(TNotifyUI &msg, bool &handled);
	void OnSelectChanged(TNotifyUI &msg, bool &handled);

private:
	void ScreenServe();
	void ScreenPush();
	void OnExit();
	void Play();
	void ToTray();
	void SetAutoRun(bool bautorun);
	void GetLocalIPAddr(vector<wstring> & id_addr);

private:
	bool is_start_serve_;
	bool is_start_client_;
	INT screen_fps_;
	INT screen_fps_old_;
	CDuiString ip_push_;
	CDuiString screen_quality_;
	CDuiString screen_quality_old_;
	libvlc_instance_t* vlc_;
	const char* media_name_;
	CDuiString dir_name_;
	CDuiString port_;
	AudioAdjustPanel audio_panel_;
};