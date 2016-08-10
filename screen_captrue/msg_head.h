#pragma once

#include "stdafx.h"

enum AppMsg
{
	kAM_ShowTaskMsg = WM_APP + 10,		// 显示托盘图标
	kAM_ExitForPop,			// 托盘右键菜单的退出消息
};