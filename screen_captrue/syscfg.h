#pragma once
#include "stdafx.h"
#include "..\third_party\pugixml\pugixml.hpp"

typedef struct _StreamDataInfo
{
	bool is_start_serve;
	bool is_start_client;
	INT screen_fps;
	INT screen_fps_old;
	LPCTSTR ip_server;
	LPCTSTR ip_push;
	LPCTSTR screen_quality;
	LPCTSTR screen_quality_old;
	const char* media_name;
	LPCTSTR dir_name;
	LPCTSTR port;
}StreamDataInfo;

class Syscfg
{
public:
	Syscfg();
	~Syscfg();

	bool LoadFile(const WCHAR * path = nullptr);
	bool SetPaht(const WCHAR * path = nullptr);

	pugi::xml_node GetRunState();
	bool SetRunState(bool is_serve);

	bool SetNodeAttr(string node, string attr, string value);


private:
	bool SaveFile(const WCHAR* path = nullptr);

	pugi::xml_node GetRootNode();
	pugi::xml_node GetNode(const char * name);

private:
	pugi::xml_document doc_;
	std::wstring path_;
};

