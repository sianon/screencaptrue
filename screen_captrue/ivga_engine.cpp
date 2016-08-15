#include "ivga_engine.h"


IvgaEngine::IvgaEngine()
	: media_name_("IVGA")
{
}
IvgaEngine::~IvgaEngine()
{
	OnDestory();
}

void IvgaEngine::SetStreamInfo(StreamInfo stream_info)
{
	stream_info_ = stream_info_old_;
	stream_info_ = stream_info;
}
StreamInfo IvgaEngine::GetStreamInfo()
{
	return stream_info_;
}

bool IvgaEngine::GetServerState()
{
	return stream_info_.is_start_serve_;
}

void IvgaEngine::StartServe()
{
//	is_start_serve_ = true;

	if (stream_info_.is_start_serve_){
		if (!vlc_){
			ProcessServer();
		}else{
			if (!(stream_info_ == stream_info_old_)){
				OnDestory();
				ProcessServer();
			}
		}
	}

}
void IvgaEngine::StartClient()
{
	if (stream_info_.is_start_serve_){
		OnDestory();
	}else{
		if (!(stream_info_ == stream_info_old_)){
			OnDestory();
			ProcessPush();
		}
		return;
	}

	stream_info_.is_start_serve_ = false;
}

void IvgaEngine::OnDestory()
{
	if (!vlc_)
		return;
	libvlc_vlm_stop_media(vlc_, media_name_);
	libvlc_vlm_release(vlc_);
}
void IvgaEngine::ProcessServer()
{
	string screen_fps("--screen-fps=");
	char fps[3];
	_itoa_s(stream_info_.screen_fps_, fps, 3, 10);
	screen_fps.append(fps);

	const char * const argv[] = {
		screen_fps.c_str(),
		"--screen-follow-mouse",
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = stream_info_.screen_quality_.GetData();
	wstring third_part = L"}:duplicate{dst=rtp{sdp=rtsp://:";
	wstring ip_server = stream_info_.ip_server_.GetData();
	wstring double_dot = L":";
	wstring port = stream_info_.port_.GetData();
	wstring last_part = stream_info_.dir_name_.GetData();
	wstring sout = first_part + scale + third_part + ip_server + port + L"/" + last_part + L"}}";

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, CW2A(sout.c_str()), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);
}
void IvgaEngine::ProcessPush()
{
	string screen_fps("--screen-fps=");
	char fps[3];
	_itoa_s(stream_info_.screen_fps_push_, fps, 3, 10);
	screen_fps.append(fps);

	const char * const argv[] = {
		screen_fps.c_str(),
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = stream_info_.screen_quality_push_.GetData();
	wstring third_part = L"}:standard{access=udp, mux=ts, dst=";
	wstring ip_push = stream_info_.ip_push_.GetData();
	wstring double_dot = L":";
	wstring port = stream_info_.port_push_.GetData();
	wstring last_part = stream_info_.dir_name_push_.GetData();
	wstring sout = first_part + scale + third_part + ip_push + double_dot + port + L"/" + last_part + L"}";

	vlc_ = libvlc_new(sizeof(argv) / sizeof(argv[0]), argv);
	libvlc_vlm_add_broadcast(vlc_, media_name_, url, CW2A(sout.c_str()), 0, NULL, true, false);
	libvlc_vlm_play_media(vlc_, media_name_);
}
