#include "stdafx.h"
#include "ParseXml.h"
using namespace std;

static const char HORIZONTAL_TAB		= (char)0x09;
static const char HT = HORIZONTAL_TAB;
static const char LINE_FEED				= (char)0x0a;
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;
static const char CR = CARRIAGE_RETURN;
static const char SPACE_KEY				= (char)0x20;
static const char SP = SPACE_KEY;
////////////////////////////////////////////////////////////////
//UTF-8是一种变长字节编码方式，第一字节高位连续的1的个数代表了该
//字符占得字节数，其余字节皆以10开头
//1字节 0xxxxxxx (ANSI 字符)
//2字节 110xxxxx 10xxxxxx 
//3字节 1110xxxx 10xxxxxx 10xxxxxx 
//........
//6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
//注意: 若一个ANSI编码的文本文件中，全部是英文字符，则其文件编码
//与UTF8 无BOM编码一模一样，这种情况下，也认为是UTF8无BOM编码
bool XMLFile::CheckFileEncoding(LPCWSTR pszFilePath)
{//若是UTF8 无 BOM编码，返回文件句柄，避免后面重复openfile
	//BOOL ret = ::PathFileExists(pszFilePath);
	//if (!ret) return FALSE;

	std::ifstream inFile(pszFilePath, ios::in|ios::binary);
	if (!inFile) return false;
	
	char byteFirst  = 0;
	char byteSecond = 0;
	char byteThird  = 0;
	
	inFile.read(&byteFirst, sizeof(char));
	inFile.read(&byteSecond, sizeof(char));
	inFile.read(&byteThird, sizeof(char));

	if (0xEF == (BYTE)byteFirst && 0xBB == (BYTE)byteSecond && 0xBF == (BYTE)byteThird)
	{//UTF-8 BOM头为文件前三字节：0xEF、0xBB、0xBF
		inFile.close();
		cout<<"UTF-8 带BOM编码"<<endl;
		return false;//UTF8 带 BOM 编码
	}
	
	//将文件扫一遍，整个文件的编码若符合UTF8编码规则，则可认为其为UTF8 无 BOM编码
	inFile.seekg(0);
	char byteTmp = 0;
	BYTE byteCount = 0;
	
	while (!inFile.eof())
	{
		inFile.read(&byteFirst, sizeof(char));
		if ((byteFirst & 0x80) == 0 )
			continue;
		else if ((byteFirst & 0xFC) == 0xFC )
			byteCount = 6;
		else if ((byteFirst & 0xF8) == 0xF8 )
			byteCount = 5;
		else if ((byteFirst & 0xF0) == 0xF0 )
			byteCount = 4;
		else if ((byteFirst & 0xE0) == 0xE0 )
			byteCount = 3;
		else if ((byteFirst & 0xC0) == 0xC0 )
			byteCount = 2;
		else 
		{
			inFile.close();
			return false;
		}
		
		while (--byteCount > 0)
		{
			inFile.read(&byteTmp, sizeof(char));
			if ((byteTmp & 0x80) != 0x80)
			{
				inFile.close();
				return false;
			}
		}
	}

	inFile.close();
	cout<<"UTF-8 无BOM编码"<<endl;
	return true;
}
bool CheckLabelMatch()
{
	return true;
}
bool CompileXml()
{
	//check xml file encoding type, must be UTF-8 without BOM
	//check label match
	return false;
}

set<string> CBaseWnd::m_attrNameSet = CBaseWnd::InitAttrNameSet();
set<string> CBaseWnd::m_eventNameSet = CBaseWnd::InitEventNameSet();

set<string> CBaseWnd::InitAttrNameSet()
{
	set<string> attrNameSet;
	attrNameSet.insert("position");
	attrNameSet.insert("left");
	attrNameSet.insert("top");
	attrNameSet.insert("right");
	attrNameSet.insert("bottom");
	attrNameSet.insert("leftexp");
	attrNameSet.insert("topexp");
	attrNameSet.insert("rightexp");
	attrNameSet.insert("bottomexp");
	attrNameSet.insert("title");
	attrNameSet.insert("visible");
	attrNameSet.insert("enable");
	attrNameSet.insert("topmost");
	attrNameSet.insert("layered");
	attrNameSet.insert("appwnd");
	attrNameSet.insert("blur");
	attrNameSet.insert("minenable");
	attrNameSet.insert("maxenable");
	attrNameSet.insert("rootobjectid");
	
	return attrNameSet;
}
bool CBaseWnd::InitAttrMap()
{
	m_attrMap.insert(pair<string, string>("position", ""));
	m_attrMap.insert(pair<string, string>("left", "0"));
	m_attrMap.insert(pair<string, string>("top", "0"));
	m_attrMap.insert(pair<string, string>("right", "0"));
	m_attrMap.insert(pair<string, string>("bottom", "0"));
	m_attrMap.insert(pair<string, string>("leftexp", ""));
	m_attrMap.insert(pair<string, string>("topexp", ""));
	m_attrMap.insert(pair<string, string>("rightexp", ""));
	m_attrMap.insert(pair<string, string>("bottomexp", ""));
	m_attrMap.insert(pair<string, string>("visible", "1"));
	m_attrMap.insert(pair<string, string>("enable", "1"));
	m_attrMap.insert(pair<string, string>("topmost", "0"));
	m_attrMap.insert(pair<string, string>("layered", "1"));
	m_attrMap.insert(pair<string, string>("appwnd", "1"));
	m_attrMap.insert(pair<string, string>("blur", "0"));
	m_attrMap.insert(pair<string, string>("minenable", "1"));
	m_attrMap.insert(pair<string, string>("maxenable", "1"));
	m_attrMap.insert(pair<string, string>("rootobjectid", ""));
	return true;
}
set<string> CBaseWnd::InitEventNameSet()
{
	set<string> eventNameSet;
	eventNameSet.insert("OnCreate");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnDestory");
	eventNameSet.insert("OnStateChange");
	eventNameSet.insert("OnMove");
	eventNameSet.insert("OnSize");
	eventNameSet.insert("OnVisibleChange");
	eventNameSet.insert("OnEnableChange");
	eventNameSet.insert("OnCreate");

	return eventNameSet;
}
bool CBaseWnd::SetAttr(string key, string value)
{
	if (m_attrNameSet.end() != m_attrNameSet.find(key))
	{
		m_attrMap.insert(pair<string, string>(key, value));
		return true;
	}
	return false;
}
bool CBaseWnd::GetAttr(string key, string* value)
{
	map<string, string>::iterator iter = m_attrMap.find(key);
	if (m_attrMap.end() != iter)
	{
		*value = iter->second;
		return true;
	}
	return false;
}

