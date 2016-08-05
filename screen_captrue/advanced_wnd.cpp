#include "advanced_wnd.h"
#include "res_singleton.h"

AdvancedWnd::AdvancedWnd(LPCTSTR play_name)
	: play_name_(play_name)
	, index_(-1)
	, ip_addr_(_T(""))
	, netmask_(_T(""))
	, flag_(FALSE)
	, type_is_add_(TRUE)
{
}

AdvancedWnd::AdvancedWnd(LPCTSTR solution_name_, int index)
	: index_(index)
	, play_name_(solution_name_)
	, ip_addr_(_T(""))
	, netmask_(_T(""))
	, flag_(FALSE)
	, type_is_add_(TRUE)
{
	XmlManager* xml_manager = ResSingleton::GetInstance()->GetXmlManager();
	pugi::xml_node node = xml_manager->GetNode(play_name_).child("MoreIP");
	node = xml_manager->GetNode(node, index_);
	ip_addr_ = xml_manager->MultiToWide(node.attribute("ip").as_string());
	netmask_ = xml_manager->MultiToWide(node.attribute("netmask").as_string());
}


AdvancedWnd::~AdvancedWnd()
{
}

void AdvancedWnd::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("OK")){
		OnOk();
	}
}

LRESULT AdvancedWnd::OnInit()
{
	IpEdit* ip_addr = static_cast<IpEdit*>(m_PaintManager.FindControl(_T("edit_ipaddr")));
	IpEdit* netmask = static_cast<IpEdit*>(m_PaintManager.FindControl(_T("edit_netmask")));

	ip_addr->SetText(ip_addr_);
	ip_addr->SetStateEdit(true);
	netmask->SetText(netmask_);
	netmask->SetStateEdit(true);

	return LRESULT();
}

bool AdvancedWnd::DoModal(HWND hwnd)
{
	if (!Create(hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(hwnd);

	return ShowModal() != 0;
}

void AdvancedWnd::OnOk()
{
	IpEdit *ipaddr = static_cast<IpEdit *>(m_PaintManager.FindControl(_T("edit_ipaddr")));
	IpEdit *netmask = static_cast<IpEdit *>(m_PaintManager.FindControl(_T("edit_netmask")));

	if (!ipaddr->IsReasonable() || !netmask->IsReasonable()) {
		MessageBox(nullptr, _T("信息不合理，请确认后重试！"), _T("ERROR"), MB_OK);
		return;
	}

	XmlManager *xml_manage = ResSingleton::GetInstance()->GetXmlManager();
	pugi::xml_node pa_node = xml_manage->GetNode(play_name_).child("MoreIP");
	pair<string, LPCTSTR> param1 = { "ip", ipaddr->GetText() };
	pair<string, LPCTSTR> param2 = { "netmask", netmask->GetText() };
	BOOL is_have = xml_manage->IsHave(pa_node, param1, param2);

	if (type_is_add_) {
		xml_manage->InsertNode(pa_node, param1, param2);
		flag_ = TRUE;
		Close();
	}
	else {
		if (xml_manage->UpdateNode(pa_node, index_, param1, param2)) {
			MessageBox(nullptr, _T("信息修改成功！"), _T("Message"), MB_OK);
			flag_ = TRUE;
			Close();
		}
	}
}