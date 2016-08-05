#ifndef __ADVANCED_WND_H__
#define __ADVANCED_WND_H__

#include "base_window.h"
#include "ip_edit_ui.h"

class AdvancedWnd : public BaseWindow
{
public:
	AdvancedWnd(LPCTSTR solution_name_);
	AdvancedWnd(LPCTSTR solution_name_, int index);
	~AdvancedWnd();

	DECLARE_DUIWND_INFO(_T("AdvancedWnd"), CS_DBLCLKS, _T("edit_more_wnd.xml"))

	BEGIN_DUICONTROL_CREATE(AdvancedWnd)
		DUICONTROL_CREATE_FROM_XML(_T("IpEdit"), _T("ip_control.xml"))
	END_DUICONTROL_CREATE()

	bool DoModal(HWND hwnd);
	bool GetFlag() { return flag_; }

	void OnUserClick(const TNotifyUI& msg) override;
	LRESULT OnInit() override;

	void OnOk();

private:
	CDuiString play_name_;
	CDuiString ip_addr_;
	CDuiString netmask_;
	bool flag_;
	bool type_is_add_;
	int index_;
};

#endif	//__ADVANCED_WND_H__