#include "stdafx.h"
#include <iostream>
#include <atlbase.h>
#include <atlcom.h>
using namespace std;

template <class T>
class Base
{
public:
	void m_print()
	{
		cout<<"Base::m_print"<<endl;
	}

	void Print()
	{
		static_cast<T*>(this)->m_print();
		CComObject objA;
	}

};

class Drive: public Base<Drive>
{
public:
	void m_print()
	{
		cout<<"Drive::Print"<<endl;
	}
};
class Drive2: public Base<Drive2>
{
public:
};