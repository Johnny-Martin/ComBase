// Caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "..\ATLSimpleA\ATLSimpleA_i.h"
#include "..\ATLSimpleA\ATLSimpleA_i.c"


int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize( NULL );
	//CComPtr<IUnknown> spUnknown;
	CComPtr<IAdd> spIAdd;
	CComPtr<IDispatch> spDisp;
	HRESULT hr = spIAdd.CoCreateInstance(CLSID_Add);
	ATLASSERT( SUCCEEDED( hr ) );
	
	LONG result=0;
	spIAdd->Add_I(782, 112, &result);

	//BSTR bstrA = ::SysAllocString();
	CComBSTR bstrA("hello ");
	CComBSTR bstrB("world!");
	CComBSTR bstrResult;
	spIAdd->CatBSTR(bstrA, bstrB, &bstrResult);

	spIAdd.Release();
	::CoInitialize( NULL ); 
	return 0;
}

