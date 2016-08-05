#include "ip_manager.h"
#include <shellapi.h>
#include <NetCon.h>


IpManager::IpManager()
{
	ResSingleton* res = ResSingleton::GetInstance();
	xml_manager_ = res->GetXmlManager();
	xml_manager_->LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
}


IpManager::~IpManager()
{
}

void IpManager::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("itemselect"))
		if (msg.pSender->GetName() == _T("play_list"))
			OnSelectPlay(msg);

	__super::Notify(msg);
}

LRESULT IpManager::OnInit()
{
	FlushPlayList();
	EnumNetName();

	vector<LPCTSTR> all_ip_ui_str = {
		_T("ip_address"),
		_T("netmask"),
		_T("gateway"),
		_T("firstDNS"),
		_T("secondDNS")
	};

	IpEdit* ip_ui_temp = nullptr;
	for (auto iter : all_ip_ui_str){
		ip_ui_temp = static_cast<IpEdit*>(m_PaintManager.FindControl(iter));
		ip_edit_vector_.push_back(ip_ui_temp);
	}

	return 0;
}

void IpManager::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("start_play_btn")) {       // 启动方案
		StartPlay();
	}
	else if (msg.pSender->GetName() == _T("advanced_btn")) {
		OnClickAdvanced();
	}
	else if (msg.pSender->GetName() == _T("edit_play_btn")) {
		OnClickEditBtn();
	}
	else if (msg.pSender->GetName() == _T("update_play_btn")) {
		OnClickUpdateBtn();
	}
	else if (msg.pSender->GetName() == _T("del_play_btn")) {
		OnClickDelBtn();
	}
	else if (msg.pSender->GetName() == _T("add_play_btn")) {
		OnClickAddPlayBtn();
	}
	else if (msg.pSender->GetName() == _T("test_btn")) {      // 测试内容
		OnClickTestBtn();
	}
}

void IpManager::OnClickAddPlayBtn()
{
	NETSTRUCT net_info;
	if (!GetPlayInfo(net_info)){
		MessageBox(NULL, _T("empty"), _T("message"), MB_OK);
		return;
	}

	net_info.net_name = m_PaintManager.FindControl(_T("net_name_list"))->GetText();

	AddSlutionWnd new_play;
	new_play.DoModal(*this);
	CDuiString play_name = new_play.GetNewName();
	if (play_name)
		return;

	net_info.play_name = play_name;
	xml_manager_->InsertNode(net_info);

	FlushPlayList();
	SetControlEnabled(false);
}

void IpManager::OnClickAdvanced()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO> (m_PaintManager.FindControl(_T("play_list")));
	AdvancedParent adv_wnd(play_list->GetText());
	adv_wnd.DoModal(*this);
}

void IpManager::OnClickDelBtn()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	LPCTSTR play_name = play_list->GetText();
	if (lstrcmp(play_name, _T("Auto"))){
		MessageBox(NULL, _T("can't delete"), _T("error"), MB_OK);
	}
	if (xml_manager_->RemoveNode(xml_manager_->GetNode(play_name))){
		MessageBox(NULL, _T("successed delet"), _T("message"), MB_OK);
		FlushPlayList();
	}else{
		MessageBox(NULL, _T("faild to delete"), _T("message"), MB_OK);
	}
}

void IpManager::OnClickEditBtn()
{
	SetControlEnabled(true);
}

void IpManager::OnClickUpdateBtn()
{
	NETSTRUCT net_info;
	net_info.play_name = m_PaintManager.FindControl(_T("play_list"))->GetText();

	if (net_info.net_name == _T("Auto")){
		MessageBox(NULL, _T("【Auto】方案不能保存，请选择【存为新方案】"), _T("ERROR"), MB_OK);
		return;
	}

	net_info.net_name = m_PaintManager.FindControl(_T("net_name_list"))->GetText();
	if (!GetPlayInfo(net_info)){
		MessageBox(NULL, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
		return;
	}

	if (xml_manager_->UpdateNode(net_info)){
		MessageBox(NULL, _T("方案修改成功！"), _T("ERROR"), MB_OK);
		return;
	} else {
		MessageBox(nullptr, _T("方案修改失败了哦，亲！"), _T("ERROR"), MB_OK);
		return;
	}
	SetControlEnabled(false);
}

BOOL IpManager::GetPlayInfo(NETSTRUCT& net_info)
{
	for (auto iter : ip_edit_vector_){
		if (!iter->IsReasonable())
			return FALSE;
	}

	for (int i = 0; i != 5; ++i){
		net_info.SetVar(xml_manager_->net_attrs_[i], ip_edit_vector_[i]->GetText());
	}
	return true;
}

void IpManager::FlushPlayList()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	for (int i = 1; i < play_list->GetCount();){
		play_list->RemoveAt(1);
	}

	PDUI_LISTLABELELEM list_elem;
	for (auto play_name : xml_manager_->GetAllNodeName()){
		list_elem = new CListLabelElementUI;
		list_elem->SetText(play_name);
		play_list->Add(list_elem);
	}

}

