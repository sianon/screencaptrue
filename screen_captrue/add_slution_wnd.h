#ifndef __ADD_SLUTION_H__
#define __ADD_SLUTION_H__

#include "base_window.h"

class AddSlutionWnd : public BaseWindow
{
public:
	AddSlutionWnd();
	~AddSlutionWnd();

	DECLARE_DUIWND_INFO(_T("AddNewPalyWnd"), CS_DBLCLKS, _T("new_play_name.xml"))

public:
	bool DoModal(HWND hwnd);
	LPCTSTR GetNewName() { return new_name_; }

private:
	virtual void OnUserClick(const TNotifyUI& msg) override;

private:
	CDuiString new_name_;
};

#endif // __ADD_SLUTION_H__
