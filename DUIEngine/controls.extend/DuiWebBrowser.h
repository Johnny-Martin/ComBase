#pragma once

#include <DuiActiveX.h>
#include <ExDisp.h>
#include <ExDispid.h>

namespace DuiEngine
{
	struct IWebEvent
	{
		virtual void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel ){}
		virtual void NavigateError(IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel){}
		virtual void NavigateComplete2(IDispatch *pDisp,VARIANT *&url){}
		virtual void ProgressChange(LONG nProgress, LONG nProgressMax){}
		virtual void CommandStateChange(long Command,VARIANT_BOOL Enable){}
		virtual void DocumentComplete(IDispatch *pDisp, VARIANT* &url){}
	};

	class CWebEventDispatch : public IDispatch
	{
	public:
		CWebEventDispatch(IWebEvent *pEventHandler):m_pEventHandler(pEventHandler),m_cRef(1)
		{

		}

		void SetEventHandler(IWebEvent *pEventHandler)
		{
			m_pEventHandler=pEventHandler;
		}
		
		// IUnknown
		STDMETHOD_(ULONG,AddRef)()
		{
			return ++m_cRef;
		}
		STDMETHOD_(ULONG,Release)()
		{
			return --m_cRef;
		}

		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)
		{
			if(IsEqualGUID(riid,__uuidof(IUnknown)) || IsEqualGUID(riid,__uuidof(IDispatch)))
			{
				*ppvObject=this;
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		//IDispatch
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
			/* [out] */ __RPC__out UINT *pctinfo){ return E_NOTIMPL;}

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
			/* [in] */ UINT iTInfo,
			/* [in] */ LCID lcid,
			/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo){ return E_NOTIMPL;}



		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
			/* [in] */ __RPC__in REFIID riid,
			/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
			/* [range][in] */ UINT cNames,
			/* [in] */ LCID lcid,
			/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId){ return E_NOTIMPL;}


		virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID riid,
			/* [in] */ LCID lcid,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS *pDispParams,
			/* [out] */ VARIANT *pVarResult,
			/* [out] */ EXCEPINFO *pExcepInfo,
			/* [out] */ UINT *puArgErr);

	protected:
		IWebEvent *m_pEventHandler;
		ULONG m_cRef;
	};


	class CDuiWebBrowser :	public CDuiActiveX, public CDuiMessageFilter
	{
		DUIOBJ_DECLARE_CLASS_NAME(CDuiWebBrowser, "browser")
	public:
		CDuiWebBrowser(void);
		~CDuiWebBrowser(void);

		IWebBrowser2 * GetIEObject(){return m_pIE;}

		CWebEventDispatch * GetEventDispatch(){return &m_eventDispatch;}

	protected:
		virtual void OnAxActivate(IUnknown *pUnknwn);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		int OnCreate(LPVOID);
		void OnDestroy();

		DUIWIN_BEGIN_MSG_MAP()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
		DUIWIN_END_MSG_MAP()
	protected:
		HRESULT RegisterEventHandler(BOOL inAdvise );


		DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
			DUIWIN_WSTRING_ATTRIBUTE("url",m_strUrl,FALSE)
		DUIWIN_DECLARE_ATTRIBUTES_END()

		CDuiStringW m_strUrl;

		DWORD	m_dwCookie;
		
		CWebEventDispatch	m_eventDispatch;
		CDuiComQIPtr<IWebBrowser2> m_pIE;
	};

}
