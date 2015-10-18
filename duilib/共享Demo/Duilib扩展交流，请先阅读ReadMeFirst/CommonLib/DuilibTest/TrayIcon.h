#pragma once

#include <ShellAPI.h>

/// 托盘区图标，此类可以作为主窗口的一个成员变量.
/// 主窗口创建成功后，初始化。

class TrayIcon
{
public:
	/// 构造函数
	TrayIcon(void);

	/// 析构函数
	~TrayIcon(void);

	/// 初始化托盘区关联窗口
	/**
		@param hWnd为托盘区图标对应的程序窗口
		@param uMsg为托盘区向窗口发送的自定义消息
	*/
	BOOL Init(HWND hWnd, UINT uMsg);

	/// 为托盘区设置Icon和Tip
	/**
		@param hIcon
		- NULL的情况为删除
		- 其他为增加或替换托盘区图标
		@param lpszTip
		- nullptr的情况为不修改提示
		- _T("")的情况为清空提示
		- 其他情况为设置提示
	*/
	BOOL SetIcon(HICON hIcon, LPCTSTR lpszTip = nullptr);

private:
	/// 当前状态，也是发送Notification消息的参数
	NOTIFYICONDATA m_nid;
};

