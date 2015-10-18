#pragma once

#include "TrayIcon.h"

class CMainFrameWnd : public DuiLib::CWindowUI
{
public:
	CMainFrameWnd(void);
	~CMainFrameWnd(void);

	LPCTSTR GetWindowClassName() const { return CLASS_MAINFRAME; }
	UINT    GetClassStyle()      const { return CS_DBLCLKS; }

	void Init();
	void Notify(DuiLib::TNotifyUI& msg);

	//通用消息处理
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//常用消息处理
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);

public:
	TrayIcon m_TryIcon;
};

