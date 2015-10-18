#pragma once


template <typename C, typename I>
HRESULT CreateObjectInterface(C** ppC, I** ppI)
{
	//����Ĭ��ֵ0
	if (ppC != NULL)	*ppC = NULL;
	if (ppI != NULL)	*ppI = NULL;

	//��������һ��Ϊ��
	if(ppC == NULL && ppI == NULL){
		return E_POINTER;
	};

	//��������(���캯���У�������Ӧ��Ϊ0)
	C* pC = new C();
	if (pC == NULL){
		return E_OUTOFMEMORY;
	}
	pC->AddRef();

	//��ѯ�ӿ�
	I* pI = NULL;
	pC->QueryInterface(__uuidof(I), (void**)&pI);
	if (pI == NULL){
		pC->Release();
		return E_NOINTERFACE;
	}

	//�����������ͷ�
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