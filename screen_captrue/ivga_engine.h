#ifndef __IVGA_ENGINE_H__
#define __IVGA_ENGINE_H__

#include "vlc/vlc.h"
#include "stdafx.h"
#include "hook.h"
#include "syscfg.h"
#include <vector>
#include <atlbase.h>
#include <memory>

class IvgaEngine
{
public:
	IvgaEngine(); 
	~IvgaEngine();

	//¹³×ÓÉèÖÃ
	void SetViewer(HWND hwnd)
	{
		keyboard_Hook_->SetViewer(hwnd);
	}
	bool SetKeyBoardHook()
	{
		keyboard_Hook_->SetIvgaKeyboardHook();
		return true;
	}
	bool UnKeyboardHook(){
		return keyboard_Hook_->UnIvgaKeyboardHook();
	}
	void SetInterceptMsg(bool intercept = true, bool keyboard_control = false, bool keyboard_enable_shortcut_key = false)
	{
		keyboard_Hook_->SetIvgaInterceptMsg(intercept, keyboard_control, keyboard_enable_shortcut_key);

	}

	void InitStreamInfo();
	void SetStreamInfo(StreamInfo stream_info);

	void StartServe();
	void StartClient();
	void OnDestory();

	StreamInfo GetStreamInfo();

public:
	void SetIsStartServe(bool start);
	bool GetIsServerState();

	void SetFPS(INT value, bool is_push = false);
	INT GetFPS(bool is_push = false);

	void SetQuality(LPCTSTR value, bool is_push = false);
	LPCTSTR GetQuality(bool is_push = false);

	void SetIpaddr(LPCTSTR value, bool is_push = false);
	LPCTSTR GetIpaddr(bool is_push = false);

	void SetPort(LPCTSTR value, bool is_push = false);
	LPCTSTR GetPort(bool is_push = false);

	void SetDir(LPCTSTR value, bool is_push = false);
	LPCTSTR GetDir(bool is_push = false);

	std::unique_ptr<IVGAHook> keyboard_Hook_;
private:
	void ProcessServer();
	void ProcessPush();
	CDuiString ChangeQuality(LPCTSTR value);

	const char* media_name_;
	StreamInfo stream_info_, stream_info_old_;
	libvlc_instance_t* vlc_;
	Syscfg cfg_;
};

#endif // !__IVGA_ENGINE_H__