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
		//CComObject objA;
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

class BaseClass
{
public:
	virtual void VFun()
	{
		cout<<"BaseClass VFun"<<endl;
	}
};
class DirveClass: public BaseClass
{
public:
	virtual void VFun()
	{
		cout<<"DirveClass VFun"<<endl;
	}
};
class FinalClass
{
private:
	FinalClass()
	{
		cout<<"FinalClass";
	}
};

class  TryClass//: public FinalClass
{
public:
	int a;
};