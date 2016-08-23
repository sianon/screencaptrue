#include "audio_adjust_panel.h"
#include "msg_head.h"

BOOL AudioAdjustPanel::SetCurrentProcessMute(BOOL mute)
{
  return true;
}

LRESULT AudioAdjustPanel::OnInit()
{
  return 0;
}

void AudioAdjustPanel::PopupWindow(PPOINT point, bool left_bottom /*= false*/)
{
	int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
	int x = point->x;
	int y = point->y;

	if (x + 70 > srceen_width)
		x -= 70;
	if (y + 124 > srceen_height)
		y -= 124;
	
	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow();
}

LRESULT AudioAdjustPanel::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

void AudioAdjustPanel::OnValueChanged(TNotifyUI &msg, bool& handled)
{
}

void AudioAdjustPanel::OnSelectChanged(TNotifyUI &msg, bool& handled)
{
	if (msg.pSender->GetName() == _T("pop_exit")) {
		::PostMessage(parent_wnd_, kAM_ExitForPop, 0, 0);
	} else if (msg.pSender->GetName() == _T("pop_home")) {
		::PostMessage(parent_wnd_, kAM_MainForPop, 0, 0);
	} else if (msg.pSender->GetName() == _T("pop_begin")) {
		::PostMessage(parent_wnd_, kAM_BeginForPop, 0, 0);
	} else if (msg.pSender->GetName() == _T("pop_end")) {
		::PostMessage(parent_wnd_, kAM_EndForPop, 0, 0);
	}

	static_cast<PDUI_RADIOBOX>(msg.pSender)->Selected(false);
	ShowWindow(SW_HIDE);
}