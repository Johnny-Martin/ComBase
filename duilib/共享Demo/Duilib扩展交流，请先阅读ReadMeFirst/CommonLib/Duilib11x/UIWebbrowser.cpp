#include "StdAfx.h"

#include "./UIWebBrowser.h"
#include "./WebBrowserEventSinker.h"
#include "ComHelper.h"

namespace DuiLib {


	//////////////////////////////////////////////////////////////////////////
	//

	CWebBrowserUI::CWebBrowserUI()
	{
		m_clsid = CLSID_WebBrowser;
		m_bAutoNavi = true;
		m_bAutoSink = true;
		m_pWebBrowser2 = nullptr;
		m_pSinker = nullptr;
	}

	CWebBrowserUI::~CWebBrowserUI()
	{
		ReleaseControl();
	}

	void CWebBrowserUI::Navigate2(LPCTSTR lpszUrl)
	{
		if (lpszUrl == nullptr)
			lpszUrl = m_sUrl;

		if (lpszUrl == nullptr || *lpszUrl == 0)
			return;

		if (m_pWebBrowser2)
		{
			VARIANT v;
			::VariantInit(&v);
			v.vt = VT_BSTR;
			v.bstrVal = ::SysAllocString(lpszUrl);
			m_pWebBrowser2->Navigate2(&v, NULL, NULL, NULL, NULL);
			::VariantClear(&v);
		}
	}

	IWebBrowser2* CWebBrowserUI::GetWebBrowser()
	{
		return m_pWebBrowser2;
	}

	bool CWebBrowserUI::DoCreateControl()
	{
		if (!CActiveXUI::DoCreateControl())
			return false;

		GetControl(IID_IWebBrowser2, (void**)&m_pWebBrowser2);

		if (m_pWebBrowser2 && m_bAutoSink)
		{
			//添加连接点
			WebBrowserEventSinker * pSinker = nullptr;
			CreateObjectInterface(&pSinker, &m_pSinker);

			if (pSinker)
			{
				pSinker->AdviseWebBrowser(m_pWebBrowser2);
				pSinker->Release();//对象指针释放，接口指针保留在成员变量
			}
		}

		if (m_pWebBrowser2 && m_bAutoNavi)
		{
			//取消3D边框，GetHostInfo在第二个页面以及以后才有效
			if (m_pSinker)
				Navigate2(_T("about:blank"));

			Navigate2(m_sUrl);
		}

		return true;
	}

	void CWebBrowserUI::ReleaseControl()
	{
		//释放连接点
		if (m_pSinker != nullptr)
		{
			((WebBrowserEventSinker*)m_pSinker)->UnAdviseWebBrowser();
			m_pSinker->Release();
			m_pSinker = nullptr;
		}
		//释放浏览器指针
		if (m_pWebBrowser2 != nullptr)
		{
			m_pWebBrowser2->Release();
			m_pWebBrowser2 = nullptr;
		}

		CActiveXUI::ReleaseControl();
	}

	void CWebBrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("url")) == 0)
		{
			m_sUrl = pstrValue;
		}
		else if (_tcscmp(pstrName, _T("autonavi")) == 0)
		{
			m_bAutoNavi = (_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcscmp(pstrName, _T("autosink")) == 0)
		{
			m_bAutoSink = (_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcscmp(pstrName, _T("clsid")) == 0)
		{
			;//ignore
		}
		else if (_tcscmp(pstrName, _T("modulename")) == 0)
		{
			;//ignore
		}
		else
		{
			CActiveXUI::SetAttribute(pstrName, pstrValue);
		}
	}

} // namespace DuiLib
