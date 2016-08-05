/************************************************************************/
// Message crack for Duilib. Rely on WindowImplBase.h !
//
//							By. Clouderman
//						2012-9-18 10:58:12
// Last modify : 
//		* 2012-10-15 15:00:43 Add IDialogBuilderCallback::CreateControl map
//		* 2012-10-12 10:49:19 Add notify types, dui types define
//		* 2012-09-24 10:34:08 Add WM_COMMAND¡¢WM_NOTIFY crack from ATL
/************************************************************************/

#ifndef _UICRACK_H___
#define _UICRACK_H___

#pragma once

#ifndef _UIWND_H___
	#error UICrack.h requires UIWnd.h(contain CDuiWindowImpl class) to be included first
#endif

/*
 *	Declare dui window class
 */

#define DECLARE_DUIWND_INFO(WndClassName, WndClassStyle, szXmlFileName) \
public: \
	LPCTSTR GetWindowClassName() const \
	{ \
		return WndClassName; \
	} \
	\
	UINT GetClassStyle() const \
	{ \
		return WndClassStyle; \
	} \
	DuiLib::CDuiString GetSkinFile() \
	{ \
	return (LPCTSTR)szXmlFileName; \
	}

/*
 *	WindowImplBase::HandleCustomMessage map
 */

#define BEGIN_DUIMSG_MAP(theClass) \
public: \
	LRESULT HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled ) \
	{ \
		LRESULT lRes = 0;

//	Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

#define DUIMSG_HANDLER(msg, func) \
		if ( uMsg == msg ) \
		{ \
			bHandled = TRUE; \
			lRes = func(uMsg, wParam, lParam, bHandled); \
			if( bHandled ) \
				return lRes; \
		}

#define DUIMSG_RANGE_HANDLER(msgFirst, msgLast, func) \
		if( uMsg >= msgFirst && uMsg <= msgLast ) \
		{ \
			bHandled = TRUE; \
			lRes = func(uMsg, wParam, lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_COMMAND_HANDLER(id, code, func) \
		if( uMsg == WM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam) ) \
		{ \
			bHandled = TRUE; \
			lRes = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if(bHandled) \
				return TRUE; \
		}

#define DUIMSG_COMMAND_ID_HANDLER(id, func) \
		if( uMsg == WM_COMMAND && id == LOWORD(wParam) ) \
		{ \
			bHandled = TRUE; \
			lRes = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_COMMAND_CODE_HANDLER(code, func) \
		if( uMsg == WM_COMMAND && code == HIWORD(wParam) ) \
		{ \
			bHandled = TRUE; \
			lRes = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_COMMAND_RANGE_HANDLER(idFirst, idLast, func) \
		if( uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast ) \
		{ \
			bHandled = TRUE; \
			lRes = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_COMMAND_RANGE_CODE_HANDLER(idFirst, idLast, code, func) \
		if( uMsg == WM_COMMAND && code == HIWORD(wParam) && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast ) \
		{ \
			bHandled = TRUE; \
			lRes = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_NOTIFY_HANDLER(id, cd, func) \
		if( uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code ) \
		{ \
			bHandled = TRUE; \
			lRes = func((int)wParam, (LPNMHDR)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_NOTIFY_ID_HANDLER(id, func) \
		if( uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom ) \
		{ \
			bHandled = TRUE; \
			lRes = func((int)wParam, (LPNMHDR)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_NOTIFY_CODE_HANDLER(cd, func) \
		if( uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code ) \
		{ \
			bHandled = TRUE; \
			lRes = func((int)wParam, (LPNMHDR)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_NOTIFY_RANGE_HANDLER(idFirst, idLast, func) \
		if( uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast ) \
		{ \
			bHandled = TRUE; \
			lRes = func((int)wParam, (LPNMHDR)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define DUIMSG_NOTIFY_RANGE_CODE_HANDLER(idFirst, idLast, cd, func) \
		if( uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast ) \
		{ \
			bHandled = TRUE; \
			lRes = func((int)wParam, (LPNMHDR)lParam, bHandled); \
			if( bHandled ) \
				return TRUE; \
		}

#define CHAIN_DUIMSG_MAP(theClass) \
		{ \
			lRes = theClass::HandleCustomMessage(uMsg, wParam, lParam, bHandled); \
			if( bHandled ) \
				return lRes; \
		}

#define END_DUIMSG_MAP() \
		bHandled = FALSE; \
		return 0; \
	}

#define DECLARE_EMPTY_DUIMSG_MAP() \
protected: \
	LRESULT HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled ) \
	{ \
		LRESULT lRes = 0; \
		bHandled = FALSE; \
		return 0; \
	}


/*
 *	Notify msg map
 */
#define BEGIN_DUINOTIFY_MAP(theClass) \
protected: \
	typedef DuiLib::TNotifyUI TNotifyUI; \
	\
	void Notify(TNotifyUI &msg) \
	{ \
        bool handled = true;

#define DUINOTIFY_HANDLER(CtrlName, NotifyType, func) \
    if (msg.pSender \
        && !msg.pSender->GetName().CompareNoCase(CtrlName) \
        && !msg.sType.CompareNoCase(NotifyType) \
        ) \
    { \
        func(msg, handled); \
        if (handled) \
            return; \
    }

#define DUINOTIFY_TYPE_HANDLER(NotifyType, func) \
	if (!msg.sType.CompareNoCase(NotifyType)) \
	{ \
		func(msg, handled); \
        if (handled) \
		    return; \
	}

#define BEGIN_DUINOTIFY_TYPE_HANDLER(NotifyType) \
	if (!msg.sType.CompareNoCase(NotifyType)) \
	{

//	Handler prototypes:
//  void NotifyHandler(TNotifyUI &msg, bool& handled);

#define DUINOTIFY_CONTROL_HANDLER(control, func) \
    if (msg.pSender == control) \
    { \
        func(msg, handled); \
        if (handled) \
            return; \
    }

#define DUINOTIFY_NAME_HANDLER(CtrlName, func) \
	if (!msg.pSender->GetName().CompareNoCase(CtrlName)) \
	{ \
		func(msg, handled); \
        if (handled) \
		    return; \
	}

#define END_DUINOTIFY_TYPE_HANDLER() \
        if (handled) \
		    return; \
	}

#define CHAIN_DUINOTIFY_MAP(theClass) \
    { \
        theClass::Notify(msg); \
    }

#define END_DUINOTIFY_MAP() \
	}


/*
 *	IDialogBuilderCallback::CreateControl map
 */
#define BEGIN_DUICONTROL_CREATE(theClass) \
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) \
	{ \
		DuiLib::CDuiString strClass(pstrClass);

#define DUICONTROL_CREATE(XmlTagName, UIClass) \
	if ( ! strClass.CompareNoCase(XmlTagName) ) \
		return ( new UIClass );

#define DUICONTROL_CREATE_FROM_XML(XmlTagName, XmlFile) \
    if ( ! strClass.CompareNoCase(XmlTagName) ) \
    { \
        DuiLib::CDialogBuilder builder; \
        DuiLib::CControlUI *pControl = builder.Create(XmlFile, NULL, this, &GetPaintManager()); \
        ASSERT(pControl != NULL && "DUICONTROL_CREATE_FROM_XML"); \
        return pControl; \
    }

#define END_DUICONTROL_CREATE() \
		return NULL; \
	}


/*
 *	Notify types
 */

#define DUINOTIFY_WINDOWINIT			(_T("WindowInit"))
#define DUINOTIFY_ONPAINT               (_T("OnPaint"))
#define DUINOTIFY_SETFOCUS				(_T("SetFocus"))
#define DUINOTIFY_KILLFOCUS				(_T("KillFocus"))
#define DUINOTIFY_MENU					(_T("Menu"))
#define DUINOTIFY_TIMER					(_T("Timer"))

#define DUINOTIFY_CLICK					(_T("Click"))
#define DUINOTIFY_SELECTCHANGED			(_T("SelectChanged"))
#define DUINOTIFY_VALUECHANGED			(_T("ValueChanged"))
#define DUINOTIFY_TEXTCHANGED			(_T("TextChanged"))
#define DUINOTIFY_RETURN				(_T("Return"))

#define DUINOTIFY_HEADERCLICK			(_T("HeaderClick"))
#define DUINOTIFY_ITEMACTIVATE			(_T("ItemActivate"))
#define DUINOTIFY_ITEMSELECT			(_T("ItemSelect"))
#define DUINOTIFY_ITEMCLICK				(_T("ItemClick"))
#define DUINOTIFY_ITEMEXPAND			(_T("ItemExpand"))
#define DUINOTIFY_ITEMCOLLAPSE			(_T("ItemCollapse"))
#define DUINOTIFY_RIGHTITEMCLICK        ( _T("ItemRClick") )

#define DUINOTIFY_LINK					(_T("Link"))
#define DUINOTIFY_TABSELECT				(_T("TabSelect"))
#define DUINOTIFY_SHOWACTIVEX			(_T("ShowActiveX"))
#define DUINOTIFY_DROPDOWN				(_T("DropDown"))
#define DUINOTIFY_SCROLLED				(_T("Scrolled"))

/*
 *	Dui types define
 */

typedef DuiLib::CControlUI					DUI_CONTROL, *PDUI_CONTROL;
typedef DuiLib::CContainerUI				DUI_CONTAINER, *PDUI_CONTAINER;
typedef DuiLib::CChildLayoutUI				DUI_CHILDLAYOUT, *PDUI_CHILDLAYOUT;
typedef DuiLib::CVerticalLayoutUI			DUI_VERTLAYOUT, *PDUI_VERTLAYOUT;
typedef DuiLib::CHorizontalLayoutUI			DUI_HORILAYOUT, *PDUI_HORILAYOUT;
typedef DuiLib::CTileLayoutUI				DUI_TILELAYOUT, *PDUI_TILELAYOUT;
typedef DuiLib::CTabLayoutUI				DUI_TABLAYOUT, *PDUI_TABLAYOUT;

typedef DuiLib::CTextUI						DUI_TEXT, *PDUI_TEXT;
typedef DuiLib::CLabelUI					DUI_LABEL, *PDUI_LABEL;
typedef DuiLib::CButtonUI					DUI_BUTTON, *PDUI_BUTTON;
typedef DuiLib::COptionUI					DUI_OPTION, *PDUI_OPTION;
typedef DuiLib::CCheckBoxUI					DUI_CHECKBOX, *PDUI_CHECKBOX;
typedef DuiLib::CRadioBoxUI					 DUI_RADIOBOX, *PDUI_RADIOBOX;
typedef DuiLib::CComboUI					DUI_COMBO, *PDUI_COMBO;
typedef DuiLib::CDateTimeUI					DUI_DATETIME, *PDUI_DATETIME;
typedef DuiLib::CProgressUI					DUI_PROGRESS, *PDUI_PROGRESS;
typedef DuiLib::CSliderUI					DUI_SLIDER, *PDUI_SLIDER;
typedef DuiLib::CScrollBarUI				DUI_SCROLLBAR, *PDUI_SCROLLBAR;
typedef DuiLib::CEditUI						DUI_EDIT, *PDUI_EDIT;
typedef DuiLib::CRichEditUI					DUI_RICHEDIT, *PDUI_RICHEDIT;
typedef DuiLib::CActiveXUI					DUI_ACTIVEX, *PDUI_ACTIVEX;
typedef DuiLib::CWebBrowserUI				DUI_WEBBROWSER, *PDUI_WEBBROWSER;

typedef DuiLib::CListUI						DUI_LIST, *PDUI_LIST;
typedef DuiLib::CListHeaderUI				DUI_LISTHEADER, *PDUI_LISTHEADER;
typedef DuiLib::CListHeaderItemUI			DUI_LISTHEADERITEM, *PDUI_LISTHEADERITEM;
typedef DuiLib::CListElementUI				DUI_LISTELEM, *PDUI_LISTELEM;
typedef DuiLib::CListLabelElementUI			DUI_LISTLABELELEM, *PDUI_LISTLABELELEM;
typedef DuiLib::CListTextElementUI			DUI_LISTTEXTELEM, *PDUI_LISTTEXTELEM;
typedef DuiLib::CListContainerElementUI		DUI_LISTCONTAINERELEM, *PDUI_LISTCONTAINERELEM;


#endif // _UICRACK_H___
