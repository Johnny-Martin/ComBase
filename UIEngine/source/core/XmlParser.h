#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "stdafx.h"
#include <list>
#include <xstring>
#include "UIFrameWnd.h"

using namespace std;

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
typedef std::map<string, CreateObjectCallBack> _ClassInfoMapType;

template <class T>
void* GreateObject()
{
	return (void*) new T();
}

#define REFLECTION_DECLARE_BEGIN() \
	private:\
	const static _ClassInfoMapType::value_type* WINAPI _GetInitValue() {\
		static const _ClassInfoMapType::value_type initValue[] = {

#define REGISTER_CLASS(className) \
	_ClassInfoMapType::value_type(#className, GreateObject<className>),

#define REFLECTION_DECLARE_END() \
	};m_initialClassInfoArraySize = sizeof(initValue);\
	return initValue; }

//singleton object factory
class CObjectFactory
{
public:
	virtual ~CObjectFactory(){}
	void* CreateObjectByClassName(string strClassName)
	{
		_ClassInfoMapType::const_iterator iter = m_classInfoMap->find(strClassName);
		
		if (m_classInfoMap->end() == iter)
			return NULL;
		else
			return iter->second();
	}
	void RegistClass(string strClassName, CreateObjectCallBack callBackFun)
	{
		m_classInfoMap->insert(std::pair<string, CreateObjectCallBack>(strClassName, callBackFun));
	}
	static CObjectFactory& GetFactoryInstance()
	{
		static CObjectFactory m_factoryInstance;
		return m_factoryInstance;
	}
	
	//the class like CBaseWnd must be declared before CObjectFactory
	REFLECTION_DECLARE_BEGIN()
		REGISTER_CLASS(CBaseWnd)
	REFLECTION_DECLARE_END()

private:
	CObjectFactory(){
		const _ClassInfoMapType::value_type* initValueRet = CObjectFactory::_GetInitValue();
		
		unsigned int y = sizeof(_ClassInfoMapType::value_type);
		//'initValueRet' is a pointer type rather than an array type,so 
		//initValueLen = sizeof(initValueRet) is definitely wrong
		unsigned int initValueLen = m_initialClassInfoArraySize / y;

		//Initialize the m_classInfoMap with a memory address and offset,
		//Compiler do not allowed us initialize a member variable in class declaration,
		//So the REFLECTION_DECLARE_BEGIN micro construct a private member function and return
		//a local static array which can be initialized in compile-time in class declaration.
		m_classInfoMap = new _ClassInfoMapType(initValueRet, initValueRet + initValueLen);

	}
	 _ClassInfoMapType* m_classInfoMap;
	static unsigned int m_initialClassInfoArraySize;
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
								XMLabel();
	void						SetLabelClassName(const string className);
	string						GetLabelClassName();

	void						SetLabelValue(const string value);
	string 					    GetLabelValue();

	void 						SetLabelId(const string labelId);
	string 						GetLabelId();

	void 						SetParent(XMLabel* pParent);
	XMLabel* 					GetParent();

	void 						SetLabelHeadClose(bool bClose);
	bool 						GetLabelHeadClose();

	void 						SetLabelTailClose(bool bClose);
	bool 						GetLabelTailClose();
	
								//this few functions should be in UEObject Class
	XMLERROR 					AddChild(XMLabel* pChild);
	XMLERROR 					RemoveChildById(string childId);

	XMLERROR 					SetAttribute(string key, string value);
	string 						GetAttribute(string key);
	
	XMLERROR					AddAttribute(map<string, string> *paraMap);
	XMLERROR 					HandleAttrLabel();
	void 						SetLabelClose(bool bClose);
	bool 						GetLabelClose();
private:
								//node 'attr' ¡¢ children' and 'eventlist' are all child node.
								//further transformation is needed to get a UIobject from XMLabel object
	map<string, string>			m_labelAttrMap;
	string 						m_labelClassName;
	string 						m_labelValue;
	string 						m_labelId;
	
	XMLabel* 					m_pParent;
	set<XMLabel*> 				m_pChildrenSet;
	unsigned int 				m_nodeBeginLineNumber;
	bool 						m_bLabelClose;
	bool 						m_bLabelHeadClose;
	bool 						m_bLabelTailClose;
};

class XMLFile
{
public:
	static bool 				CheckFileEncoding(LPCWSTR pszFilePath);
	XMLERROR  					ParseXml(LPCWSTR pszFilePath, XMLabel** rootLabelObj);
	XMLERROR  					LoadXmlFile(LPCWSTR pszFilePath);
	XMLabel*  					GetRootObj();
protected:
private:
	XMLERROR  					ReadLabelAttrValue(std::ifstream& inFile, string* attrValueOut, XMLabel* labelObj);
	XMLERROR  					ReadLabelAttrName(std::ifstream& inFile, string* attrNameOut, XMLabel* labelObj);
	XMLERROR  					ReadLabelAttr(std::ifstream& inFile, XMLabel* labelObj);
	XMLERROR  					ReadLabelName(std::ifstream& inFile, string* labelNameOut, XMLabel* labelObj);
	XMLERROR  					ReadLableTail(std::ifstream& inFile, string* tailName);
	XMLERROR  					ReadComment(std::ifstream& inFile);
	XMLERROR  					ReadLableHead(std::ifstream& inFile, XMLabel** ppLableObj);
	XMLERROR  					ReadLableHead(std::ifstream& inFile, XMLabel* labelObj);
	XMLERROR  					ReadLableValue(std::ifstream& inFile, XMLabel* labelObj);

	XMLabel* 					m_pRootLabel;//the m_rootObj's id *must* be ""
	
	unsigned int  				m_curLineNumber;
	string  					ErrorInfo;
	string  					ErrorInfoEx;
	wstring  					m_wszXmlFileName;
};

#endif