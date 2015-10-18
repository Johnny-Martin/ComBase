#pragma once


template <typename C, typename I>
HRESULT CreateObjectInterface(C** ppC, I** ppI)
{
	//参数默认值0
	if (ppC != NULL)	*ppC = NULL;
	if (ppI != NULL)	*ppI = NULL;

	//允许其中一个为空
	if(ppC == NULL && ppI == NULL){
		return E_POINTER;
	};

	//创建对象(构造函数中，计数器应该为0)
	C* pC = new C();
	if (pC == NULL){
		return E_OUTOFMEMORY;
	}
	pC->AddRef();

	//查询接口
	I* pI = NULL;
	pC->QueryInterface(__uuidof(I), (void**)&pI);
	if (pI == NULL){
		pC->Release();
		return E_NOINTERFACE;
	}

	//保存结果，或释放
	if (ppC != NULL)
		*ppC = pC;
	else
		pC->Release();

	if (ppI != NULL)
		*ppI = pI;
	else
		pI->Release();

	return S_OK;
}


namespace DuiLib {


	CRect ParseRect(LPCTSTR lpszText);


} //namespace DuiLib