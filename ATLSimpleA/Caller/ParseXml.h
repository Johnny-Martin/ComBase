#include "stdafx.h"
#include <list>

#ifndef PARSEXML_H
#define PARSEXML_H

using namespace std;
class CXmlParser
{
public:
protected:
private:
};

BOOL CheckFileEncoding(LPCWSTR pszFilePath);

typedef void* (*CreateObjectCallBack)(void);

class CObjectFactory//单例工厂
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
	XML_SUCCESS = 0,
	XML_WRONG_FILENOTEXISTS,
	XML_WRONG_ENCODING_TYPE,
	XML_WRONG_LABELNOTCOMPLETE,
	XML_WRONG_CHILDREN_ID_RECOVER,
	XML_WRONG_CHILDREN_ID_NOTFOUND,
	XML_ERROR_LABELTAIL,
	XML_MISMATCH,
	XML_ERROR_COUNT
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

//<labelClassName attr1="attr1 value" attr2="attr2 value">labelValue</labelClassName>
//UI label must have ID
class XMLabel
{
public:
	XMLabel():m_bLabelClose(false){}

	void SetLabelClassName(const string className){m_labelClassName = className;}
	string GetLabelClassName(){return m_labelClassName;}

	void SetLabelValue(const string value){m_labelValue = value;}
	string GetLabelValue(){ return m_labelValue;}

	void SetLabelId(const string labelId){m_labelId = labelId;}
	string GetLabelId(){return m_labelId;}

	void SetParent(XMLabel* pParent){m_pParent = pParent;}
	XMLabel* GetParent(){return m_pParent;}
	
	XMLERROR AddChild(XMLabel* pChild){
		list<XMLabel>::iterator iter;
		for (iter=m_pChildrenList.begin(); m_pChildrenList.end() != iter; ++iter)
		{
			if ((*iter).GetLabelId() == pChild->GetLabelId())
				return XML_WRONG_CHILDREN_ID_RECOVER;
		}
		m_pChildrenList.push_back(*pChild);
		return XML_SUCCESS;
	}
	XMLERROR RemoveChildById(string childId){
		list<XMLabel>::iterator iter;
		for (iter=m_pChildrenList.begin(); m_pChildrenList.end() != iter; ++iter)
		{
			if ((*iter).GetLabelId() == childId)
			{
				m_pChildrenList.erase(iter);
				//发事件
				return XML_SUCCESS;
			}
		}
		return XML_WRONG_CHILDREN_ID_NOTFOUND;
	}

	XMLERROR SetAttribute(string key, string value){
		m_labelAttrMap.insert(std::pair<string, string>(key, value));
		return XML_SUCCESS;
	}
	string GetAttribute(string key){
		map<string, string>::iterator iter = m_labelAttrMap.find(key);
		if (m_labelAttrMap.end() != iter)
			return (*iter).second;
		
		return NULL;
	}

	void SetLabelClose(bool bClose){m_bLabelClose = bClose;}
	bool GetLabelClose(){ return m_bLabelClose;}
private:
	map<string, string> m_labelAttrMap;
	string m_labelClassName;
	string m_labelValue;
	string m_labelId;
	
	XMLabel* m_pParent;
	list<XMLabel> m_pChildrenList;
	unsigned int m_nodeBeginLineNumber;
	bool m_bLabelClose;
};

class XMLFile
{
public:
	static bool CheckFileEncoding(LPCWSTR pszFilePath);
	XMLERROR ParseXml(LPCWSTR pszFilePath);
	XMLERROR LoadXmlFile(LPCWSTR pszFilePath)
	{
		BOOL ret = ::PathFileExists(pszFilePath);
		if (!ret) 
			return XML_WRONG_FILENOTEXISTS;

		if (!XMLFile::CheckFileEncoding(pszFilePath))
			return XML_WRONG_ENCODING_TYPE;

		return ParseXml(pszFilePath);
	}
protected:
private:
	XMLabel m_rootLabel;//the m_rootObj's id *must* be ""
	
	unsigned int m_curLineNumber;
	string ErrorInfo;
	string ErrorInfoEx;
};

#endif