#ifndef __SCREEN_CAPTURE_H__
#define __SCREEN_CAPTURE_H__

#include "Resource.h"
#include "ip_manager.h "
#include "base_window.h"
#include "ip_edit_ui.h"
#include "vlc/vlc.h"
#include "advanced_wnd.h"
#include "advanced_parent.h"
#include "xml_manager.h"
#include <atlconv.h>
#include <atlbase.h>

#define WM_SHOWTASK_1 (WM_USER + 1)

class ScreenCapture
	: public BaseWindow
{
public:
	ScreenCapture();
	~ScreenCapture();

	BEGIN_DUIMSG_MAP(ScreenCapture)
		DUIMSG_HANDLER(WM_SHOWTASK_1, OnTray)
	END_DUIMSG_MAP()

	void ScreenServe();
	void ScreenPush();

	void OnClickOk();
	void OnExit();
	void ToTray();

	void Play();
	void SetAutoRun(bool bautorun);
	void OnUserClick(const TNotifyUI& msg)override;
	BOOL OnSysClick(TNotifyUI& msg)override;
	LRESULT OnTray(UINT uMsg,WPARAM wparam, LPARAM lparam, BOOL& bHandled);
	//LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
protected:
	void Notify(TNotifyUI& msg) override;
	virtual CDuiString GetSkinFile() override;
	virtual CDuiString GetSkinFolder() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
private:
	bool is_start_serve_;
	bool is_start_client_;
	INT screen_fps_;
	INT screen_fps_old_;
	CDuiString screen_quality_;
	CDuiString screen_quality_old_;
	CDuiString dir_name_;
	CDuiString port_;
	libvlc_instance_t* vlc_;
	const char* media_name_;
	XmlManager* xml_manager_;
};

#endif	//__SCREEN_CAPTURE_H__