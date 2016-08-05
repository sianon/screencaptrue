/************************************************************************/
//   This file include CResourceZip, CDuiWindowImpl helper class for Duilib.
//							By. Clouderman
//						2012-10-12 13:56:53
// Last modify : 
//		* 2013-1-23 10:55:51  Add CDuiWindowImpl::FindControl, 
//							  CDuiWindowImpl::SendNotify helper.
//      * 2013-1-18 16:08:32  Delete unnecessary message handle.
//		* 2013-1-9 10:45:45   Add CResourceZip::SetStringTableZip, 
//							  CDuiWindowImpl::ReattachDialog.
//		* 2012-12-14 13:59:18 Add CDuiWTLMsgFilter for WTL Project
//		* 2012-10-12 13:56:53 Add CDuiWindowImpl, CResourceZip
/************************************************************************/

#ifndef _UIWND_H___
#define _UIWND_H___

#pragma once


namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
/*
 *	Use in WTL project.
 */
#ifdef __ATLAPP_H__

class CDuiWTLMsgFilter : public WTL::CMessageFilter
{
public:
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return DuiLib::CPaintManagerUI::TranslateMessage(pMsg);
	}
};

#endif


//////////////////////////////////////////////////////////////////////////
/*
 *	CResourceZip
 */
class CResourceZip
{
public:
	static BOOL SetResourceZip(UINT uResID)
	{
		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(uResID), _T("ZIPRES"));
		if( hResource == NULL )
			return FALSE;
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if( hGlobal == NULL ) 
		{
#if defined(WIN32) && !defined(UNDER_CE)
			::FreeResource(hResource);
#endif
			return FALSE;
		}
		dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
		if( dwSize == 0 )
			return FALSE;
		m_lpResourceZIPBuffer = new BYTE[dwSize];
		if (m_lpResourceZIPBuffer != NULL)
		{
			::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
		}
#if defined(WIN32) && !defined(UNDER_CE)
		::FreeResource(hResource);
#endif
		CPaintManagerUI::SetResourceZip(m_lpResourceZIPBuffer, dwSize);
		return TRUE;
	}

	static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = true)
	{
		CPaintManagerUI::SetResourceZip( pstrZip, bCachedResourceZip );
	}

	static void SetStringTableZip(LPCTSTR pstrZip)
	{
		CPaintManagerUI::SetStringTableZip(pstrZip);
	}

    static void FreeResourceZip()
    {
        CPaintManagerUI::Term();
        if ( m_lpResourceZIPBuffer != NULL )
        {
            delete [] m_lpResourceZIPBuffer;
            m_lpResourceZIPBuffer = NULL;
        }
    }

private:
	static LPBYTE m_lpResourceZIPBuffer;
};

__declspec(selectany) LPBYTE CResourceZip::m_lpResourceZIPBuffer = NULL;


//////////////////////////////////////////////////////////////////////////
/*
 *	CDuiWindowImpl
 */
class CDuiWindowImpl :
	public CWindowWnd,
	public INotifyUI,
	public IMessageFilterUI,
	public IDialogBuilderCallback
{
public:
    CPaintManagerUI& GetPaintManager()
    {
        return m_paintManager;
    }

	template<typename T, typename U>
	bool FindControl(T clue, U& pControl)
	{
		pControl = static_cast<U>(GetPaintManager().FindControl(clue));
		return pControl != NULL;
	}

	void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false)
	{
		return GetPaintManager().SendNotify(pControl, pstrMessage, wParam, lParam, bAsync);
	}

    BOOL ReattachDialog()
    {
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &GetPaintManager());
        if ( pRoot != NULL )
        {
            GetPaintManager().AttachDialog(pRoot);
            return TRUE;
        }
        return FALSE;
    }

// Overrideables

	virtual LPCTSTR GetWindowClassName() const
	{
		// must be implemented
		ASSERT(FALSE);
		return static_cast<LPCTSTR>(NULL);
	}

	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	virtual CDuiString GetSkinFile()
	{
		// must be implemented
		ASSERT(FALSE);
		return static_cast<LPCTSTR>(NULL);
	}

	virtual void Notify(TNotifyUI &msg)
	{
		// must be implemented
		ASSERT(FALSE);
	}

	virtual LRESULT OnInit()
	{
		return 0;
	}

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		GetPaintManager().RemovePreMessageFilter(this);
		GetPaintManager().RemoveNotifier(this);
		GetPaintManager().ReapObjects(GetPaintManager().GetRoot());
	}

	virtual CControlUI* CreateControl(LPCTSTR /*pstrClass*/)
	{
		return NULL;
	}

// Message handle

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
        LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg)
		{
		case WM_CREATE:
            lRes = OnCreate(uMsg, wParam, lParam, bHandled);
            break;
		case WM_NCACTIVATE:
            lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCCALCSIZE:
            lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCPAINT:
            lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
            break;
		case WM_NCHITTEST:
            lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
            break;
		case WM_GETMINMAXINFO:
            lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
            break;
		case WM_SIZE:
            lRes = OnSize(uMsg, wParam, lParam, bHandled);
            break;
		default:
            bHandled = FALSE;
		}
		if (bHandled)
			return lRes;

		lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled)
			return lRes;

		if (GetPaintManager().MessageHandler(uMsg, wParam, lParam, lRes))
			return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	virtual LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return OnDefaultKey((UINT)wParam);
			default:
				break;
			}
		}
		return FALSE;
	}

    virtual LRESULT OnDefaultKey(UINT nVirtualKey)
    {
        if (nVirtualKey == VK_RETURN)
        {
            return FALSE;
        }
        else if (nVirtualKey == VK_ESCAPE)
        {
            return TRUE;
        }

        return FALSE;
    }

// On standard windows message

    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        GetPaintManager().Init(m_hWnd);
        GetPaintManager().AddPreMessageFilter(this);

        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &GetPaintManager());
        ASSERT(pRoot && "Load dui resource failed!");
        if (pRoot==NULL)
        {
            MessageBox(NULL,_T("加载资源文件失败！"),_T("Duilib"),MB_OK|MB_ICONERROR);
            ExitProcess(1);
            return 0;
        }
        GetPaintManager().AttachDialog(pRoot);
        GetPaintManager().AddNotifier(this);
        GetPaintManager().SetBackgroundTransparent(TRUE);

        return OnInit();
    }

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = GetPaintManager().GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
		if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) )
		{
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
#endif
		bHandled = FALSE;
		return 0;
	}

    virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( ::IsIconic(*this) )
            bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) )
        {
            RECT rcSizeBox = GetPaintManager().GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top )
            {
                if( pt.x < rcClient.left + rcSizeBox.left )
                    return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right )
                    return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
            {
                if( pt.x < rcClient.left + rcSizeBox.left )
                    return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right )
                    return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if( pt.x < rcClient.left + rcSizeBox.left )
                return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right )
                return HTRIGHT;
        }

        RECT rcCaption = GetPaintManager().GetCaptionRect();

        if( pt.x >= rcClient.left + rcCaption.left 
            && pt.x <  rcClient.right - rcCaption.right 
            && pt.y >= rcCaption.top 
            && pt.y <  rcCaption.bottom ) 
        {
            CControlUI* pControl = static_cast<CControlUI*>(GetPaintManager().FindControl(pt));
            if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                _tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                _tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 )
                return HTCAPTION;
        }

        return HTCLIENT;
    }

    virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;
        lpMMI->ptMaxSize.x		= rcWork.right;
        lpMMI->ptMaxSize.y		= rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }

private:
    CPaintManagerUI m_paintManager;
};

} // namespace Duilib

#endif // _UIWND_H___
