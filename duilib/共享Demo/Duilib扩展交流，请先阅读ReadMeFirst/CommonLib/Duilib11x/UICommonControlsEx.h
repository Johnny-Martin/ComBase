#ifndef __UICOMMONCONTROLSEX_H__
#define __UICOMMONCONTROLSEX_H__

#pragma once

namespace DuiLib {


	/// 最普通的单选按钮控件，只有是、否两种结果
	/// 派生于COptionUI，只是每组只有一个按钮而已，组名为空，配置文件默认属性举例：
	/// <Default name="CheckBox" value="height=&quot;20&quot; align=&quot;left&quot; textpadding=&quot;24,0,0,0&quot; normalimage=&quot;file='sys_check_btn.png' source='0,0,20,20' dest='0,0,20,20'&quot; selectedimage=&quot;file='sys_check_btn.png' source='20,0,40,20' dest='0,0,20,20'&quot; disabledimage=&quot;file='sys_check_btn.png' source='40,0,60,20' dest='0,0,20,20'&quot;"/>

	class UILIB_API CCheckBoxUI : public COptionUI
	{
	public:
		LPCTSTR GetClass() const;

		void SetCheck(bool bCheck);
		bool GetCheck() const;
	};


	/// 扩展下拉列表框
	/// 增加arrowimage属性,一张图片平均分成5份,Normal/Hot/Pushed/Focused/Disabled(必须有source属性)
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


	/// 时间选择控件
	/// 增加arrowimage属性,一张图片平均分成5份,Normal/Hot/Pushed/Focused/Disabled(必须有source属性)
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
		CStdString m_sArrowImage; //下拉选择按钮
		int        m_nArrowWidth;
		CStdString m_sClearImage; //清除时间按钮
		int        m_nClearWidth;
	};


} // namespace DuiLib

#endif // __UICOMMONCONTROLSEX_H__

