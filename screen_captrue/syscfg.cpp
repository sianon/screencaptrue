#include "syscfg.h"
#include "..\third_party\pugixml\pugixml.cpp"


Syscfg::Syscfg()
	: is_load_(false)
{
}

Syscfg::~Syscfg()
{
}

bool Syscfg::LoadFile(const WCHAR * path /*= nullptr*/)
{
	if (path && wcslen(path) != 0)
		path_ = path;
	else
		path_ = _T("syscfg.xml");

	if (doc_.load_file(path_.c_str())) {
		if (doc_.child("cfginfo"))
			return is_load_ = true;
		doc_.reset();
	}
	return is_load_ = false;
}

bool Syscfg::SetPaht(const WCHAR * path /*= nullptr*/)
{
	if (path && wcslen(path) != 0) {
		path_ = path;
		return true;
	}
	return false;
}

bool Syscfg::SaveFile(const WCHAR * path)
{
	if (path)
		return doc_.save_file(path, "  ", pugi::format_indent | pugi::format_write_bom, pugi::encoding_utf8);
	else
		return doc_.save_file(path_.c_str(), "  ", pugi::format_indent | pugi::format_write_bom, pugi::encoding_utf8);
	return false;
}

pugi::xml_node Syscfg::GetRootNode()
{
	pugi::xml_node node = doc_.child("cfginfo");
	if (!node) {
		if (doc_.first_child().type() != pugi::node_declaration) {
			doc_.append_child(pugi::node_declaration).set_name("xml version=\"1.0\" encoding=\"utf-8\"");
		}
		node = doc_.append_child("cfginfo");
		node.append_attribute("name") = L"default";
		node.append_attribute("version") = L"1.0";
	}
	return node;
}

pugi::xml_node Syscfg::GetNode(const char * name)
{
	pugi::xml_node node = GetRootNode().child(name);
	if (!node)
		node = GetRootNode().append_child(name);
	return node;
}

bool Syscfg::GetRunState()
{
	pugi::xml_node node = GetNode("run_state");
	pugi::xml_attribute node_attr = node.attribute("serve");
	if (!node_attr) {
		node_attr = node.append_attribute("serve");
		node_attr = true;
	}

	node_attr = node.attribute("client");
	if (!node_attr) {
		node_attr = node.append_attribute("client");
		node_attr = false;
		SaveFile();
	}

	return !node_attr.as_bool();
}

bool Syscfg::SetRunState(bool attr)
{
	pugi::xml_node node = GetNode("run_state");
	pugi::xml_attribute node_attr = node.attribute("serve");
	if (!node_attr)
		node_attr = node.append_attribute("serve");
	node_attr = attr;

	node_attr = node.attribute("client");
	if (!node_attr)
		node_attr = node.append_attribute("client");
	node_attr = !attr;

	return SaveFile();
}

int Syscfg::GetFPS(bool is_push /*= false*/)
{
	pugi::xml_node node;
	if(!is_push)
		node = GetNode("serve_data");
	else 
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("fps");
	if (!attr) {
		attr = node.append_attribute("fps");
		attr = 25;
		SaveFile();
	}

	return attr.as_int();
}

bool Syscfg::SetFPS(INT value, bool is_push /*= false*/)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("fps");
	if (!attr)
		attr = node.append_attribute("fps");
	attr = value;

	return SaveFile();
}

LPCTSTR Syscfg::GetQuality(bool is_push /*= false*/)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("quality");
	if (!attr) {
		attr = node.append_attribute("quality");
		attr = "7";
		SaveFile();
	}

	return MultiToWide(attr.as_string());
}

bool Syscfg::SetQuality(LPCTSTR value, bool is_push /*= false*/)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("quality");
	if (!attr)
		attr = node.append_attribute("quality");
	char multi[MAX_PATH];
	attr = WideToMulti(value, multi);

	return SaveFile();
}

LPCTSTR Syscfg::GetIpaddr(bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("ipaddr");
	if (!attr) {
		attr = node.append_attribute("ipaddr");
		if (!is_push)
			attr = "10.18.3.62";
		else 
			attr = "10.18.3.61";
		SaveFile();
	}

	return MultiToWide(attr.as_string());
}

bool Syscfg::SetIpaddr(LPCTSTR value, bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("ipaddr");
	if (!attr)
		attr = node.append_attribute("ipaddr");
	char multi[MAX_PATH];
	attr = WideToMulti(value, multi);

	return SaveFile();
}

LPCTSTR Syscfg::GetPort(bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("port");
	if (!attr) {
		attr = node.append_attribute("port");
		attr = "554";
		SaveFile();
	}

	return MultiToWide(attr.as_string());
}

bool Syscfg::SetPort(LPCTSTR value, bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("port");
	if (!attr)
		attr = node.append_attribute("port");
	attr = "554";

	return SaveFile();
}

LPCTSTR Syscfg::GetDir(bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("dir_name");
	if (!attr) {
		attr = node.append_attribute("dir_name");
		if (!is_push)
			attr = "live123";
		else
			attr = "push123";
		SaveFile();
	}

	return MultiToWide(attr.as_string());
}

bool Syscfg::SetDir(LPCTSTR value, bool is_push)
{
	pugi::xml_node node;
	if (!is_push)
		node = GetNode("serve_data");
	else
		node = GetNode("push_data");

	pugi::xml_attribute attr = node.attribute("dir_name");
	if (!attr)
		attr = node.append_attribute("dir_name");
	if (!is_push)
		attr = "live123";
	else
		attr = "push123";

	return SaveFile();
}

void Syscfg::GetStreamInfo(StreamInfo & info)
{
	info.dir_name_ = GetDir();
	info.dir_name_push_ = GetDir(true);
	info.ip_push_ = GetIpaddr();
	info.ip_server_ = GetIpaddr(true);
	info.is_start_serve_ = GetRunState();
	info.port_ = GetPort();
	info.port_push_ = GetPort(true);
	info.screen_fps_ = GetFPS();
	info.screen_fps_push_ = GetFPS(true);
	info.screen_quality_ = GetQuality();
	info.screen_quality_push_ = GetQuality(true);
}

char * Syscfg::WideToMulti(CDuiString wide, char * multi)
{
	ZeroMemory(multi, MAX_PATH);
	int wide_len = wide.GetLength();
	int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
	::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
	return multi;
}

CDuiString Syscfg::MultiToWide(string multi)
{
	int multi_len = multi.length();
	int wide_len = ::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, NULL, 0);
	wchar_t *wide_str;
	wide_str = new wchar_t[wide_len + 1];
	memset(wide_str, 0, (wide_len + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, (LPWSTR)wide_str, wide_len);

	CDuiString ret = (wchar_t*)wide_str;
	delete wide_str;

	return ret;
}

