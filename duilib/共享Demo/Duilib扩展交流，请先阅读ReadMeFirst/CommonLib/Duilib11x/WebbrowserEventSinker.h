#pragma once


class WebBrowserEventSinker : public IDispatch
{
public:

	/***** IUnknown Methods *****/ 
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	/***** IDispatch Methods *****/ 
	STDMETHODIMP GetTypeInfoCount(UINT *iTInfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, 
		ITypeInfo **ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid,  
		OLECHAR **rgszNames, 
		UINT cNames,  LCID lcid,
		DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
		WORD wFlags, DISPPARAMS* pDispParams,
		VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
		UINT* puArgErr);


public:
	WebBrowserEventSinker(void);
	virtual ~WebBrowserEventSinker(void);

	HRESULT AdviseWebBrowser(IWebBrowser2* pWebBrowser);
	HRESULT UnAdviseWebBrowser();

protected:
	HRESULT SetCustomDocUIHandler(LPDISPATCH lpDisp);

	LONG						m_refCount;				//计数器
	IConnectionPoint*			m_pConnectionPoint;		//连接点
	DWORD						m_dwConnectionCookie;	//连接点Cookie
};

