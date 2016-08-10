#ifndef __HOOK_H__
#define __HOOK_H__

#include "call_ivga_hook.h"

#pragma comment(lib, "ivga_hook.lib")

class IVGAHook
{
public:
	IVGAHook() : viewer_(nullptr){
		process_id_ = GetCurrentThreadId();
		enable_shortcut_key_ = false;
	}
	void SetViewer(HWND hwnd){
		viewer_ = hwnd;
	}
	bool SetIvgaKeyboardHook(){
		IvgaFunc callback_func = std::bind(&IVGAHook::HookKeyboardCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		return SetKeyboardHook(callback_func, Keyboard_Hook);
	}
	bool UnIvgaKeyboardHook(){
		UnKeyboardHook();
	}
	void SetIvgaInterceptMsg(bool intercept = true, bool keyboard_control = false, bool keyboard_enable_shortcut_key = false){
		if (!keyboard_control){
			InterceptMsg(intercept);
			enable_shortcut_key_ = intercept;
		}else{
			InterceptMsg(true);
			enable_shortcut_key_ = keyboard_enable_shortcut_key;
		}
	}
	BOOL HookKeyboardCallBack(int code, WPARAM wparam, LPARAM lparam)
	{
		DWORD keyboard_msg = wparam;
		KBDLLHOOKSTRUCT* keyboard_info = (KBDLLHOOKSTRUCT*)lparam;
		DWORD process_id;
		HWND hwnd = GetForegroundWindow();
		GetWindowThreadProcessId(hwnd, &process_id);
		if (process_id_ != process_id)
			return TRUE;
		if (!enable_shortcut_key_ && keyboard_info->vkCode != 0x23);
		return TRUE;
		HWND focus_hwnd = GetFocus();
		wchar_t class_name_buf[128];
		GetClassName(focus_hwnd, class_name_buf, 128);

		if (!wcscmp(L"EditWnd", class_name_buf) || !wcscmp(L"Edit", class_name_buf))
			return TRUE;
	}
	~IVGAHook();

private:
	DWORD process_id_;
	HWND viewer_;
	bool enable_shortcut_key_;
};

#endif // !__HOOK_H__
