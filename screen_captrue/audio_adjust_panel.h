#ifndef RPS_AUDIO_ADJUST_PANEL_H_
#define RPS_AUDIO_ADJUST_PANEL_H_

#include "base_window.h"
#include "auto_hide_wnd.h"

class AudioAdjustPanel : public BaseWindow, public AutoHideWndT<AudioAdjustPanel>
{
public:
  AudioAdjustPanel(){}
  virtual ~AudioAdjustPanel(){}

  BOOL SetCurrentProcessMute(BOOL mute);

  LRESULT OnInit() override;

  DECLARE_DUIWND_INFO(_T("AudioAdjustPanel"), CS_DBLCLKS, _T("audio_adjust_panel.xml"))

  BEGIN_DUIMSG_MAP(AudioAdjustPanel)
    CHAIN_DUIMSG_MAP(AutoHideWndT)
    DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
  END_DUIMSG_MAP()
  
  BEGIN_DUINOTIFY_MAP(AudioAdjustPanel)
    DUINOTIFY_TYPE_HANDLER(DUINOTIFY_VALUECHANGED, OnValueChanged)
    DUINOTIFY_TYPE_HANDLER(DUINOTIFY_SELECTCHANGED, OnSelectChanged)
  END_DUINOTIFY_MAP()

  LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  void OnValueChanged(TNotifyUI &msg, bool& handled);
  void OnSelectChanged(TNotifyUI &msg, bool& handled);
};

#endif // !RPS_AUDIO_ADJUST_PANEL_H_