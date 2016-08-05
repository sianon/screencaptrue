#ifndef __ADVANCED_PARENT_H__
#define __ADVANCED_PARENT_H__

#include "stdafx.h"
#include "base_window.h"

class AdvancedParent : public BaseWindow
{
public:
	AdvancedParent();
	AdvancedParent(LPCTSTR play_name);
	~AdvancedParent();

	DECLARE_DUIWND_INFO(_T("AdvacedWnd"), CS_DBLCLKS, _T("advanced_wnd.xml"))

	virtual LRESULT OnInit() override;
	virtual void OnUserClick(const TNotifyUI& msg) override;

	bool DoModal(HWND hwnd);

private:
	void FlushList();
	void OnAdd();
	void OnUpdata();
	void OnDel();

	CDuiString play_name_;
	vector<CDuiString> ip_;
	vector<CDuiString> netmask_;
};

#endif// __ADVANCED_PARENT_H__