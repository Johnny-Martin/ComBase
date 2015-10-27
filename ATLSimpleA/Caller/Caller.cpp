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


extern "C" int CCallLua ();

int example_1()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("SampleWnd no bom.xml");
	//doc.Print()
	int errid = static_cast<int> (doc.ErrorID());
	if (0 != errid)
	{
		//std::cout<<"error: "<<
		doc.PrintError();
		return errid;
	}


	return errid;
}

int _tmain(int argc, _TCHAR* argv[])
{
	example_1();
	return 0;
}

