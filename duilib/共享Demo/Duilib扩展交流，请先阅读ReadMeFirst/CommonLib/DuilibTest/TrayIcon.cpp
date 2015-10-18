#include "StdAfx.h"
#include "TrayIcon.h"


TrayIcon::TrayIcon(void)
{
	memset(&m_nid, 0, sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uID = 100; /*keep 0-12 no use*/
}


TrayIcon::~TrayIcon(void)
{
	SetIcon(NULL, nullptr); // remove icon from system tray
}

BOOL TrayIcon::Init(HWND hWnd, UINT uMsg)
{
	ASSERT(hWnd != NULL);
	ASSERT(uMsg != NULL || uMsg > WM_USER);

	m_nid.hWnd = hWnd;
	m_nid.uCallbackMessage = uMsg;

	return TRUE;
}

BOOL TrayIcon::SetIcon(HICON hIcon, LPCTSTR lpszTip)
{
	UINT msg;
	m_nid.uFlags = 0;

	// set icon
	if (hIcon)
	{
		msg = m_nid.hIcon ? NIM_MODIFY : NIM_ADD; //modify or add
		m_nid.hIcon = hIcon;
		m_nid.uFlags |= NIF_ICON;
	}
	else
	{
		if (m_nid.hIcon==NULL)
			return TRUE;		// already deleted
		msg = NIM_DELETE;
	}

	// set tip
	if (lpszTip != nullptr)
	{
		_tcsncpy(m_nid.szTip, lpszTip, sizeof(m_nid.szTip));
		m_nid.uFlags |= NIF_TIP;
	}

	// set notify msg
	if (m_nid.uCallbackMessage && m_nid.hWnd)
		m_nid.uFlags |= NIF_MESSAGE;

	// shell notify
	BOOL bRet = ::Shell_NotifyIcon(msg, &m_nid);
	if (msg == NIM_DELETE || !bRet)
		m_nid.hIcon = NULL;	// failed

	return bRet;
}