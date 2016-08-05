#include "ip_edit_ui.h"


IpEdit::IpEdit()
{
	vector<LPCTSTR> edit = {
		_T("ip_edit_0"),
		_T("ip_edit_1"),
		_T("ip_edit_2"),
		_T("ip_edit_3")
	};

	for (auto ip_str : edit){
		ip_edit_val_.push_back(ip_str);
	}
}

IpEdit::~IpEdit()
{
}

LPVOID IpEdit::GetInterface(LPCTSTR pstr_name)
{
	if (_tcscmp(pstr_name, _T("IpEdit")) == 0)
		return static_cast<IpEdit*>(this);

	return CHorizontalLayoutUI::GetInterface(pstr_name);
}

void IpEdit::Init()
{
	for (auto str : ip_edit_val_) {
		ip_edit_vector_.push_back(static_cast<PDUI_EDIT>(FindSubControl(str)));
	}

	__super::Init();
}

CDuiString IpEdit::GetText() const
{
	CDuiString ret = _T("");
	for (auto ip_edit_elem : ip_edit_vector_){
		ret += ip_edit_elem->GetText();
		ret += _T(".");
	}

	return ret.Mid(0, ret.GetLength() - 1);
}

void IpEdit::SetText(LPCTSTR pstr_text)
{
	CDuiString ip_str = pstr_text;
	int begin = 0, end = 0;

	for (auto ip_elem_edit : ip_edit_vector_){
		end = ip_str.Find(_T("."), begin);
		ip_elem_edit->SetText(ip_str.Mid(begin, end - begin));
		begin = end + 1;
	}
}

BOOL IpEdit::IsReasonable()
{
	for (auto iter : ip_edit_vector_)
		if (iter->GetText() == _T(""))
			return FALSE;

	return TRUE;
}

void IpEdit::SetStateEdit(bool state)
{
	for (auto iter : ip_edit_vector_){
		iter->SetEnabled(state);
	}

	if (state)
		ip_edit_vector_[0]->GetParent()->SetBkColor(0xffffffff);
	else
		ip_edit_vector_[0]->GetParent()->SetBkColor(0x00aaaaaa);
}