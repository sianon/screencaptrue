#ifndef __BASE_WINDOW_H__
#define __BASE_WINDOW_H__

#include "stdafx.h"

enum UILIB_RESOURCETYPE
{
	UILIB_FILE = 1,				// 来自磁盘文件
	UILIB_ZIP,						// 来自磁盘zip压缩包
	UILIB_RESOURCE,			// 来自资源
	UILIB_ZIPRESOURCE,	// 来自资源的zip压缩包
};

class BaseWindow :
	public CDuiWindowImpl
{
public:
	BaseWindow() {};
	virtual ~BaseWindow() {};

	virtual LRESULT OnInit() { return 0; };						// 窗体初始化
	virtual void OnFinalMessage(HWND hWnd) override;			// 最后的处理，一般是释放资源使用
	virtual void Notify(TNotifyUI& msg) override;				// DuiLib 控件消息路由
	virtual BOOL OnSysClick(TNotifyUI& msg);					// 系统按钮点击响应
	virtual void OnUserClick(const TNotifyUI& msg) {};			// 	用户控件点击响应 -- 用于复写

protected:
	virtual CDuiString GetSkinFolder() { return xml_path_; }
	virtual CDuiString GetSkinFile() { return xml_name_; }
	virtual LPCTSTR GetWindowClassName(void) const { return _T("BaseWindow"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

public:
	virtual UINT GetClassStyle() const override;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;				// WM_ 消息路由处理函数
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	// 处理自定义 WM_ 消息
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetSkinFolder(CDuiString xml_path) { xml_path_ = xml_path; }
	void SetSkinFile(CDuiString xml_name) { xml_name_ = xml_name; }

protected:
	static LPBYTE m_lpResourceZIPBuffer;
	CPaintManagerUI m_PaintManager;
	CDuiString xml_path_;
	CDuiString xml_name_;
	CDuiString win_class_naem_;
};

#endif	//__BASE_WINDOW_H__