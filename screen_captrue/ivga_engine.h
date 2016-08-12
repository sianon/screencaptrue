#ifndef __IVGA_ENGINE_H__
#define __IVGA_ENGINE_H__

#include "vlc/vlc.h"
#include "stdafx.h"
#include "hook.h"
#include <vector>
#include <atlbase.h>
#include <memory>

struct StreamInfo
{
	INT screen_fps_;
	CDuiString port_;
	CDuiString dir_name_;
	CDuiString ip_push_;
	CDuiString ip_server_;
	CDuiString screen_quality_;

	bool operator==(const StreamInfo stream){
		return(stream.screen_fps_ == screen_fps_
			&& stream.port_ == port_
			&& stream.dir_name_ == dir_name_
			&& stream.ip_push_ == ip_push_
			&& stream.ip_server_ == ip_server_
			&& stream.screen_quality_ == screen_quality_);
	}
};

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
	}
	bool UnKeyboardHook(){
		return keyboard_Hook_->UnIvgaKeyboardHook();
	}
	void SetInterceptMsg(bool intercept = true, bool keyboard_control = false, bool keyboard_enable_shortcut_key = false)
	{
		keyboard_Hook_->SetIvgaInterceptMsg(intercept, keyboard_control, keyboard_enable_shortcut_key);

	}

	void SetStreamInfo(StreamInfo stream_info);

	void StartServe();
	void StartClient();

	StreamInfo GetStreamInfo();
	bool GetServerState();
private:
	void OnDestory();
	void ProcessServer();
	void ProcessPush();

	bool is_start_serve_;
	const char* media_name_;
	StreamInfo stream_info_, stream_info_old_;
	libvlc_instance_t* vlc_;
	std::unique_ptr<IVGAHook> keyboard_Hook_;
};

#endif // !__IVGA_ENGINE_H__