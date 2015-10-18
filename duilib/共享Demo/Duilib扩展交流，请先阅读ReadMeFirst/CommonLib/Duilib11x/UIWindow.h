#pragma once


namespace DuiLib {


/// һ�������˻����¼��Ĵ����࣬������ΪMainFrame/Dialog�Ļ��ࡣ
/**
	CWindowWnd�ഴ���˴��ڣ���������Ϣ���̣�������һЩ��������û����Ӧ��
	һ���������������̳У���д�ܶ���룻һ����Ŀ�������ڣ����кܶ��ظ��Ĵ��롣
	����д������࣬�Լ�����Ŀ�Ĺ������������Ҫ���������صĴ��ڣ���Ȼ���Դ�CWindowWnd�̳С�

	����������������Լ����
	sysbtn_min:��С����ť
	sysbtn_max:��󻯰�ť
	sysbtn_cls:�رհ�ť
	sysbtn_rst:�ָ���ť
	sysbtn_ok:�Ի����ȷ��
	sysbtn_cancel:�Ի����ȡ��
*/

class UILIB_API CWindowUI : public CWindowWnd, public INotifyUI
{
public:
	/// ���캯��
	CWindowUI(void);
	/// ��������
	~CWindowUI(void);

	virtual LPCTSTR GetWindowClassName() const {return _T("WindowWnd");}

	/// ͨ��XML��������
	HWND CreateX(HWND hwndParent, DWORD dwStyle, LPCTSTR lpszXml);
	UINT DoModal(HWND hwndParent, LPCTSTR lpszXml);

	/// ����ͨ����Ϣ
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// ����֪ͨ��Ϣ
	void Notify(TNotifyUI& msg);

	//����Ϊ������Ϣ
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);        //Ϊ����Ԥ���ӿ�
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);         //Ϊ����Ԥ���ӿ�
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);       //Ϊ����Ԥ���ӿ�
	virtual LRESULT OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);     //Ĭ�ϴ���
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);    //Ĭ�ϴ���
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);     //Ĭ�ϴ���
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);          //Ĭ�ϴ���
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); //Ĭ�ϴ���
	virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);       //Ĭ�ϴ���
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);    //Ĭ�ϴ���

public:
	/// ���ƹ�����
	CPaintManagerUI m_PaintManager;

protected:
	/// ����xml�ļ��������Զ��崰��
	CStdString      m_xmlCreate;
};


} // namespace DuiLib