//a label name end with 0x20(space)、LF、CR、HT、'/>'、or '>'
//read a whole label(until '>'), if match '/>',then close the label;
//handle the label ID and the label attributes
XMLERROR ReadLabelAttrValue(std::ifstream& inFile, string strValue)
{

	return XML_SUCCESS;
}
XMLERROR ReadLabelAttrName(std::ifstream& inFile, string attrName)
{
	
	return XML_SUCCESS;
}
XMLERROR ReadLable(std::ifstream& inFile, XMLabel** ppLableObj)
{
	string ret="";
	char tmp;
	unsigned int count=0;
	string labelName;//label class name
	string attrName;
	string attrValue;
	bool bLabelNameComplete = false;
	bool bLabelClose        = false;
	bool bAttrNameComplete  = false;
	bool bAttrValueComplete = true;
	bool bComplete = false;

#define  LABEL_ID_ENDLESS(tmpChar) (SP != tmp && CR != tmp && HT != tmp && LF != tmp && '/' != tmp)
	
	XMLabel* tmpXmlObj = new XMLabel();
	while (!inFile.eof())
	{
		count++;
		inFile.read(&tmp, sizeof(char));
		if ('>' == tmp )
		{
			bLabelNameComplete = true;
			break;
		}else if ('/' == tmp)
		{
			inFile.read(&tmp, sizeof(char));
			if ('>' == tmp)
			{
				bLabelNameComplete = true;
				bLabelClose = true;
				break;
			}else
			{
				delete tmpXmlObj;
				return XML_ERROR_LABELTAIL;
			}
		}else if (!bLabelNameComplete )
		{
			if (LABEL_ID_ENDLESS(tmp))
				labelName.append(sizeof(char), tmp);
			else
			{
				bLabelNameComplete = true;
				tmpXmlObj->SetLabelClassName(labelName);
			}
		}else if (!bAttrNameComplete)
		{
			if (0 == attrName.length() && !LABEL_ID_ENDLESS(tmp))
				continue;
			else if (LABEL_ID_ENDLESS(tmp) && ('=' != tmp))
				attrName.append(sizeof(char), tmp);
			else
			{
				bAttrNameComplete = true;
			}
		}else if (!bAttrValueComplete)
		{
			if (0 == attrValue.length() && (!LABEL_ID_ENDLESS(tmpChar) || '=' == tmp))
			{
				if ('\"' == tmp)
					bAttrValueComplete = false;
				continue;
			}
			else if (!bAttrValueComplete && LABEL_ID_ENDLESS(tmp))
			{
				if (('\"' != tmp))
					attrValue.append(sizeof(char), tmp);
				else
				{
					bAttrValueComplete = true;
				}
			}
		}
		
		if (bAttrNameComplete && bAttrValueComplete)
		{
			tmpXmlObj->SetAttribute(attrName, attrValue);
			bAttrNameComplete  = false;
			bAttrValueComplete = true;
		}
	}
	
	if (!bLabelNameComplete || bAttrNameComplete || !bAttrValueComplete)
	{
		delete tmpXmlObj;
		return XML_ERROR_LABELTAIL;
	}
	
	tmpXmlObj->SetLabelClose(bLabelClose);
	tmpXmlObj->SetLabelClassName(labelName);
	*ppLableObj = tmpXmlObj;
	return XML_SUCCESS;
}

XMLERROR XMLFile::ParseXml(LPCWSTR pszFilePath)
{
	std::ifstream inXmlFile(pszFilePath, ios::in);
	//Assert(inXmlFile);

	std::stack<XMLabel*> lableObjStack;
	inXmlFile.seekg(0);

	//create a tmp XMLabel and push in lableObjStack, and fill in the obj's attr
	//pop out when match a finish label;
	//and then push the finished obj in XMLFile obj's m_xmlObjVec

	XMLabel* newObj = NULL;
	char tmpChar=0;
	while (!inXmlFile.eof())
	{
		//注释无需压栈
		inXmlFile.read(&tmpChar, sizeof(tmpChar));
		if ('<' == tmpChar)
		{
			
			XMLERROR ret = ReadLable(inXmlFile, &newObj);
			if (XML_SUCCESS == ret && newObj != NULL)
			{

			}
		}
		
	}
	return XML_SUCCESS;
}