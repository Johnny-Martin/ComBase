#ifndef __UIWEBBROWSER_H__
#define __UIWEBBROWSER_H__

#pragma once


namespace DuiLib {


	
	//////////////////////////////////////////////////////////////////////////
	//

	/// 此类包装了一个简单的浏览器WebBrowser
	/*
		继承自CActiveXUI，增加两个属性
		-url 浏览器要打开的链接
		-autonavi 浏览器创建成功后，打开url；否则需要手工打开。
		-autosink 是否使用默认的连接点接收器，除去3D边框、右键
		忽略如下属性
		-clsid
		-modulename
	*/

	class UILIB_API CWebBrowserUI : public CActiveXUI
	{
	public:
		/// 构造函数
		CWebBrowserUI();
		virtual ~CWebBrowserUI();

		/// 显示一个URL
		void Navigate2(LPCTSTR lpszUrl = nullptr);

		/// 获得浏览器
		IWebBrowser2* GetWebBrowser();

	protected:
		/// 创建之后，增加连接点，去掉边框和滚动条
		virtual bool DoCreateControl();
		/// 销毁之前，释放连接点
		virtual void ReleaseControl();
		/// 设置属性
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		IWebBrowser2* m_pWebBrowser2; //浏览器指针
		CStdString m_sUrl;
		bool m_bAutoNavi;
		bool m_bAutoSink;
		IUnknown* m_pSinker; //默认接收器，接收IE事件
	};


} // namespace DuiLib

#endif // __UIWEBBROWSER_H__

