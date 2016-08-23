#pragma once

#include "stdafx.h"

enum AppMsg
{
	kAM_Init = WM_APP + 10,
	kAM_ShowTaskMsg,		// 显示托盘图标
	kAM_ExitForPop,			// 托盘右键菜单的退出消息
	kAM_MainForPop,			// 托盘右键菜单 打开主界面
	kAM_BeginForPop,		// 托盘右键菜单 开始
	kAM_EndForPop,			// 托盘右键菜单 停止
};