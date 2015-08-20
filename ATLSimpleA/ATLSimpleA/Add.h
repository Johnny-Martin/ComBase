// Add.h : Declaration of the CAdd

#pragma once
#include "resource.h"       // main symbols

#include "ATLSimpleA_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CAdd

class ATL_NO_VTABLE CAdd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAdd, &CLSID_Add>,
	public IDispatchImpl<IAdd, &IID_IAdd, &LIBID_ATLSimpleALib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CAdd()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ADD)


BEGIN_COM_MAP(CAdd)
	COM_INTERFACE_ENTRY(IAdd)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(Add_I)(LONG paraA, LONG paraB, LONG* result);
	STDMETHOD(CatBSTR)(BSTR paraA, BSTR paraB, BSTR* result);
};

OBJECT_ENTRY_AUTO(__uuidof(Add), CAdd)
