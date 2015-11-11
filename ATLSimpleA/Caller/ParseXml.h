#include "stdafx.h"
#include <list>
#include <xstring>

#ifndef PARSEXML_H
#define PARSEXML_H

using namespace std;


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
class CXmlParser
{
public:
protected:
private:
};

BOOL CheckFileEncoding(LPCWSTR pszFilePath);

typedef void* (*CreateObjectCallBack)(void);
typedef std::map<string, CreateObjectCallBack> _ClassMapType;

template <class T>
void* GreateObject()
{
	return (void*) new T();
}

#define REFLECTION_DECLARE_BEGIN() \
	private:\
	const static _ClassMapType::value_type* WINAPI _GetInitValue() {\
	static const _ClassMapType::value_type initValue[] = {

#define REGISTER_CLASS(className) \
	_ClassMapType::value_type(#className, GreateObject<className>),

#define REFLECTION_DECLARE_END() \
	};return initValue; }

class CObjectFactory//singleton object factory
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

	REFLECTION_DECLARE_BEGIN()
		REGISTER_CLASS(CBaseWnd)
	REFLECTION_DECLARE_END()

private:
	CObjectFactory(){
		const _ClassMapType::value_type* initValue = CObjectFactory::_GetInitValue();
		m_classMap = _ClassMapType(initValue, initValue + (sizeof(initValue) / sizeof(_ClassMapType::value_type)));

	}
	std::map<string, CreateObjectCallBack> m_classMap;
};




























































































































































enum XMLERROR{
	XML_SUCCESS = 0,
	XML_WRONG_FILENOTEXISTS,
	XML_WRONG_ENCODING_TYPE,
	XML_WRONG_LABELNOTCOMPLETE,
	XML_WRONG_CHILDREN_ID_RECOVER,
	XML_WRONG_CHILDREN_ID_NOTFOUND,
	XML_ERROR_QUOTES_NOT_MATCH,
	XML_ERROR_LABELHEAD,
	XML_ERROR_LABELTAIL,
	XML_ERROR_COMMENTHEAD,
	XML_ERROR_COMMENTTAIL,
	XML_ERROR_UNEXPECTTAIL,
	XML_ERROR_UNEXPECTEND,
	XML_ERROR_LABELVALUE,
	XML_MISMATCH,
		XML_ERROR_MEMORY,
	XML_ERROR_UNKNOWN,

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
	XMLabel()
		:m_bLabelClose(false)
		,m_bLabelHeadClose(false)
		,m_bLabelTailClose(false)
		,m_pParent(NULL)
	{}

	void SetLabelClassName(const string className){m_labelClassName = className;}
	string GetLabelClassName(){return m_labelClassName;}

	void SetLabelValue(const string value){m_labelValue = value;}
	string GetLabelValue(){ return m_labelValue;}

	void SetLabelId(const string labelId){m_labelId = labelId;}
	string GetLabelId(){return m_labelId;}

	void SetParent(XMLabel* pParent){m_pParent = pParent;}
	XMLabel* GetParent(){return m_pParent;}

	void SetLabelHeadClose(bool bClose){m_bLabelHeadClose = bClose;}
	bool GetLabelHeadClose(){return m_bLabelHeadClose;}

	void SetLabelTailClose(bool bClose){m_bLabelTailClose = bClose;}
	bool GetLabelTailClose(){return m_bLabelTailClose;}
	
	//this few functions should be in UEObject Class
	XMLERROR AddChild(XMLabel* pChild){
		set<XMLabel*>::iterator iter = m_pChildrenSet.find(pChild);

		if (iter != m_pChildrenSet.end()){
				return XML_WRONG_CHILDREN_ID_RECOVER;
		}
		m_pChildrenSet.insert(pChild);
		return XML_SUCCESS;
	}
	XMLERROR RemoveChildById(string childId){
		set<XMLabel*>::iterator iter;
		for (iter=m_pChildrenSet.begin(); m_pChildrenSet.end() != iter; ++iter)
		{
			if ((*iter)->GetLabelId() == childId)
			{
				m_pChildrenSet.erase(iter);
				//·¢ÊÂ¼þ
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
	
	XMLERROR AddAttribute(map<string, string> *paraMap){
		string attrKey;
		string attrValue;
		map<string, string>::iterator iter;
		for (iter=paraMap->begin(); iter!=paraMap->end(); ++iter)
		{
			attrKey = iter->first;
			attrValue = iter->second;
			m_labelAttrMap.insert(std::pair<string, string>(attrKey, attrValue));
		}
		return XML_SUCCESS;
	}
	XMLERROR HandleAttrLabel(){
		XMLabel* attrObj ;//= m_pChildrenSet.find();
		if (NULL == attrObj)
			return XML_SUCCESS;
		if (attrObj->GetLabelClassName() != "attr")
			return XML_ERROR_UNKNOWN;
		//list<XMLabel>::iterator iter = attrObj->
	}
	void SetLabelClose(bool bClose){m_bLabelClose = bClose;}
	bool GetLabelClose(){ return m_bLabelClose;}
private:
	map<string, string> m_labelAttrMap;
	string m_labelClassName;
	string m_labelValue;
	string m_labelId;
	
	XMLabel* m_pParent;
	set<XMLabel*> m_pChildrenSet;
	unsigned int m_nodeBeginLineNumber;
	bool m_bLabelClose;
	bool m_bLabelHeadClose;
	bool m_bLabelTailClose;
};

class XMLFile
{
public:
	static bool CheckFileEncoding(LPCWSTR pszFilePath);
	XMLERROR ParseXml(LPCWSTR pszFilePath, XMLabel** rootLabelObj);
	XMLERROR LoadXmlFile(LPCWSTR pszFilePath)
	{
		BOOL ret = ::PathFileExists(pszFilePath);
		if (!ret) 
			return XML_WRONG_FILENOTEXISTS;

		if (!XMLFile::CheckFileEncoding(pszFilePath))
			return XML_WRONG_ENCODING_TYPE;

		m_wszXmlFileName = PathFindFileNameW(pszFilePath);
		return ParseXml(pszFilePath, &m_pRootLabel);
	}
	XMLabel* GetRootObj(){return m_pRootLabel;}
protected:
private:
	XMLERROR ReadLabelAttrValue(std::ifstream& inFile, string* attrValueOut, XMLabel* labelObj);
	XMLERROR ReadLabelAttrName(std::ifstream& inFile, string* attrNameOut, XMLabel* labelObj);
	XMLERROR ReadLabelAttr(std::ifstream& inFile, XMLabel* labelObj);
	XMLERROR ReadLabelName(std::ifstream& inFile, string* labelNameOut, XMLabel* labelObj);
	XMLERROR ReadLableTail(std::ifstream& inFile, string* tailName);
	XMLERROR ReadComment(std::ifstream& inFile);
	XMLERROR ReadLableHead(std::ifstream& inFile, XMLabel** ppLableObj);
	XMLERROR ReadLableHead(std::ifstream& inFile, XMLabel* labelObj);
	XMLERROR ReadLableValue(std::ifstream& inFile, XMLabel* labelObj);

	XMLabel* m_pRootLabel;//the m_rootObj's id *must* be ""
	
	unsigned int m_curLineNumber;
	string ErrorInfo;
	string ErrorInfoEx;
	wstring m_wszXmlFileName;
};

#endif