#include "ivga_engine.h"
#include "res_singleton.h"

IvgaEngine::IvgaEngine()
	: media_name_("IVGA")
	, keyboard_Hook_(new IVGAHook)

{
	cfg_.LoadFile();
}
IvgaEngine::~IvgaEngine()
{
	OnDestory();
}

void IvgaEngine::InitStreamInfo()
{
	cfg_.GetStreamInfo(stream_info_);
}

void IvgaEngine::SetStreamInfo(StreamInfo stream_info)
{
	stream_info_old_ = stream_info_;
	stream_info_ = stream_info;
}
StreamInfo IvgaEngine::GetStreamInfo()
{
	return stream_info_;
}

bool IvgaEngine::GetIsServerState()
{
	stream_info_.is_start_serve_ = cfg_.GetRunState();
	return stream_info_.is_start_serve_;
}

void IvgaEngine::SetIsStartServe(bool start)
{
	stream_info_.is_start_serve_ = start;
	cfg_.SetRunState(stream_info_.is_start_serve_);
}

void IvgaEngine::SetFPS(INT value, bool is_push /*= false*/)
{
	if (!is_push)
		stream_info_.screen_fps_ = value;
	else
		stream_info_.screen_fps_push_ = value;
	cfg_.SetFPS(value, is_push);
}

INT IvgaEngine::GetFPS(bool is_push /*= false*/)
{
	INT ret = cfg_.GetFPS(is_push);

	if (!is_push)
		stream_info_.screen_fps_ = ret;
	else
		stream_info_.screen_fps_push_ = ret;

	return ret;
}

void IvgaEngine::SetQuality(LPCTSTR value, bool is_push /*= false*/)
{
	if (!is_push)
		stream_info_.screen_quality_ = value;
	else
		stream_info_.screen_quality_push_ = value;
	cfg_.SetQuality(value, is_push);
}

LPCTSTR IvgaEngine::GetQuality(bool is_push /*= false*/)
{
	LPCTSTR ret = cfg_.GetQuality(is_push);

	if (!is_push)
		stream_info_.screen_quality_ = ret;
	else
		stream_info_.screen_quality_push_ = ret;

	return ret;
}

void IvgaEngine::SetIpaddr(LPCTSTR value, bool is_push /*= false*/)
{
	if (!is_push)
		stream_info_.ip_server_ = value;
	else
		stream_info_.ip_push_ = value;
	cfg_.SetIpaddr(value, is_push);
}

LPCTSTR IvgaEngine::GetIpaddr(bool is_push /*= false*/)
{
	LPCTSTR ret = cfg_.GetIpaddr(is_push);

	if (!is_push)
		stream_info_.ip_server_ = ret;
	else
		stream_info_.ip_push_ = ret;

	int x = 1;
	return ret;
}

void IvgaEngine::SetPort(LPCTSTR value, bool is_push /*= false*/)
{
	if (!is_push)
		stream_info_.port_ = value;
	else
		stream_info_.port_push_ = value;
	cfg_.SetPort(value, is_push);
}

LPCTSTR IvgaEngine::GetPort(bool is_push /*= false*/)
{
	LPCTSTR ret = cfg_.GetPort(is_push);

	if (!is_push)
		stream_info_.port_ = ret;
	else
		stream_info_.port_push_ = ret;

	return ret;
}

void IvgaEngine::SetDir(LPCTSTR value, bool is_push /*= false*/)
{
	if (!is_push)
		stream_info_.dir_name_ = value;
	else
		stream_info_.dir_name_push_ = value;
	cfg_.SetDir(value, is_push);
}

LPCTSTR IvgaEngine::GetDir(bool is_push /*= false*/)
{
	LPCTSTR ret = cfg_.GetDir(is_push);

	if (!is_push)
		stream_info_.dir_name_ = ret;
	else
		stream_info_.dir_name_push_ = ret;

	return ret;
}

bool IvgaEngine::GetAutoStart()
{
	return cfg_.GetAutoStart();
}

bool IvgaEngine::SetAutoStart(bool value)
{
	return cfg_.SetAutoStart(value);
}

bool IvgaEngine::GetMinStart()
{
	return cfg_.GetMinStart();
}

bool IvgaEngine::SetMinStart(bool value)
{
	return cfg_.SetMinStart(value);
}

bool IvgaEngine::IsOnlyLive()
{
	return cfg_.IsOnlyLive();
}

bool IvgaEngine::SetOnlyLive(bool value)
{
	return cfg_.SetOnlyLive(value);
}

void IvgaEngine::StartServe()
{
	stream_info_.is_start_serve_ = true;

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
		"--live-caching=300",
	};

	const char* url = "Screen://";
	wstring first_part = L"#transcode{vcodec=mp4v,acodec=none,vb=16,threads=2,scale=";
	wstring scale = ChangeQuality(stream_info_.screen_quality_).GetData();
	wstring third_part = L"}:duplicate{dst=rtp{sdp=rtsp://";
	wstring ip_server = stream_info_.ip_server_.GetData();
	wstring double_dot = L":";
	wstring port = stream_info_.port_.GetData();
	wstring last_part = stream_info_.dir_name_.GetData();
	wstring sout = first_part + scale + third_part + ip_server + double_dot + port + L"/" + last_part + L"}}";

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
	wstring scale = ChangeQuality(stream_info_.screen_quality_).GetData();
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

CDuiString IvgaEngine::ChangeQuality(LPCTSTR value)
{
	char temp[20];
	double qua = _ttoi(value) / 10.0;
	sprintf_s(temp, "%.2lf", qua);
	CDuiString cds_temp;
	cds_temp = temp;
	return cds_temp;
}
