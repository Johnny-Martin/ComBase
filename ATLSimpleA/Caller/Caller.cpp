// Caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "template.h"
#include <vector>
#include <iostream>
#include "..\ATLSimpleA\ATLSimpleA_i.h"
#include "..\ATLSimpleA\ATLSimpleA_i.c"
#include <windows.h>
//#include "lua_and_c.h"
#include "ParseXml.h"
#include "tinyxml2.h"
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
union {
	char a;
	unsigned int i;
}u;

int g_i = 10;  //一个全局变量
CRITICAL_SECTION cs;  //一个临界区变量
DWORD WINAPI ThreadProc(LPVOID lpv)
{
	::EnterCriticalSection(&cs);
	g_i += 10;
	std::cout <<"In the Thread " << ::GetCurrentThreadId() << ",the first g_i is "  <<  g_i  <<  "!"  << std::endl;
	
	::LeaveCriticalSection(&cs);

	Sleep(5000); //睡眠
	
	::EnterCriticalSection(&cs);
	g_i += 10;
	std::cout <<"In the Thread " << ::GetCurrentThreadId() << ",the secend g_i is "  <<  g_i  << "!" << std::endl;
	::LeaveCriticalSection(&cs);

	return 0;
	
}

void CreateMyThread()
{
	DWORD threadID[2];
	HANDLE hThreads[2];
	InitializeCriticalSection(&cs);

	for(int i = 0; i <= 1; i++ )         //创建两个线程
		hThreads[i] = ::CreateThread(NULL, 0,ThreadProc,NULL, 0, &threadID[i]);

	WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);   //等待线程结束

	for(int i = 0; i <= 1; i++ )
		::CloseHandle(hThreads[i]);             //关闭线程句柄
	system("pause");
}

extern "C" int CCallLua ();

int example_1()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("SampleWnd ansi.xml");
	//doc.Print()
	int errid = static_cast<int> (doc.ErrorID());
	if (0 != errid)
	{
		//std::cout<<"error: "<<
		doc.PrintError();
	}
	return errid;
}

//void* GreateModalWndObject()
//{
//	return (void*)new CBaseWnd();
//}
//
//void* GreateBaseWndObject()
//{
//	return (void*)new CBaseWnd();
//}


int _tmain(int argc, _TCHAR* argv[])
{
	{
		//注册
		/*CObjectFactory& factortInstance = CObjectFactory::GetFactoryInstance();
		factortInstance.RegistClass("CModalWnd", GreateModalWndObject);
		factortInstance.RegistClass("CBaseWnd", GreateBaseWndObject);

		CBaseWnd* tmpWnd = (CBaseWnd*)factortInstance.CreateObjectByClassName("CBaseWnd");
		tmpWnd->strWndID;*/
	}
	//example_1();
	//CCallLua ();
	//LuaCallC();
	/*DirveClass D1;
	BaseClass B1 = D1;

	BaseClass &B2 = D1;
	BaseClass *B3 = &D1;

	B1.VFun();
	B2.VFun();
	B3->VFun();

	TryClass a;*/
	//BaseClass * pB;
	//CreateMyThread();

// 	Drive d1;
// 	d1.Print();
// 
// 	Drive2 d2;
// 	d2.Print();
	/*u.i = 0xf0f1f2f3;
	cout<<hex<<u.i<<endl;
	cout<<hex<<(int)u.a<<endl;
	Node* pLinkA = CreateOrderedNode(10, 1);
	PrintLink(pLinkA);

	Node* pLinkB = CreateOrderedNode(10, 1);
	PrintLink(pLinkB);

	Node* pNewHead = Merge(pLinkA, pLinkB);
	PrintLink(pNewHead);*/

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
	//cout<<CheckFileEncoding(_T("F:\\code\\ComBase\\trunk\\ATLSimpleA\\Caller\\test no bom.lua"))<<endl;
	cout<<CheckFileEncoding(_T("F:\\code\\ComBase\\trunk\\ATLSimpleA\\Caller\\test bom.lua"))<<endl;
	cout<<CheckFileEncoding(_T("F:\\code\\ComBase\\trunk\\ATLSimpleA\\Caller\\test ansi.lua"))<<endl;
	return 0;
}

