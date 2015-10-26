#include "stdafx.h"

using namespace std;
class CXmlParser
{
public:
protected:
private:
};

BOOL CheckFileEncoding(LPCWSTR pszFilePath);

typedef void* (*CreateObjectCallBack)(void);

class CObjectFactory//µ¥Àý¹¤³§
{
public:
	virtual ~CObjectFactory(){}
	void* CreateObjectByClassName(string strClassName)
	{
		map<string, CreateObjectCallBack>::const_iterator iter = m_classMap.find(strClassName);
		
		if (m_classMap.end() == iter)
			return NULL;
		else
			return iter->second();
	}
	void RegistClass(string strClassName, CreateObjectCallBack callBackFun)
	{
		m_classMap.insert(std::pair<string, CreateObjectCallBack>(strClassName, callBackFun));
	}
	static CObjectFactory& GetFactoryInstance()
	{
		static CObjectFactory m_factoryInstance;
		return m_factoryInstance;
	}

private:
	CObjectFactory(){}
	std::map<string, CreateObjectCallBack> m_classMap;
};
/////////////////CBaseWnd version 0.1 ////////////
// create window object from xml file
// the attribute in xml saved in map as a class member
// the attribute include:
// left, top, right, bottom, leftexp, topexp, rightexp, 
// bottomexp,title, visible, enable,
// topmost, layered, append, blur, maxenable, minenable
// rootobjectid, 
//
class CBaseWnd
{
public:
	static set<string> InitAttrNameMap();
	static set<string> InitEventNameMap();
private:
	
	map<string, string> m_attrMap;
	map<string, string> m_eventMap;

	static set<string> m_attrNameSet;
	static set<string> m_eventNameSet;
};

//set<string> CBaseWnd::InitAttrNameMap()
//
//set<string> CBaseWnd::InitEventNameMap()
