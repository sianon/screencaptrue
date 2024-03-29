#pragma once

#include "Resource.h"
#include "base_window.h"
#include "vlc/vlc.h"
#include "audio_adjust_panel.h"
#include "msg_head.h"
#include "hook.h"
#include "ivga_engine.h"
#include "encrypt.h"
#include <memory>
#include <atlbase.h>
#include <vector>

class Manager : public BaseWindow
{
public:
	Manager();
	~Manager();

	BEGIN_DUIMSG_MAP(Manager)
		DUIMSG_HANDLER(kAM_Init, OnInitMsg)
		DUIMSG_HANDLER(kAM_ShowTaskMsg, OnTray)
		DUIMSG_HANDLER(kAM_ExitForPop, OnPopMsg)
		DUIMSG_HANDLER(kAM_MainForPop, OnPopMsg)
		DUIMSG_HANDLER(kAM_BeginForPop, OnPopMsg)
		DUIMSG_HANDLER(kAM_EndForPop, OnPopMsg)
		DUIMSG_HANDLER(FastKey, OnFastKey)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(Manager)
		DUINOTIFY_HANDLER(_T("minbtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("closebtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("begin_btn"), DUINOTIFY_CLICK, OnClickBeginBtn)
		DUINOTIFY_HANDLER(_T("end_btn"), DUINOTIFY_CLICK, OnClickEndBtn)
		DUINOTIFY_HANDLER(_T("companyweb"), DUINOTIFY_CLICK, OnOpenWeb)

		/* TabLayout 的切换响应 */
		DUINOTIFY_HANDLER(_T("options_opt"),DUINOTIFY_SELECTCHANGED, OnTabSelectChanged)
		DUINOTIFY_HANDLER(_T("serve_opt"), DUINOTIFY_SELECTCHANGED, OnTabSelectChanged)
		DUINOTIFY_HANDLER(_T("AVS_opt"), DUINOTIFY_SELECTCHANGED, OnTabSelectChanged)
		DUINOTIFY_HANDLER(_T("aboutme_opt"), DUINOTIFY_SELECTCHANGED, OnTabSelectChanged)

		/* options_opt_body 控件变化响应 */
		DUINOTIFY_HANDLER(_T("auto_start"), DUINOTIFY_SELECTCHANGED, OnTabOptionsChanged)
		DUINOTIFY_HANDLER(_T("min_start"), DUINOTIFY_SELECTCHANGED, OnTabOptionsChanged)
		DUINOTIFY_HANDLER(_T("live"), DUINOTIFY_SELECTCHANGED, OnTabOptionsChanged)
		DUINOTIFY_HANDLER(_T("live_and_videos"), DUINOTIFY_SELECTCHANGED, OnTabOptionsChanged)

		/* serve_opt_body 控件变化响应 */
		DUINOTIFY_HANDLER(_T("serve_radio"), DUINOTIFY_SELECTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("push_radio"), DUINOTIFY_SELECTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("videos_check"), DUINOTIFY_SELECTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("live_address"), DUINOTIFY_ITEMSELECT, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("live_port_edit"), DUINOTIFY_TEXTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("live_dir_edit"), DUINOTIFY_TEXTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("push_address"), DUINOTIFY_TEXTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("push_port_edit"), DUINOTIFY_TEXTCHANGED, OnTabServeChanged)
		DUINOTIFY_HANDLER(_T("push_dir_edit"), DUINOTIFY_TEXTCHANGED, OnTabServeChanged)

		/* AVS_opt_body 控件变化响应 */
		DUINOTIFY_HANDLER(_T("frame_rate"), DUINOTIFY_ITEMSELECT, OnTabAVSChanged)
		DUINOTIFY_HANDLER(_T("quality"), DUINOTIFY_ITEMSELECT, OnTabAVSChanged)
	END_DUINOTIFY_MAP()

	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

public:
	virtual LRESULT OnInit() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) override;

private:
	void OptTabInit();
	void ServeTabInit();
	void AVSTabInit();
	bool HelpInit();
	void ControlEnable(bool benable);

private:
	LRESULT OnInitMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);
	LRESULT OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);
	LRESULT OnPopMsg(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);
	LRESULT OnFastKey(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);

private:
	void OnClickSysBtn(TNotifyUI &msg, bool &handled);
	void OnClickBeginBtn(TNotifyUI &msg, bool &handled);
	void OnClickEndBtn(TNotifyUI &msg, bool &handled);
	void OnTabSelectChanged(TNotifyUI &msg, bool &handled);
	void OnTabOptionsChanged(TNotifyUI &msg, bool &handled);
	void OnTabServeChanged(TNotifyUI &msg, bool &handled);
	void OnTabAVSChanged(TNotifyUI &msg, bool &handled);
	void OnAutoRun(TNotifyUI &msg, bool &handled);
	void OnOpenWeb(TNotifyUI &msg, bool &handled);

private:
	void ToTray();
	void SetAutoRun(bool bautorun);
	void GetLocalIPAddr(vector<wstring> & id_addr);
	void FillFPSAndQuality();
	void ReloadAddShow();

private:
	//libvlc_instance_t* vlc_;
	AudioAdjustPanel audio_panel_;
	IvgaEngine engine_;
	Encrypt encpyt_;
};