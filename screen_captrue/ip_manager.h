#ifndef __IP_MANAGER_H__
#define __IP_MANAGER_H__

#include "base_window.h"
#include "ip_edit_ui.h"
#include "xml_manager.h"
#include "res_singleton.h"
#include "advanced_parent.h"
#include "add_slution_wnd.h"

class IpManager : public BaseWindow
{
public:
	IpManager();
	~IpManager();

	BEGIN_DUICONTROL_CREATE(IpManager)
		DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
		DUICONTROL_CREATE(_T("IpControlLayout"), IpEdit)
	END_DUICONTROL_CREATE()

	virtual CDuiString GetSkinFolder() override{ return _T("skin"); }
	virtual CDuiString GetSkinFile()override{ return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName() const override { return _T("IpManager"); }
	virtual void Notify(TNotifyUI& msg) override;

private:
	virtual LRESULT OnInit() override;
	virtual void OnUserClick(const TNotifyUI& msg) override;

	void OnClickAddPlayBtn();
	void OnClickAdvanced();
	void OnClickDelBtn();
	void OnClickEditBtn();
	void OnClickUpdateBtn();
	BOOL GetPlayInfo(NETSTRUCT &node_info);
	void FlushPlayList();
	void EnumNetName();
	void SetNetName(LPCTSTR net_name);
	void OnSelectPlay(TNotifyUI &msg);
	void StartPlay();
	BOOL ExcuteCommand(LPCTSTR command_lien);               // 执行一个命令
	BOOL ExcuteCommand(vector<CDuiString> command_lien_s);  // 执行一组命令集
	void SetControlEnabled(bool enable);
	CDuiString MakeComLine(vector<CDuiString> prarm);
	LPCTSTR GetNetName();
	void OnClickTestBtn();

	XmlManager* xml_manager_;
	vector<IpEdit*> ip_edit_vector_;

};

#endif//__IP_MANAGER_H__

