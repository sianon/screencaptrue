#pragma once
#include "stdafx.h"
#include "..\third_party\pugixml\pugixml.hpp"

struct StreamInfo
{
	bool is_start_serve_;

	INT screen_fps_;
	CDuiString port_;
	CDuiString dir_name_;
	CDuiString ip_server_;
	CDuiString screen_quality_;

	CDuiString ip_push_;
	INT screen_fps_push_;
	CDuiString port_push_;
	CDuiString dir_name_push_;
	CDuiString screen_quality_push_;

	bool operator==(const StreamInfo stream) {
		return(stream.screen_fps_ == screen_fps_
			&& stream.port_ == port_
			&& stream.dir_name_ == dir_name_
			&& stream.ip_push_ == ip_push_
			&& stream.ip_server_ == ip_server_
			&& stream.screen_quality_ == screen_quality_
			&& stream.screen_fps_push_ == screen_fps_push_
			&& stream.port_push_ == port_push_
			&& stream.dir_name_push_ == dir_name_push_
			&& stream.screen_quality_push_ == dir_name_push_);
	}
};

class Syscfg
{
public:
	Syscfg();
	~Syscfg();

	bool LoadFile(const WCHAR * path = nullptr);
	bool SetPaht(const WCHAR * path = nullptr);

	bool GetIsLoad() { return is_load_; }

	bool GetRunState();
	bool SetRunState(bool is_serve);

	int GetFPS(bool is_push = false);
	bool SetFPS(INT value, bool is_push = false);

	LPCTSTR GetQuality(bool is_push = false);
	bool SetQuality(LPCTSTR value, bool is_push = false);

	LPCTSTR GetIpaddr(bool is_push = false);
	bool SetIpaddr(LPCTSTR value, bool is_push = false);

	LPCTSTR GetPort(bool is_push = false);
	bool SetPort(LPCTSTR value, bool is_push = false);

	LPCTSTR GetDir(bool is_push = false);
	bool SetDir(LPCTSTR value, bool is_push = false);

	bool GetAutoStart();
	bool SetAutoStart(bool value);

	bool GetMinStart();
	bool SetMinStart(bool value);

	bool IsOnlyLive();
	bool SetOnlyLive(bool value);

	void GetStreamInfo(StreamInfo & info);

	char * WideToMulti(CDuiString wide, char * multi);
	CDuiString MultiToWide(string multi);

private:
	bool SaveFile(const WCHAR* path = nullptr);

	pugi::xml_node GetRootNode();
	pugi::xml_node GetNode(const char * name);

private:
	pugi::xml_document doc_;
	std::wstring path_;
	bool is_load_;
};

