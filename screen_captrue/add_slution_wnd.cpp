#include "add_slution_wnd.h"


AddSlutionWnd::AddSlutionWnd()
	: new_name_(_T(""))
{
}


AddSlutionWnd::~AddSlutionWnd()
{
}

bool AddSlutionWnd::DoModal(HWND hwnd)
{
	if (!Create(hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(hwnd);

	return ShowModal() != 0;
}

void AddSlutionWnd::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("OK_btn")) {
		PDUI_EDIT name_edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("name_edit")));
		new_name_ = name_edit->GetText();
		if (new_name_ == _T(""))
			MessageBox(NULL, _T("empty"), _T("Meassage"), MB_OK);
		Close();
	}else if (msg.pSender->GetName() == _T("cancel_btn")){
		Close();
	}
}