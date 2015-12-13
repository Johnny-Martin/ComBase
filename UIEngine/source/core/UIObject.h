#ifndef UIOBJECT_H
#define UIOBJECT_H
#include "stdafx.h"

using namespace std;
class LayoutObject
{
public:
	LayoutObject(){ InitAttrMap(); }
	bool SetAttr(string key, string value);
	bool GetAttr(string key, string* value);
	bool CheckAttrName(string strName);
	bool CheckEventName(string strName);
private:
	//static set<string> InitAttrNameSet();
	static set<string> InitEventNameSet();
	static set<string> m_eventNameSet;

	bool InitAttrMap();
	//bool InitEventMap();
	map<string, string> m_attrMap;
	map<string, string> m_eventMap;	
};

#endif