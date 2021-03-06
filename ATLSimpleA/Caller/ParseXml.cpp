#include "stdafx.h"
#include "ParseXml.h"
using namespace std;

unsigned int CObjectFactory::m_initialClassInfoArraySize = 0;
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

#define  LABEL_ID_ENDLESS(tmpChar) (SP != tmpChar && CR != tmpChar && HT != tmpChar && LF != tmpChar && '/' != tmpChar)
#define  IS_LETTER(tmpChar) (tmpChar >= 'A' && tmpChar <= 'z')
#define  IS_SPACE_LETTER(tmpChar) (CR == tmpChar || LF == tmpChar)

XMLERROR XMLFile::ReadLabelAttrValue(std::ifstream& inFile, string* attrValueOut, XMLabel* labelObj)
{
	char tmp;
	string attrValue;

	bool bAttrValueComplete = false;
	bool bAttrValueBegin    = false;
	bool bAttrValueEnd      = false;

	XMLabel* tmpXmlObj = new XMLabel();
	while (!inFile.eof())
	{
		inFile.read(&tmp, sizeof(char));
		if ('>' == tmp || '/' == tmp)
		{   //attrValue must end by '\"'
			return XML_ERROR_UNEXPECTTAIL;

		}else if (!bAttrValueComplete)
		{
			if (0 == attrValue.length() && (!LABEL_ID_ENDLESS(tmp) || '=' == tmp))
			{
				continue;
			}else if ('\"' == tmp && !bAttrValueBegin)
			{
				bAttrValueBegin = true;
				continue;
			}
			else if (bAttrValueBegin && !bAttrValueComplete && LABEL_ID_ENDLESS(tmp))
			{
				if (('\"' != tmp))
					attrValue.append(sizeof(char), tmp);
				else
				{
					bAttrValueComplete = true;
					bAttrValueEnd = true;
					break;
				}
			}
		}

		if (bAttrValueComplete)
			if (!bAttrValueBegin || !bAttrValueEnd)
				return XML_ERROR_QUOTES_NOT_MATCH;
	}

	if (!bAttrValueComplete)
		return XML_ERROR_LABELVALUE;

	*attrValueOut = attrValue;
	return XML_SUCCESS;
}
XMLERROR XMLFile::ReadLabelAttrName(std::ifstream& inFile, string* attrNameOut, XMLabel* labelObj)
{
	char tmp;
	string attrName;
	bool bAttrNameComplete  = false;

	while (!inFile.eof())
	{
		inFile.read(&tmp, sizeof(char));
		if ('>' == tmp)
		{
			labelObj->SetLabelHeadClose(true);
			if (attrName.length() > 0)
				return XML_ERROR_UNEXPECTTAIL;
			else
				return XML_SUCCESS;
			
		}else if ('/' == tmp)
		{
			inFile.read(&tmp, sizeof(char));
			if ('>' != tmp)
				return XML_ERROR_UNEXPECTTAIL;

			labelObj->SetLabelHeadClose(true);
			labelObj->SetLabelClose(true);
			if (attrName.length() > 0)
				return XML_ERROR_UNEXPECTTAIL;
			else
				return XML_SUCCESS;
		}else if (!bAttrNameComplete)
		{
			if (0 == attrName.length() && !LABEL_ID_ENDLESS(tmp))
				continue;
			else if (LABEL_ID_ENDLESS(tmp) && ('=' != tmp))
				attrName.append(sizeof(char), tmp);
			else
			{
				bAttrNameComplete = true;
				break;
			}
		}	
	}
	if (!bAttrNameComplete )
		return XML_ERROR_UNEXPECTEND;

	*attrNameOut = attrName;
	return XML_SUCCESS;
}
XMLERROR XMLFile::ReadLabelAttr(std::ifstream& inFile, XMLabel* labelObj)
{
	string attrName;
	string attrValue;
	XMLERROR readAttrNameRet;
	XMLERROR readAttrValueRet;
	map<string, string> attrMap;
	bool bHeadClose= false;
	while(!inFile.eof())
	{
		attrName = "";
		attrValue = "";	
		readAttrNameRet = ReadLabelAttrName(inFile, &attrName, labelObj);
		if (readAttrNameRet != XML_SUCCESS)
			return readAttrNameRet;
		
		bHeadClose = labelObj->GetLabelHeadClose();
		if (bHeadClose)
		{
			if (0 == attrName.length())
			{
				labelObj->AddAttribute(&attrMap);
				return XML_SUCCESS;
			}
			else
				return XML_ERROR_UNKNOWN;
		}

		readAttrValueRet = ReadLabelAttrValue(inFile, &attrValue, labelObj);
		if (readAttrValueRet != XML_SUCCESS)
			return readAttrValueRet;

		bHeadClose = labelObj->GetLabelHeadClose();
		if (bHeadClose)
		{
			if (0 == attrValue.length())
			{
				labelObj->AddAttribute(&attrMap);
				return XML_SUCCESS;
			}
			else
				return XML_ERROR_UNKNOWN;
		}

		if (attrName.length() > 0 && attrValue.length() > 0)
			attrMap.insert(pair<string, string>(attrName, attrValue));
	}

	return XML_SUCCESS;
}
XMLERROR XMLFile::ReadLabelName(std::ifstream& inFile, string* labelNameOut, XMLabel* labelObj)
{
	string labelName;//label class name
	bool bLabelHeadClose = false;
	bool bLabelClose     = false;
	char tmp = 0;

	bool bHeadClose = labelObj->GetLabelHeadClose();
	if (bHeadClose)
		return XML_ERROR_UNKNOWN;

	while (!inFile.eof())
	{
		inFile.read(&tmp, sizeof(char));
		if ('>' == tmp )
		{
			bLabelHeadClose = true;
			break;
		}else if ('/' == tmp)
		{
			inFile.read(&tmp, sizeof(char));
			if ('>' == tmp)
			{
				bLabelHeadClose = true;
				bLabelClose = true;
				break;
			}else
				return XML_ERROR_LABELTAIL;

		}else if (!bLabelHeadClose )
		{
			if (LABEL_ID_ENDLESS(tmp))
				labelNameOut->append(sizeof(char), tmp);
			else
				break;
		}
	}
	//*labelNameOut = labelName;
	labelObj->SetLabelHeadClose(bLabelHeadClose);
	labelObj->SetLabelClose(bLabelClose);

	return XML_SUCCESS;
}
XMLERROR XMLFile::ReadLableTail(std::ifstream& inFile, string* tailName)
{
	string labelTailName;//label class name
	bool bLabelTailClose = false;
	char tmp = 0;

	while (!inFile.eof())
	{
		inFile.read(&tmp, sizeof(char));
		if ('>' == tmp )
		{
			bLabelTailClose = true;
			break;
		}else
		{
			if (IS_LETTER(tmp))
				tailName->append(sizeof(char), tmp);
		}
	}

	if (!bLabelTailClose)
		return XML_ERROR_LABELTAIL;
	
	return XML_SUCCESS;
}
XMLERROR XMLFile::ReadComment(std::ifstream& inFile)
{
	char commentHeadA;
	char commentHeadB;
	inFile.read(&commentHeadA, sizeof(char));
	inFile.read(&commentHeadB, sizeof(char));

	string comment;
	if (commentHeadA != '-' || commentHeadB != '-')
		return XML_ERROR_COMMENTHEAD;
	char commentChar;
	while (!inFile.eof())
	{
		inFile.read(&commentChar, sizeof(char));

		if ('-' == commentChar)
		{
			char commentTailA;
			char commentTailB;
			inFile.read(&commentTailA, sizeof(char));
			inFile.read(&commentTailB, sizeof(char));
			
			
			if ('-' == commentTailA && '>' == commentTailB)
			{
				int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, comment.c_str(), -1, NULL, 0);
				LPWSTR wComment = new WCHAR[unicodeLen + 1];
				MultiByteToWideChar(CP_UTF8, 0, comment.c_str(), -1, wComment, unicodeLen);

				return XML_SUCCESS;
			}
			else
			{
				inFile.seekg(-2, ios::cur);
				comment.append(sizeof(char), commentChar);
			}
		}else if ('>' == commentChar)
			return XML_ERROR_COMMENTTAIL;
		else
			comment.append(sizeof(char), commentChar);
	}
	return XML_ERROR_UNKNOWN;
}
XMLERROR XMLFile::ReadLableHead(std::ifstream& inFile, XMLabel** ppLableObj)
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
	bool bAttrValueComplete = false;
	bool bAttrValueBegin    = false;
	bool bAttrValueEnd      = false;
	bool bComplete = false;
	
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
			if (0 == attrValue.length() && (!LABEL_ID_ENDLESS(tmp) || '=' == tmp))
			{
				continue;
			}else if ('\"' == tmp && !bAttrValueBegin)
			{
				bAttrValueBegin = true;
				continue;
			}
			else if (bAttrValueBegin && !bAttrValueComplete && LABEL_ID_ENDLESS(tmp))
			{
				if (('\"' != tmp))
					attrValue.append(sizeof(char), tmp);
				else
				{
					bAttrValueComplete = true;
					bAttrValueEnd = true;
				}
			}
		}
		
		if (bAttrValueComplete)
			if (!bAttrValueBegin || !bAttrValueEnd)
				return XML_ERROR_QUOTES_NOT_MATCH;
		
		if (bAttrNameComplete && bAttrValueComplete)
		{
			tmpXmlObj->SetAttribute(attrName, attrValue);
			bAttrNameComplete  = false;
			bAttrValueComplete = false;
			bAttrValueBegin    = false;
			bAttrValueEnd      = false;
			attrName  = "";
			attrValue = "";
		}
	}
	
	if (!bLabelNameComplete || bAttrNameComplete || bAttrValueComplete)
	{
		delete tmpXmlObj;
		return XML_ERROR_LABELTAIL;
	}
	
	tmpXmlObj->SetLabelClose(bLabelClose);
	tmpXmlObj->SetLabelClassName(labelName);
	*ppLableObj = tmpXmlObj;
	return XML_SUCCESS;
}

