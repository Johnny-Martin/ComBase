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
			//������ӵ�
			WebBrowserEventSinker * pSinker = nullptr;
			CreateObjectInterface(&pSinker, &m_pSinker);

			if (pSinker)
			{
				pSinker->AdviseWebBrowser(m_pWebBrowser2);
				pSinker->Release();//����ָ���ͷţ��ӿ�ָ�뱣���ڳ�Ա����
			}
		}

		if (m_pWebBrowser2 && m_bAutoNavi)
		{
			//ȡ��3D�߿�GetHostInfo�ڵڶ���ҳ���Լ��Ժ����Ч
			if (m_pSinker)
				Navigate2(_T("about:blank"));

			Navigate2(m_sUrl);
		}

		return true;
	}

	void CWebBrowserUI::ReleaseControl()
	{
		//�ͷ����ӵ�
		if (m_pSinker != nullptr)
		{
			((WebBrowserEventSinker*)m_pSinker)->UnAdviseWebBrowser();
			m_pSinker->Release();
			m_pSinker = nullptr;
		}
		//�ͷ������ָ��
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
