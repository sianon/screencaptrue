#ifndef __UICHILDLAYOUT_H__
#define __UICHILDLAYOUT_H__

#pragma once

namespace DuiLib
{
    class IDialogBuilderCallback;
	class UILIB_API CChildLayoutUI : public CContainerUI
	{
	public:
		CChildLayoutUI();
		CChildLayoutUI( IDialogBuilderCallback* pCallBack );

		void Init();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetChildLayoutXML(CDuiString pXML);
		DuiLib::CDuiString GetChildLayoutXML();
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual LPCTSTR GetClass() const;

	private:
		DuiLib::CDuiString m_pstrXMLFile;
		IDialogBuilderCallback* m_pCallback;
	};
} // namespace DuiLib
#endif // __UICHILDLAYOUT_H__