XMLERROR XMLFile::ReadLableHead(std::ifstream& inFile, XMLabel* labelObj)
{
	string labelName;
	XMLERROR ret = ReadLabelName(inFile, &labelName, labelObj);
	if (ret != XML_SUCCESS)
		return ret;
	
	labelObj->SetLabelClassName(labelName);
	bool bHeadClose = labelObj->GetLabelHeadClose();
	if (!bHeadClose)
	{
		ret = ReadLabelAttr(inFile, labelObj);
		if (ret != XML_SUCCESS)
			return ret;
	}

	return XML_SUCCESS;

}
XMLERROR XMLFile::ReadLableValue(std::ifstream& inFile, XMLabel* labelObj)
{
	char tmpChar=0;
	string labelValue;
	while (!inFile.eof())
	{
		inFile.read(&tmpChar, sizeof(tmpChar));
		if ('<' == tmpChar)
		{
			inFile.seekg(-1, ios::cur);
			labelObj->SetLabelValue(labelValue);
			return XML_SUCCESS;

		}else if (CR != tmpChar && LF != tmpChar)
		{
			labelValue.append(sizeof(char), tmpChar);
		}
	}
	return XML_ERROR_UNKNOWN;
}

XMLERROR XMLFile::ParseXml(LPCWSTR pszFilePath, XMLabel** rootLabelObj)
{
	//must be ios::binary, because seekg(off, way) do not support text file
	std::ifstream inXmlFile(pszFilePath, ios::binary);
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
		XMLERROR ret = XML_SUCCESS;
		inXmlFile.read(&tmpChar, sizeof(tmpChar));
		if ('<' == tmpChar)
		{
			inXmlFile.read(&tmpChar, sizeof(tmpChar));
			if (IS_LETTER(tmpChar))
			{
				newObj = new XMLabel();
				if (NULL == newObj)
					return XML_ERROR_MEMORY;
				
				inXmlFile.seekg(-1, ios::cur);
				ret = ReadLableHead(inXmlFile, newObj);
				
				if (XML_SUCCESS != ret )
					return ret;
				
				if (!newObj->GetLabelClose())
				{
					ret = ReadLableValue(inXmlFile, newObj);
					if (XML_SUCCESS != ret )
						return ret;

					lableObjStack.push(newObj);
				}else if (!lableObjStack.empty())
				{
					XMLabel* topObj = lableObjStack.top();
					newObj->SetParent(topObj);
					topObj->AddChild(newObj);
				}
			}else if ('!' == tmpChar)
			{
				XMLERROR ret = ReadComment(inXmlFile);
				if (XML_SUCCESS != ret )
					return ret;
			}else if ('/' == tmpChar)
			{
				string labelTailName;
				ret = ReadLableTail(inXmlFile, &labelTailName);
				if (XML_SUCCESS != ret )
					return ret;

				//pop the labelObject in stack, check the labelObject's id with labelTailName
				//then set the labelObject to its parent
				if (lableObjStack.empty())
					return XML_ERROR_UNKNOWN;

				XMLabel* topObj = lableObjStack.top();
				if (NULL == topObj )
					return XML_ERROR_UNKNOWN;

				if (topObj->GetLabelClassName() != labelTailName)
					return XML_MISMATCH;

				topObj->SetLabelClose(true);
				lableObjStack.pop();

				if (!lableObjStack.empty())
				{
					XMLabel* parentObj =  lableObjStack.top();

					topObj->SetParent(parentObj);
					parentObj->AddChild(topObj);
				}else
				{
					*rootLabelObj = topObj;
				}
				
			}
			
		}
		
	}
	return XML_SUCCESS;
}