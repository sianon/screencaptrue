#pragma once
#include "stdafx.h"
#include "..\third_party\pugixml\pugixml.hpp"

class Syscfg
{
public:
	Syscfg();
	~Syscfg();

	bool LoadFile(const WCHAR * path = nullptr);
	bool SetPaht(const WCHAR * path = nullptr);

	pugi::xml_node GetTestNode();
	bool SetTestNode(std::wstring attr);

private:
	bool SaveFile(const WCHAR* path = nullptr);

	pugi::xml_node GetRootNode();
	pugi::xml_node GetNode(const char * name);
	

private:
	pugi::xml_document doc_;
	std::wstring path_;
	volatile bool modified_;
};

