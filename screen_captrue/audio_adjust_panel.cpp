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
	}

	ShowWindow(SW_HIDE);
}