// Add.cpp : Implementation of CAdd

#include "stdafx.h"
#include "Add.h"


// CAdd


STDMETHODIMP CAdd::Add_I(LONG paraA, LONG paraB, LONG* result)
{
	// TODO: Add your implementation code here

	*result = paraA + paraB;
	return S_OK;
}

STDMETHODIMP CAdd::CatBSTR(BSTR paraA, BSTR paraB, BSTR* result)
{
	// TODO: Add your implementation code here
	CComBSTR temp(paraA);
	temp.AppendBSTR(paraB);
	*result = temp.Detach();

	return S_OK;
}
