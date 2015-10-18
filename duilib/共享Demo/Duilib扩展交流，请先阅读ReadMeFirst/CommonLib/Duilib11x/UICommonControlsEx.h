#ifndef __UICOMMONCONTROLSEX_H__
#define __UICOMMONCONTROLSEX_H__

#pragma once

namespace DuiLib {


	/// ����ͨ�ĵ�ѡ��ť�ؼ���ֻ���ǡ������ֽ��
	/// ������COptionUI��ֻ��ÿ��ֻ��һ����ť���ѣ�����Ϊ�գ������ļ�Ĭ�����Ծ�����
	/// <Default name="CheckBox" value="height=&quot;20&quot; align=&quot;left&quot; textpadding=&quot;24,0,0,0&quot; normalimage=&quot;file='sys_check_btn.png' source='0,0,20,20' dest='0,0,20,20'&quot; selectedimage=&quot;file='sys_check_btn.png' source='20,0,40,20' dest='0,0,20,20'&quot; disabledimage=&quot;file='sys_check_btn.png' source='40,0,60,20' dest='0,0,20,20'&quot;"/>

	class UILIB_API CCheckBoxUI : public COptionUI
	{
	public:
		LPCTSTR GetClass() const;

		void SetCheck(bool bCheck);
		bool GetCheck() const;
	};


	/// ��չ�����б��
	/// ����arrowimage����,һ��ͼƬƽ���ֳ�5��,Normal/Hot/Pushed/Focused/Disabled(������source����)
	/// <Default name="ComboBox" value="arrowimage=&quot;file='sys_combobox_btn.png' source='0,0,16,16'&quot; "/>
	class UILIB_API CComboBoxUI : public CComboUI
	{
	public:
		CComboBoxUI();
		LPCTSTR GetClass() const;

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		bool SelectString(LPCTSTR lpszText); //zjie 2010-10-11

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

	protected:
		CStdString m_sArrowImage;
		int        m_nArrowWidth;
	};


	/// ʱ��ѡ��ؼ�
	/// ����arrowimage����,һ��ͼƬƽ���ֳ�5��,Normal/Hot/Pushed/Focused/Disabled(������source����)
	/// <Default name="DateTime" value="arrowimage=&quot;file='sys_datetime_btn.png' source='0,0,16,16'&quot; "/>

	class CDateTimeWnd;

	class UILIB_API CDateTimeUI : public CLabelUI
	{
		friend class CDateTimeWnd;
	public:
		CDateTimeUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void DoEvent(TEventUI& event);
		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
		void PaintBorder(HDC hDC);

		CRect GetClearRect();
		CRect GetArrowRect();
		CRect GetTextRect();

	protected:
		SYSTEMTIME m_sysTime;
		bool       m_bReadOnly;

		CDateTimeWnd* m_pWindow;

	protected:
		CStdString m_sArrowImage; //����ѡ��ť
		int        m_nArrowWidth;
		CStdString m_sClearImage; //���ʱ�䰴ť
		int        m_nClearWidth;
	};


} // namespace DuiLib

#endif // __UICOMMONCONTROLSEX_H__

