#include "StdAfx.h"
#include "./WebBrowserEventSinker.h"
#include "DocHostUIHandler.h"
#include "ComHelper.h"


/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either 
*  directly or indirectly, the IUnknown interface.
******************************************************************************/ 

/******************************************************************************
*  QueryInterface -- Determines if this component supports the 
*  requested interface, places a pointer to that interface in ppvObj if it is 
*  available, and returns S_OK. If not, sets ppvObj to NULL and returns 
*  E_NOINTERFACE.
******************************************************************************/ 
STDMETHODIMP WebBrowserEventSinker::QueryInterface(REFIID riid, void ** ppvObj)
{
	if (riid == IID_IUnknown){
		*ppvObj = static_cast<IUnknown*>(this);
	}

	else if (riid == IID_IDispatch){
		*ppvObj = static_cast<IDispatch*>(this);
	}

	else if (riid == DIID_DWebBrowserEvents2){
		*ppvObj = static_cast<IDispatch*>(this);
	}

	else{
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppvObj)->AddRef();
	return S_OK;
}

/******************************************************************************
*  AddRef() -- In order to allow an object to delete itself when 
*  it is no longer needed, it is necessary to maintain a count of all 
*  references to this object.  When a new reference is created, this function 
*  increments the count.
******************************************************************************/ 
STDMETHODIMP_(ULONG) WebBrowserEventSinker::AddRef()
{
	return ++m_refCount;
}

/******************************************************************************
*  Release() -- When a reference to this object is removed, this 
*  function decrements the reference count.  If the reference count is 0, 
*  this function deletes this object and returns 0.
******************************************************************************/ 
STDMETHODIMP_(ULONG) WebBrowserEventSinker::Release()
{
	m_refCount--;

	if (m_refCount == 0)
	{
		//释放组件本身
		delete this;
		return 0;
	}
	return m_refCount;
}


/******************************************************************************
*   IDispatch Interface -- This interface allows this class to be used as an
*   Automation server, allowing its functions to be called by other COM
*   objects.
******************************************************************************/ 

/******************************************************************************
*   GetTypeInfoCount -- This function determines if the class supports type 
*   information interfaces or not. It places 1 in iTInfo if the class supports
*   type information and 0 if it does not.
******************************************************************************/ 
STDMETHODIMP WebBrowserEventSinker::GetTypeInfoCount(UINT *iTInfo)
{
	*iTInfo = 0;
	return S_OK;
}

/******************************************************************************
*   GetTypeInfo -- Returns the type information for the class. For classes 
*   that do not support type information, this function returns E_NOTIMPL;
******************************************************************************/ 
STDMETHODIMP WebBrowserEventSinker::GetTypeInfo(UINT iTInfo, LCID lcid, 
	ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}

/******************************************************************************
*   GetIDsOfNames -- Takes an array of strings and returns an array of DISPIDs
*   that correspond to the methods or properties indicated. If the name is not 
*   recognized, it returns DISP_E_UNKNOWNNAME.
******************************************************************************/ 
STDMETHODIMP WebBrowserEventSinker::GetIDsOfNames(REFIID riid,  
	OLECHAR **rgszNames, 
	UINT cNames,  LCID lcid,
	DISPID *rgDispId)
{
	return E_NOTIMPL;
}

