#include "syscfg.h"
#include "..\third_party\pugixml\pugixml.cpp"


Syscfg::Syscfg()
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
			return true;
		doc_.reset();
	}
	return false;
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

pugi::xml_node Syscfg::GetRunState()
{
	return GetNode("run_state");
}

bool Syscfg::SetRunState(bool attr)
{
	pugi::xml_node node = GetRunState();
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

bool Syscfg::SetNodeAttr(string node_n, string attr_n, string value)
{
	pugi::xml_node node = GetNode(node_n.c_str());
	pugi::xml_attribute attr = node.attribute(attr_n.c_str());
	if (!attr)
		attr = node.append_attribute(attr_n.c_str());
	attr = value.c_str();

	return SaveFile();
}

