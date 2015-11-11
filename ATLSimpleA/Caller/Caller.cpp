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
//#include "tinyxml2.h"
using namespace std;
//using namespace tinyxml2;

extern "C" int CCallLua ();

/*
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

	int v0 = 0;
	int v1 = 0;
	XMLElement* rootElement = doc.FirstChildElement();
	XMLElement* windowElement = rootElement->FirstChildElement("window");
	XMLElement* wndAttrElement = windowElement->FirstChildElement("attr");
	//traverse all child node about attr node
	//class CBaseWnd should have the ability of traversing all attribute in map;
	string attrName;
	string attrValue;
	CBaseWnd tmpWnd;
	for( XMLNode* ele = wndAttrElement->FirstChild(); ele; ele = ele->NextSibling())
	{
		attrName = ((XMLElement*)ele)->Value();
		attrValue = ((XMLElement*)ele)->GetText();
		if (CBaseWnd::CheckAttrName(attrName))
		{
			tmpWnd.SetAttr(attrName, attrValue);
		}
	}
	
	//cout<<tmpWnd.GetAttr()
	XMLElement* attrLayeredElement = wndAttrElement->FirstChildElement("layered");
	attrLayeredElement->QueryIntText( &v0 );

	//XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement( "textApproach" );
	//textApproachElement->FirstChildElement( "v" )->QueryIntText( &v1 );

	printf( "Both values are the same: %d and %d\n", v0, v1 );

	return errid;   be able to parse xml,but do not support self-closed label
}*/

void* GreateBaseWndObject()
{
	return (void*)new CBaseWnd();
}

void example_3()
{
	//register class information
	CObjectFactory& factortInstance = CObjectFactory::GetFactoryInstance();
	factortInstance.RegistClass("CBaseWnd", GreateBaseWndObject);

	CBaseWnd* tmpWnd = (CBaseWnd*)factortInstance.CreateObjectByClassName("CBaseWnd");
	
}
void example_2()
{
	XMLFile xmlFile;
	XMLERROR ret = xmlFile.LoadXmlFile(_T("SampleWnd no bom.xml"));
	if (XML_SUCCESS == ret)
	{
		XMLabel* rootObj = xmlFile.GetRootObj();
		cout<<rootObj->GetLabelClassName()<<endl;

		//XMLabel* child = rootObj->
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//example_1();
	example_2();

	return 0;
}

