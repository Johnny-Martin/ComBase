// Caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "..\ATLSimpleA\ATLSimpleA_i.h"
#include "..\ATLSimpleA\ATLSimpleA_i.c"

using namespace std;
struct Node
{
	int iData;
	Node* pNext;

	Node()
	{
		iData = 0;
		pNext = NULL;
	}
};

Node* Merge(Node* pLinkA, Node* pLinkB)
{
	Node* pNewHead = NULL;
	if (pLinkA->iData > pLinkB->iData)
	{
		pNewHead = pLinkA;
	}else
	{
		pNewHead = pLinkB;
	}

	Node* pTmp = NULL;
	while(pLinkA->pNext != NULL && pLinkB->pNext != NULL)
	{
		if (pLinkA->iData > pLinkB->iData)
		{
			pTmp = pLinkA->pNext;
			pLinkA->pNext = pLinkB;
			pLinkA = pTmp;
		}else
		{
			pTmp = pLinkB->pNext;
			pLinkB->pNext = pLinkA;
			pLinkB = pTmp;
		}
	}

	return pNewHead;
}

Node* CreateOrderedNode(int begin, int end)
{
	Node* pHead = new Node();
	Node* pTmp = pHead;

	while(begin > end)
	{
		pTmp->iData = begin;
		pTmp->pNext = new Node();
		pTmp = pTmp->pNext;

		begin -= 2;
	}

	return pHead;
}

void PrintLink(Node* pLink)
{
	while(pLink->pNext != NULL)
	{
		cout<<pLink->iData<<"  ";
		pLink = pLink->pNext;
	}
	cout<<endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	Node* pLinkA = CreateOrderedNode(10, 1);
	PrintLink(pLinkA);

	Node* pLinkB = CreateOrderedNode(10, 1);
	PrintLink(pLinkB);

	Node* pNewHead = Merge(pLinkA, pLinkB);
	PrintLink(pNewHead);

	//::CoInitialize( NULL );
	////CComPtr<IUnknown> spUnknown;
	//CComPtr<IAdd> spIAdd;
	//CComPtr<IDispatch> spDisp;
	//HRESULT hr = spIAdd.CoCreateInstance(CLSID_Add);
	//ATLASSERT( SUCCEEDED( hr ) );
	//
	//LONG result=0;
	//spIAdd->Add_I(782, 112, &result);

	////BSTR bstrA = ::SysAllocString();
	//CComBSTR bstrA("hello ");
	//CComBSTR bstrB("world!");
	//CComBSTR bstrResult;
	//spIAdd->CatBSTR(bstrA, bstrB, &bstrResult);

	//spIAdd.Release();
	//::CoInitialize( NULL ); 
	return 0;
}

