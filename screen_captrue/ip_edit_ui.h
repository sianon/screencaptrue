#ifndef __IP_EDIT_H__
#define __IP_EDIT_H__

#include "stdafx.h"

class IpEdit : public CHorizontalLayoutUI
{
public:
	IpEdit();
	~IpEdit();

	LPCTSTR GetClass() const override { return _T("IpEdit"); }
	LPVOID GetInterface(LPCTSTR pstr_name) override;
	virtual void Init() override;
	virtual CDuiString GetText() const override;
	virtual void SetText(LPCTSTR pstr_text) override;

	BOOL IsReasonable();
	void SetStateEdit(bool state);

private:
	std::vector<PDUI_EDIT> ip_edit_vector_;
	std::vector<LPCTSTR> ip_edit_val_;
};

#endif // __IP_EDIT_H__