#ifndef __UIWEBBROWSER_H__
#define __UIWEBBROWSER_H__

#pragma once


namespace DuiLib {


	
	//////////////////////////////////////////////////////////////////////////
	//

	/// �����װ��һ���򵥵������WebBrowser
	/*
		�̳���CActiveXUI��������������
		-url �����Ҫ�򿪵�����
		-autonavi ����������ɹ��󣬴�url��������Ҫ�ֹ��򿪡�
		-autosink �Ƿ�ʹ��Ĭ�ϵ����ӵ����������ȥ3D�߿��Ҽ�
		������������
		-clsid
		-modulename
	*/

	class UILIB_API CWebBrowserUI : public CActiveXUI
	{
	public:
		/// ���캯��
		CWebBrowserUI();
		virtual ~CWebBrowserUI();

		/// ��ʾһ��URL
		void Navigate2(LPCTSTR lpszUrl = nullptr);

		/// ��������
		IWebBrowser2* GetWebBrowser();

	protected:
		/// ����֮���������ӵ㣬ȥ���߿�͹�����
		virtual bool DoCreateControl();
		/// ����֮ǰ���ͷ����ӵ�
		virtual void ReleaseControl();
		/// ��������
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		IWebBrowser2* m_pWebBrowser2; //�����ָ��
		CStdString m_sUrl;
		bool m_bAutoNavi;
		bool m_bAutoSink;
		IUnknown* m_pSinker; //Ĭ�Ͻ�����������IE�¼�
	};


} // namespace DuiLib

#endif // __UIWEBBROWSER_H__