void IpManager::EnumNetName()
{
	INetConnectionManager* net_manager = nullptr;
	INetConnection* net_conn = nullptr;
	IEnumNetConnection* net_enum = nullptr;
	ULONG celt_fetched;

	setlocale(LC_CTYPE, "");
	CoInitialize(NULL);
	CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&net_manager);
	if (net_manager == nullptr){
		MessageBox(NULL, _T("网络检测失败"), _T("ERROR"), MB_OK);
		return;
	}

	net_manager->EnumConnections(NCME_DEFAULT, &net_enum);
	NETCON_PROPERTIES* net_proper;
	PDUI_COMBO net_name_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("net_name_list")));
	PDUI_LISTLABELELEM list_elem;
	while (net_enum->Next(1, & net_conn, &celt_fetched) == S_OK)
	{
		net_conn->GetProperties(&net_proper);
		list_elem = new CListLabelElementUI;
		list_elem->SetText(net_proper->pszwName);
		net_name_list->Add(list_elem);
	}
	net_name_list->SelectItem(0);
}

void IpManager::SetNetName(LPCTSTR net_name)
{
	PDUI_COMBO net_name_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("net_name_list")));
	int count = net_name_list->GetCount();
	for (int i = 0; i < count; ++i){
		if (net_name_list->GetItemAt(i)->GetText() == net_name){
			net_name_list->SelectItem(i);
			return;
		}
	}
}

void IpManager::OnSelectPlay(TNotifyUI& msg)
{
	SetControlEnabled(FALSE);
	CDuiString play_name = msg.pSender->GetText();
	if (play_name == _T("auto")){
		for (auto iter : ip_edit_vector_){
			iter->SetText(_T("..."));
		}
		m_PaintManager.FindControl(_T("net_name_list"))->SetEnabled(true);
		return;
	}

	NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_name);
	for (int i = 0; i != 5; ++i){
		ip_edit_vector_[i]->SetText(net_info.GetVar(xml_manager_->net_attrs_[i]));
	}
	SetNetName(net_info.net_name);
}

void IpManager::StartPlay()
{
	vector<CDuiString> command_lines;
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	vector<CDuiString> param;

	if (play_list->GetText() == _T("Auto")){
		param.clear();
		param.push_back(_T("set address"));
		param.push_back(GetNetName());
		param.push_back(_T("dhcp"));
		command_lines.push_back(MakeComLine(param));

		param.clear();
		param.push_back(_T("set dnsservers"));
		param.push_back(GetNetName());
		param.push_back(_T("dhcp"));
		command_lines.push_back(MakeComLine(param));
	}else{
		NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_list->GetText());

		param.clear();
		param.push_back(_T("set address"));
		param.push_back(GetNetName());
		param.push_back(_T("static"));
		param.push_back(net_info.ip_address + _T(""));
		param.push_back(net_info.netmask + _T(""));
		param.push_back(net_info.gateway);
		command_lines.push_back(MakeComLine(param));

		param.clear();
		param.push_back(_T("set dnsservers"));
		param.push_back(GetNetName());
		param.push_back(_T("static"));
		param.push_back(net_info.firstDNS);
		command_lines.push_back(MakeComLine(param));

		for (auto iter : net_info.more_ip_mask){
			param.clear();
			param.push_back(_T("add address"));
			param.push_back(GetNetName());
			param.push_back(iter.first + _T(""));
			param.push_back(iter.second);
			command_lines.push_back(MakeComLine(param));
		}
	}

	if (ExcuteCommand(command_lines))
		MessageBox(NULL, _T("方案启用成功"), _T("Message"), MB_OK);
	else
		MessageBox(NULL, _T("方案启用失败"), _T("Message"), MB_OK);
}

BOOL IpManager::ExcuteCommand(LPCTSTR command_lien)
{
	SHELLEXECUTEINFO shell_info = {0};
	shell_info.cbSize = sizeof(SHELLEXECUTEINFO);
	shell_info.fMask = SEE_MASK_NOCLOSEPROCESS;
	shell_info.lpVerb = _T("runas");
	shell_info.hwnd = NULL;
	shell_info.lpFile = _T("cmd.exe");
	shell_info.lpDirectory = NULL;
	shell_info.nShow = SW_HIDE;
	shell_info.lpParameters = command_lien;
	shell_info.hInstApp = NULL;

	DWORD exitcode;
	ShellExecuteEx(&shell_info);
	return GetExitCodeProcess(shell_info.hProcess, &exitcode);
}

BOOL IpManager::ExcuteCommand(vector<CDuiString> command_lien_s)
{
	for (auto iter : command_lien_s)
		if (!ExcuteCommand(iter))
			return FALSE;
	return TRUE;
}

void IpManager::SetControlEnabled(bool enable){
	for (auto iter : ip_edit_vector_){
		iter->SetStateEdit(enable);
	}
	m_PaintManager.FindControl(_T("net_name_list"))->SetEnabled(enable);
	m_PaintManager.FindControl(_T("update_play_btn"))->SetVisible(enable);
	m_PaintManager.FindControl(_T("edit_play_btn"))->SetVisible(enable);
}

CDuiString IpManager::MakeComLine(vector<CDuiString> prarm)
{
	CDuiString ret = _T("/c netsh interface ip");
	for (auto iter : prarm){
		ret += iter;
	}
	return ret;
}

LPCTSTR IpManager::GetNetName()
{
	CDuiString ret = _T("\"");
	PDUI_CONTROL play_list = m_PaintManager.FindControl(_T("play_list"));
	if (play_list->GetText() == _T("Auto")){
		ret += m_PaintManager.FindControl(_T("net_name_list"))->GetText() + _T("\"");
	}else{
		ret += xml_manager_->GetNodeInfo(play_list->GetText()).net_name + _T("\"");
	}
	return ret;
}

void IpManager::OnClickTestBtn()
{
}
