#include "advanced_parent.h"
#include "advanced_wnd.h"
#include "res_singleton.h"
#include "xml_manager.h"


AdvancedParent::AdvancedParent()
	: play_name_(_T(""))
{
}

AdvancedParent::AdvancedParent(LPCTSTR play_name)
{
	AdvancedParent();
	play_name_ = play_name;
}

AdvancedParent::~AdvancedParent()
{
}

LRESULT AdvancedParent::OnInit()
{
	FlushList();

	return 0;
}

void AdvancedParent::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("add_btn")) {
		OnAdd();
	}
	else if (msg.pSender->GetName() == _T("update_btn")) {
		OnUpdata();
	}
	else if (msg.pSender->GetName() == _T("del_btn")) {
		OnDel();
	}
}

bool AdvancedParent::DoModal(HWND pa_hwnd)
{
	if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(pa_hwnd);

	return ShowModal() != 0;
}

void AdvancedParent::FlushList()
{
	XmlManager* xml_manager = ResSingleton::GetInstance()->GetXmlManager();
	NETSTRUCT net_info = xml_manager->GetNodeInfo(play_name_);

	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	ip_list->RemoveAll();
	PDUI_LISTTEXTELEM elem;

	for (auto iter : net_info.more_ip_mask) {
		elem = new CListTextElementUI;
		ip_list->Add(elem);
		elem->SetText(0, iter.first);
		elem->SetText(1, iter.second);
	}
}

void AdvancedParent::OnAdd()
{
	AdvancedWnd more_wnd(play_name_);
	more_wnd.DoModal(*this);
	if (more_wnd.GetFlag())
		FlushList();
}

void AdvancedParent::OnUpdata()
{
	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	int index = ip_list->GetCurSel();
	if (index != -1) {
		AdvancedWnd more_wnd(play_name_, index);
		more_wnd.DoModal(*this);
		if (more_wnd.GetFlag())
			FlushList();
	}
}

void AdvancedParent::OnDel()
{
	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	int index = ip_list->GetCurSel();
	if (index != -1) {
		if (IDNO == MessageBox(nullptr, _T("确定删除该条信息？？"), _T("Warning"), MB_YESNO))
			return;

		XmlManager* xml_manager = ResSingleton::GetInstance()->GetXmlManager();
		pugi::xml_node pa_node = xml_manager->GetNode(play_name_).child("MoreIP");
		if (xml_manager->RemoveNode(pa_node, index)) {
			MessageBox(nullptr, _T("IP 删除成功!"), _T("Message"), MB_OK);
			FlushList();
		}
		else {
			MessageBox(nullptr, _T("IP 删除失败!"), _T("ERROR"), MB_OK);
		}
	}
}