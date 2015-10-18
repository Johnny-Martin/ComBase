#pragma once

#include <MsHtmHst.h>
#include <MsHTML.h>

class CDocHostUIHandler : public IDocHostUIHandler2, public IOleCommandTarget
{
private:
	LONG m_refCount;	//计数器
public:
	CDocHostUIHandler() : m_refCount(0) {}

public:

	// interface IUnknown

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv)
	{
		if (NULL == ppv){
			return E_POINTER;
		}
		*ppv = NULL;

		if (IID_IUnknown == riid){
			*ppv = static_cast<IUnknown*>(static_cast<IDocHostUIHandler2*>(this));
		}
		else if(IID_IDocHostUIHandler == riid){
			*ppv = static_cast<IDocHostUIHandler*>(static_cast<IDocHostUIHandler2*>(this));
		}
		else if (IID_IDocHostUIHandler2 == riid){
			*ppv = static_cast<IDocHostUIHandler2*>(this);
		}
		else if (IID_IOleCommandTarget == riid){
			*ppv = static_cast<IOleCommandTarget*>(this);
		}
		else{
			ASSERT(0);
			return E_NOINTERFACE;
		}

		static_cast<IUnknown*>(*ppv)->AddRef();
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_refCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
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

	// interface IDocHostUIHandler

	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		if (pInfo != NULL)
		{
			pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
			pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DOUTERBORDER;
			pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO; //maybe need
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ShowUI(
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE HideUI( void)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableModeless(
		/* [in] */ BOOL fEnable)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(
		/* [in] */ BOOL fActivate)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(
		/* [in] */ BOOL fActivate)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder(
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		/* [in] */ BOOL fRameWindow)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
		/* [in] */ DWORD nCmdID)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(
		/* [out] */ LPOLESTR __RPC_FAR *pchKey,
		/* [in] */ DWORD dw)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget(
		/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetExternal(
		/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl(
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject(
		/* [in] */ IDataObject __RPC_FAR *pDO,
		/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
		/* [annotation][out] */ 
		__deref_out  LPOLESTR *pchKey,
		/* [in] */ DWORD dw)
	{
		return E_NOTIMPL;
	}

	// interface IOleCommandTarget

	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE QueryStatus( 
		/* [unique][in] */ const GUID *pguidCmdGroup,
		/* [in] */ ULONG cCmds,
		/* [out][in][size_is] */ OLECMD prgCmds[  ],
		/* [unique][out][in] */ OLECMDTEXT *pCmdText)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE Exec( 
		/* [unique][in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID,
		/* [in] */ DWORD nCmdexecopt,
		/* [unique][in] */ VARIANT *pvaIn,
		/* [unique][out][in] */ VARIANT *pvaOut)
	{
		HRESULT hr = E_NOTIMPL;

		if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
		{

			switch (nCmdID) 
			{

			case OLECMDID_SHOWSCRIPTERROR:
				{
					IHTMLDocument2*             pDoc = NULL;
					IHTMLWindow2*               pWindow = NULL;
					IHTMLEventObj*              pEventObj = NULL;
					BSTR                        rgwszNames[5] = 
					{ 
						SysAllocString(L"errorLine"),
						SysAllocString(L"errorCharacter"),
						SysAllocString(L"errorCode"),
						SysAllocString(L"errorMessage"),
						SysAllocString(L"errorUrl")
					};
					DISPID                      rgDispIDs[5];
					VARIANT                     rgvaEventInfo[5];
					DISPPARAMS                  params;
					BOOL                        fContinueRunningScripts = true;
					int	                        i;

					params.cArgs = 0;
					params.cNamedArgs = 0;

					// Get the document that is currently being viewed.
					hr = pvaIn->punkVal->QueryInterface(IID_IHTMLDocument2, (void **) &pDoc);				
					// Get document.parentWindow.
					hr = pDoc->get_parentWindow(&pWindow);
					// Get the window.event object.
					hr = pWindow->get_event(&pEventObj);
					// Get the error info from the window.event object.
					for (i = 0; i < 5; i++) 
					{  
						// Get the property's dispID.
						hr = pEventObj->GetIDsOfNames(IID_NULL, &rgwszNames[i], 1, 
							LOCALE_SYSTEM_DEFAULT, &rgDispIDs[i]);
						// Get the value of the property.
						hr = pEventObj->Invoke(rgDispIDs[i], IID_NULL,
							LOCALE_SYSTEM_DEFAULT,
							DISPATCH_PROPERTYGET, &params, &rgvaEventInfo[i],
							NULL, NULL);
						SysFreeString(rgwszNames[i]);
					}

					// At this point, you would normally alert the user with 
					// the information about the error, which is now contained
					// in rgvaEventInfo[]. Or, you could just exit silently.

					(*pvaOut).vt = VT_BOOL;
					if (fContinueRunningScripts)
					{
						// Continue running scripts on the page.
						(*pvaOut).boolVal = VARIANT_TRUE;
					}
					else
					{
						// Stop running scripts on the page.
						(*pvaOut).boolVal = VARIANT_FALSE;			
					}

					pDoc->Release();

					hr = S_OK;

					break;
				}

			case OLECMDID_SHOWMESSAGE:
				break;

			default:
				hr = OLECMDERR_E_NOTSUPPORTED;
				break;
			}
		}
		else
		{
			hr = OLECMDERR_E_UNKNOWNGROUP;
		}

		return (hr);
	}
};

class CDocHostShowUI : public IDocHostShowUI
{
private:
	LONG m_refCount;	//计数器
public:
	CDocHostShowUI() : m_refCount(0) {}

public:

	// interface IUnknown

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv)
	{
		if (NULL == ppv){
			return E_POINTER;
		}
		*ppv = NULL;

		if (IID_IUnknown == riid){
			*ppv = static_cast<IUnknown*>(static_cast<IDocHostShowUI*>(this));
		}
		else if (IID_IDocHostShowUI == riid){
			*ppv = static_cast<IDocHostShowUI*>(this);
		}
		else{
			return E_NOINTERFACE;
		}

		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_refCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
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

	// interface IDocHostShowUI

	virtual HRESULT STDMETHODCALLTYPE ShowMessage( 
		/* [in] */ HWND hwnd,
		/* [in] */ LPOLESTR lpstrText,
		/* [in] */ LPOLESTR lpstrCaption,
		/* [in] */ DWORD dwType,
		/* [in] */ LPOLESTR lpstrHelpFile,
		/* [in] */ DWORD dwHelpContext,
		/* [out] */ LRESULT *plResult)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ShowHelp( 
		/* [in] */ HWND hwnd,
		/* [in] */ LPOLESTR pszHelpFile,
		/* [in] */ UINT uCommand,
		/* [in] */ DWORD dwData,
		/* [in] */ POINT ptMouse,
		/* [out] */ IDispatch *pDispatchObjectHit)
	{
		return E_NOTIMPL;
	}
};