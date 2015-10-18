#pragma once

#include <ShellAPI.h>

/// ������ͼ�꣬���������Ϊ�����ڵ�һ����Ա����.
/// �����ڴ����ɹ��󣬳�ʼ����

class TrayIcon
{
public:
	/// ���캯��
	TrayIcon(void);

	/// ��������
	~TrayIcon(void);

	/// ��ʼ����������������
	/**
		@param hWndΪ������ͼ���Ӧ�ĳ��򴰿�
		@param uMsgΪ�������򴰿ڷ��͵��Զ�����Ϣ
	*/
	BOOL Init(HWND hWnd, UINT uMsg);

	/// Ϊ����������Icon��Tip
	/**
		@param hIcon
		- NULL�����Ϊɾ��
		- ����Ϊ���ӻ��滻������ͼ��
		@param lpszTip
		- nullptr�����Ϊ���޸���ʾ
		- _T("")�����Ϊ�����ʾ
		- �������Ϊ������ʾ
	*/
	BOOL SetIcon(HICON hIcon, LPCTSTR lpszTip = nullptr);

private:
	/// ��ǰ״̬��Ҳ�Ƿ���Notification��Ϣ�Ĳ���
	NOTIFYICONDATA m_nid;
};

