#ifndef RPS_AUTO_HIDE_WND_H_
#define RPS_AUTO_HIDE_WND_H_

template<class T>
class AutoHideWndT
{
protected:
  HWND parent_wnd_;

public:
  AutoHideWndT() : parent_wnd_(0) {}
  virtual ~AutoHideWndT(){}

	HWND CreateWithStyle(HWND parent, DWORD style  )
	{
		parent_wnd_ = parent;
		return static_cast<T *>(this)->Create(
			parent,
			nullptr,
			style, 
			0);
	}

  HWND CreateWithDefaultStyle(HWND parent) 
  {
    parent_wnd_ = parent;

    return static_cast<T *>(this)->Create(
      parent,
      nullptr,
      WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
  }

  HWND CreateWithStyleAndSize(HWND parent, 
	  LPCTSTR pstrName, 
	  DWORD dwStyle, 
	  DWORD dwExStyle, 
	  int x = CW_USEDEFAULT, 
	  int y = CW_USEDEFAULT, 
	  int cx = CW_USEDEFAULT, 
	  int cy = CW_USEDEFAULT
	  )
  {
	  parent_wnd_ = parent;
	  return static_cast<T *>(this)->Create(
		  parent,
		  pstrName,
		  WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		  WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		  x, y, cx, cy);
  }

	void PopupWindow(PDUI_CONTROL refer, bool left_bottom =  false )
  {
		RECT rcCtrl = refer->GetPos();
		CWindow(parent_wnd_).ClientToScreen(&rcCtrl);
		HWND hWnd = static_cast<T *>(this)->GetHWND();
		RECT rcSelf;
		CWindow(hWnd).GetWindowRect(&rcSelf);
		int width = rcSelf.right - rcSelf.left;
		int height = rcSelf.bottom - rcSelf.top;
		int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
		int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
		int x, y;
		if (!left_bottom)
		{
			if (rcCtrl.right + width > srceen_width && rcCtrl.bottom + height > srceen_height)
			{
				x = rcCtrl.right - width;
				y = rcCtrl.bottom - height;
			} else if (rcCtrl.right + width > srceen_width)
			{
				x = rcCtrl.right - width;
				y = rcCtrl.bottom;
			} else if (rcCtrl.bottom + height > srceen_height)
			{
				x = rcCtrl.right;
				y = rcCtrl.bottom - height;
			} else
			{
				x = rcCtrl.right;
				y = rcCtrl.bottom;
			}
		} else
		{
			if (rcCtrl.left + width > srceen_width && rcCtrl.bottom + height > srceen_height)
			{
				x = rcCtrl.left - width;
				y = rcCtrl.bottom - height;
			} else if (rcCtrl.left + width > srceen_width)
			{
				x = rcCtrl.left - width;
				y = rcCtrl.bottom;
			} else if (rcCtrl.bottom + height > srceen_height)
			{
				x = rcCtrl.left;
				y = rcCtrl.bottom - height;
			} else
			{
				x = rcCtrl.left;
				y = rcCtrl.bottom;
			}
		}
		::SetWindowPos(hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		static_cast<T *>(this)->ShowWindow();
	}

	void PopupWindow(PPOINT point, bool left_bottom = false)
	{
		HWND hWnd = static_cast<T *>(this)->GetHWND();

		int x, y;
		x = point->x;
		y = point->y;
		::SetWindowPos(hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		static_cast<T *>(this)->ShowWindow();
	}

  BEGIN_DUIMSG_MAP(AutoHideWndT)
    DUIMSG_HANDLER(WM_KEYDOWN, OnKeyDown)
    DUIMSG_HANDLER(WM_KILLFOCUS, OnKillFocus)
  END_DUIMSG_MAP()

  LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    if (wParam == VK_ESCAPE) {
      static_cast<T *>(this)->ShowWindow(SW_HIDE);
    }
    bHandled = TRUE;
    return 0;
  }
  LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
  {
    static_cast<T *>(this)->ShowWindow(SW_HIDE);
    bHandled = FALSE;
    return 0;
  }
};

#endif // !RPS_AUTO_HIDE_WND_H_