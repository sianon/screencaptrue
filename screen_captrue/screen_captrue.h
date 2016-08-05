#ifndef __SCREEN_CAPTURE_H__
#define __SCREEN_CAPTURE_H__
#include "ip_manager.h "
#include "base_window.h"
#include "ip_edit_ui.h"
#include "vlc/vlc.h"
#include "advanced_wnd.h"
#include "advanced_parent.h"
#include "xml_manager.h"



class ScreenCapture : public BaseWindow
{
public:

	ScreenCapture()
		:screen_fps_(15)
		, screen_fps_old_(0)
		, screen_quality_(L"0.8")
		, screen_quality_old_(L"")
		, vlc_(nullptr)
		, media_name_("screen")
		, is_start_serve_(true)
		, is_start_client_(FALSE)
	{
		//xml_manager_->LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
	}
	~ScreenCapture(){}

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				return ResponseDefaultKeyEvent(wParam);
			default:
				break;
			}
		}
		return FALSE;
	}

	void OnUserClick(const TNotifyUI& msg)override {
		CDuiString sCtrlName = msg.pSender->GetName();

		if (sCtrlName == _T("ok")){
			OnClickOk();
			if (is_start_serve_){
				ScreenServe();
			} else {
				ScreenPush();
			}
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (sCtrlName == _T("radio_serve")){
			PDUI_RADIOBOX radiobox = static_cast<PDUI_RADIOBOX>(msg.pSender);
			radiobox->Selected(true);
			if (radiobox->IsSelected()){
				is_start_serve_ = true;
			}
		} else if (sCtrlName == _T("radio_client")){
			PDUI_RADIOBOX radiobox = static_cast<PDUI_RADIOBOX>(msg.pSender);
			radiobox->Selected(true);
			if (radiobox->IsSelected()){
				is_start_serve_ = false;
			}
		} else if (sCtrlName == _T("Cancel")) {

			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
	}

	void ScreenServe(){
		if (is_start_serve_){
			if (!vlc_) {
				string screen_fps("--screen-fps=");
				char fps[3];
				_itoa_s(screen_fps_, fps, 3, 10);
				screen_fps.append(fps);

				const char * const argv[] = {
					screen_fps.c_str(),
					"--screen-follow-mouse",
				};

				const char* url = "Screen://";
				wstring sout = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2, scale=0.8}:duplicate{dst=rtp{sdp=rtsp://:554/screen}}";

				wstring::size_type xc = sout.find(L"scale");
				wstring screen_quality = screen_quality_;
				sout.erase(xc + 6, 3);
				sout.insert(xc + 6, screen_quality_);

				std::string str_quality(sout.length(), L'');
				std::copy(sout.begin(), sout.end(), str_quality.begin());

				vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
				libvlc_vlm_add_broadcast(vlc_, media_name_, url, str_quality.c_str(), 0, NULL, true, false);
				libvlc_vlm_play_media(vlc_, media_name_);
			} else {
				if (!(screen_fps_old_ == screen_fps_) || !(_tcscmp(screen_quality_, screen_quality_old_) == 0)){
					OnExit();
					string screen_fps("--screen-fps=");
					char fps[3];
					_itoa_s(screen_fps_, fps, 3, 10);
					screen_fps.append(fps);

					const char * const argv[] = {
						screen_fps.c_str(),
						"--screen-follow-mouse",
					};

					const char* url = "Screen://";
					wstring sout = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2, scale=0.8}:duplicate{dst=rtp{sdp=rtsp://:554/screen}}";

					wstring::size_type xc = sout.find(L"scale");
					wstring screen_quality = screen_quality_;
					sout.erase(xc + 6, 3);
					sout.insert(xc + 6, screen_quality_);

					std::string str_quality(sout.length(), L'');
					std::copy(sout.begin(), sout.end(), str_quality.begin());

					vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
					libvlc_vlm_add_broadcast(vlc_, media_name_, url, str_quality.c_str(), 0, NULL, true, false);
					libvlc_vlm_play_media(vlc_, media_name_);
		} 
			}
		}

		is_start_serve_ = true;
	}
	void ScreenPush(){
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

	void OnClickOk(){
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

	void OnExit(){
		libvlc_vlm_stop_media(vlc_, media_name_);
		libvlc_vlm_release(vlc_);
	}

protected:
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }
	virtual CDuiString GetSkinFile() override { return _T("screen_capture_wnd.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("ScreenCapture"); }
	void Notify(TNotifyUI& msg) override
	{
		if (msg.sType == _T("click")){
			if (!OnSysClick(msg))
				OnUserClick(msg);
		}else if(msg.sType == _T("selectshanged")){

		}
	}
private:
	bool is_start_serve_;
	bool is_start_client_;
	INT screen_fps_;
	INT screen_fps_old_;
	CDuiString screen_quality_;
	CDuiString screen_quality_old_;
	libvlc_instance_t* vlc_;
	const char* media_name_;
	XmlManager* xml_manager_;
};

#endif	//__SCREEN_CAPTURE_H__