/******************************************************************************
*   Invoke -- Takes a dispid and uses it to call another of the methods of this 
*   class. Returns S_OK if the call was successful.
******************************************************************************/ 
STDMETHODIMP WebBrowserEventSinker::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS* pDispParams,
	VARIANT* pVarResult,
	EXCEPINFO* pExcepInfo,
	UINT* puArgErr)
{
	//Validate arguments.
	if ((riid != IID_NULL))
		return E_INVALIDARG;

	//if (dispIdMember != DISPID_STATUSTEXTCHANGE)//过于频繁的不显示了
	//{
	//	TCHAR buf[128] = {0};
	//	_stprintf(buf, _T("WebBrowserEventSinker::Invoke dispId = %d\n"), dispIdMember);
	//	TRACE(buf);
	//}

	HRESULT hr = S_OK;  // Initialize.

	switch (dispIdMember)
	{
		//注意：pDispParams的参数和MFC的参数顺序是相反的，MFC的第一个参数是WebBrowser的this指针
		//The last of pDispParams is an object that evaluates to the top-level or frame WebBrowser object corresponding to the event.

// 	case DISPID_NEWWINDOW3:
// 		// (IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
// 		return E_NOTIMPL;
// 		break;
// 
// 	case DISPID_NEWWINDOW2:
// 		// pDispParams->rgvarg[0]: BOOL*   Cancel
// 		// pDispParams->rgvarg[1]: LPDISPATCH*   ppDisp, dest app
// 		{
// 			int nCount = pDispParams->cArgs;
// 			_variant_t vVal = pDispParams->rgvarg[1];
// 			IWebBrowser2Ptr pWB = vVal.pdispVal;
// 			AdviseWebBrowser(pWB);
// 		}
// 		break;

//	case DISPID_BEFORENAVIGATE2:
		// pDispParams->rgvarg[6]: WebBrowser object
		// pDispParams->rgvarg[5 - 0]: 
		// (LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
//		{
//			int nCount = pDispParams->cArgs;
//			_variant_t vVal = pDispParams->rgvarg[nCount - 2];
//		}
//		break;

	case DISPID_NAVIGATECOMPLETE2:
		//(LPCTSTR lpszURL)
		// pDispParams->rgvarg[0]: URL to navigate to - VT_BYREF|VT_VARIANT
		// pDispParams->rgvarg[1]: An object that evaluates to the top-level or frame WebBrowser object corresponding to the event.
		if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[1].vt == VT_DISPATCH)
			SetCustomDocUIHandler(pDispParams->rgvarg[1].pdispVal);
		break;


	case DISPID_DOCUMENTCOMPLETE:
		//(LPCTSTR lpszURL)
		// pDispParams->rgvarg[0]: URL to navigate to - VT_BYREF|VT_VARIANT
		// pDispParams->rgvarg[1]: An object that evaluates to the top-level or frame WebBrowser object corresponding to the event.
		if (pDispParams->cArgs >= 2 && pDispParams->rgvarg[1].vt == VT_DISPATCH)
		{
			IWebBrowser2Ptr pBrowser;
			(pDispParams->rgvarg[1].pdispVal)->QueryInterface(IID_IWebBrowser2,(void**)&pBrowser);
			if (pBrowser)
			{
				IHTMLDocument2Ptr pDocument;
				IDispatchPtr spDisp;
				pBrowser->get_Document(&spDisp);
				pDocument = spDisp;
				if (pDocument)
				{
					IHTMLElementPtr pElement;
					IHTMLBodyElementPtr pBody; //IID_IHTMLBodyElement
					pDocument->get_body(&pElement);
					pBody = pElement;
					if (pBody != NULL)
					{
						pBody->put_scroll((BSTR)L"auto");

						IHTMLStylePtr pStyle;
						pElement->get_style(&pStyle);
						pStyle->put_border((BSTR)L"none");
						pStyle->put_overflow((BSTR)L"auto");
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return hr;
}


//////////////////////////////////////////////////////////////////////////
//
//  类的非接口函数实现
//
//////////////////////////////////////////////////////////////////////////


WebBrowserEventSinker::WebBrowserEventSinker()
{
	m_refCount           = 0;
	m_pConnectionPoint   = NULL;
	m_dwConnectionCookie = 0;
}

WebBrowserEventSinker::~WebBrowserEventSinker()
{
}

HRESULT WebBrowserEventSinker::AdviseWebBrowser(IWebBrowser2* pWebBrowser2)
{
	//设置连接点
	try
	{
		if (pWebBrowser2)
		{
			IConnectionPointContainerPtr pConnPtContainer;
			IConnectionPointPtr pConnectionPoint;

			// Get IConnectionPointContainer interface for the server.
			HRESULT hr = pWebBrowser2->QueryInterface( IID_IConnectionPointContainer, (void **)&pConnPtContainer );
			if (SUCCEEDED(hr) && pConnPtContainer != NULL)
			{
				// Find the connection point for events that you are interested in.
				hr = pConnPtContainer->FindConnectionPoint( DIID_DWebBrowserEvents2, &pConnectionPoint);
				if (SUCCEEDED(hr) && pConnectionPoint != NULL)
				{
					// Set up advisory connection.
					hr = pConnectionPoint->Advise((IUnknown*)this, &m_dwConnectionCookie);
					if (SUCCEEDED(hr))
					{
						m_pConnectionPoint = pConnectionPoint;
						m_pConnectionPoint->AddRef();
						return S_OK;
					}
				}
			}
		}
	}
	catch (...)
	{
	}
	return E_FAIL;
}

HRESULT WebBrowserEventSinker::UnAdviseWebBrowser()
{
	//If the connection point has been advised, unadvise it and clean up.
	if (m_pConnectionPoint != NULL)
	{
		m_pConnectionPoint->Unadvise(m_dwConnectionCookie);
		m_pConnectionPoint->Release();
		m_pConnectionPoint = NULL;
		m_dwConnectionCookie = 0;
	}

	return S_OK;
}

HRESULT WebBrowserEventSinker::SetCustomDocUIHandler(LPDISPATCH lpDisp)
{
	if(lpDisp == NULL)
		return E_INVALIDARG;

	IWebBrowser2Ptr pWebBrowser2;
	HRESULT hr = lpDisp->QueryInterface(IID_IWebBrowser2, (void**)&pWebBrowser2);

	if(SUCCEEDED(hr) && pWebBrowser2)
	{
		IDispatchPtr pDoc;
		hr = pWebBrowser2->get_Document(&pDoc);

		if(SUCCEEDED(hr) && pDoc)
		{
			ICustomDocPtr pCustDoc;
			hr = pDoc->QueryInterface(IID_ICustomDoc, (void**)&pCustDoc);
			if(SUCCEEDED(hr) && pCustDoc != NULL)
			{				
				IDocHostUIHandler* pUIHandler;
				CreateObjectInterface<CDocHostUIHandler, IDocHostUIHandler>(NULL, &pUIHandler);

				if (pUIHandler)
					pCustDoc->SetUIHandler(pUIHandler);

				pUIHandler->Release();
			}
		}
	}

	return hr;
}
