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
// topmost, layered, appwnd, blur, maxenable, minenable
// rootobjectid, 
//
class CBaseWnd
{
public:
	CBaseWnd(){ InitAttrMap(); }
	bool SetAttr(string key, string value);
	bool GetAttr(string key, string* value);
	static bool CheckAttrName(string strName){ return (m_attrNameSet.end() != m_attrNameSet.find(strName)) ? true:false;}
	static bool CheckEventName(string strName){ return (m_attrNameSet.end() != m_eventNameSet.find(strName)) ? true:false;}
private:
	static set<string> InitAttrNameSet();
	static set<string> InitEventNameSet();
	
	static set<string> m_attrNameSet;
	static set<string> m_eventNameSet;

	bool InitAttrMap();
	map<string, string> m_attrMap;
	map<string, string> m_eventMap;	
};

//set<string> CBaseWnd::InitAttrNameMap()
//
//set<string> CBaseWnd::InitEventNameMap()

enum XMLERROR{
	XML_SUCCESS = 0;
	XML_WRONG_ENCODING_TYPE;
	XML_MISMATCH;

};
//handle the special character 
//&lt;	    <
//&gt;	    >
//&amp;	    &
//&apos;	'
//&quot;	"
struct ENTITY{
	const char* pattern;
	unsigned int lenth;
	char value;
};

static const ENTITY entities[]={
	{ "lt",   2, '<' },
	{ "gt",   2, '>' },
	{ "amp",  3, '&' },
	{ "apos", 4, '\''},
	{ "quot", 4, '\"'},
};

class XMLObject
{
public:
protected:
private:
	string objectClass;
	string objectId;
};
class XMLNode
{
public:
	XMLERROR LoadXmlFile();
private:
	static bool CheckFileEncoding(LPCWSTR pszFilePath);

	map<string, string> m_nodeAttrMap;
	string m_nodeName;
	string m_nodeValue;

	XMLNode* m_pParent;
	vector<XMLNode*> m_pChildren;
	unsigned int m_nodeBeginLineNumber£»
};