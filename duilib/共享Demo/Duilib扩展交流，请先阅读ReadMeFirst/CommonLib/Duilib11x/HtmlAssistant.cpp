#include "StdAfx.h"
#include "HtmlAssistant.h"
#include <string>
#include <vector>

__if_not_exists(tstring){
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string  tstring;
#endif
};

//////////////////////////////////////////////////////////////////////////
//
//  SelectNode / SelectNodeList
//
//////////////////////////////////////////////////////////////////////////

IDispatchPtr HTMLDOMGetNode(IHTMLDocument3* pDocument, LPCTSTR lpszXPath)
{
	IDispatchPtr pDispatch;

	if (pDocument == NULL)
		return pDispatch;

	if (lpszXPath == nullptr || *lpszXPath == 0)
		return pDispatch;

	TCHAR tag[8] = {0};
	_tcsncpy_s(tag, lpszXPath, 6);
	if(0 != _tcsicmp(tag, _T("/html/")))
		return pDispatch;

	IHTMLElementPtr pElement;
	pDocument->get_documentElement(&pElement);//the name is HTML
	pDispatch = pElement;

	if (lpszXPath[6] == _T('\0'))
	{
		return pDispatch;
	}
	else
	{
		IHTMLDOMNodePtr pNode = pDispatch;
		return HTMLDOMGetNode((IHTMLDOMNode*)pNode, lpszXPath + 6);
	}
}

IDispatchPtr HTMLDOMGetNode(IHTMLDOMNode* pHTMLDOMNode, LPCTSTR lpszXPath)
{
	IDispatchPtr pDispatch;

	if (pHTMLDOMNode == NULL)
		return pDispatch;

	if (lpszXPath == nullptr || *lpszXPath == 0)
		return pDispatch;

	ASSERT(*lpszXPath != _T('/'));

	IHTMLDOMNodePtr pNode = pHTMLDOMNode;

	tstring sXpath = lpszXPath;
	std::vector<tstring> splitor;
	size_t p1 = 0;
	size_t p2 = 0;
	while (true)
	{
		p2 = sXpath.find(_T('/'), p1);
		if (p2 == tstring::npos)
		{
			splitor.push_back(sXpath.substr(p1));
			break;
		}
		else
		{
			splitor.push_back(sXpath.substr(p1, p2 - p1));
			p1 = p2 + 1;
		}
	}

	for (size_t i = 0; i < splitor.size(); ++i)
	{
		//拆出名称和下标(base 1）
		tstring sTagName;
		long     nIndex = 1;

		tstring sUnit = splitor[i];
		size_t pos = sUnit.find(_T('['));
		if (pos == tstring::npos)
		{
			sTagName = sUnit;
			nIndex   = 1;
		}
		else
		{
			sTagName = sUnit.substr(0, pos);
			nIndex   = _ttoi(sUnit.substr(pos+1).c_str());
		}

		//分类处理
		if (sTagName.empty())
		{
			continue;//ignore
		}
		else if (sTagName.at(0) == _T('@')) //attribute
		{
			IDispatchPtr pDisp;
			IHTMLAttributeCollection2Ptr pAC2;
			pNode->get_attributes(&pDisp);
			pAC2 = pDisp;
			pNode.Release();

			IHTMLDOMAttributePtr pAt;
			if (pAC2)
			{
				pAt = pAC2->getNamedItem(_bstr_t(sTagName.c_str() + 1));
				pDispatch = pAt;
				pNode     = pAt;//will null and break, so copy to pDispatch
			}
		}
		else //child
		{
			//获得子节点集合
			IDispatchPtr pDisp;
			IHTMLDOMChildrenCollectionPtr pCC;
			pNode->get_childNodes(&pDisp);
			pCC = pDisp;
			pNode.Release();

			long nCount = 0;
			pCC->get_length(&nCount);

			//根据下标获取
			if (sTagName == _T("*"))
			{
				if (nIndex > 0 && nIndex <= nCount)
				{
					pNode = pCC->item(nIndex - 1);
				}
			}
			else
			{
				int k = 0;//同名标签数
				for (int j = 0; j < nCount; ++j)
				{
					IHTMLDOMNodePtr pTemp = pCC->item(j);

					_bstr_t bstr;
					pTemp->get_nodeName(bstr.GetAddress());
					if (0 == _tcsicmp(sTagName.c_str(), (LPCTSTR)bstr))
					{
						++k;
						if (k == nIndex)
						{
							pNode = pTemp;
							break;
						}
					}
				}//for 0-count
			}
		}//child

		//判断为空跳出
		if (pNode == NULL)
			break;
	}

	if (pNode != NULL)
		pDispatch = pNode;

	return pDispatch;
}

_bstr_t HTMLDOMGetTextValue(IDispatch* pElementOrAttribute)
{
	_bstr_t bstr;

	IHTMLElementPtr pElement = pElementOrAttribute;
	if (pElement)
	{
		pElement->get_innerText(bstr.GetAddress());
		return bstr;
	}

	IHTMLDOMAttribute2Ptr pAttribute = pElementOrAttribute;
	if (pAttribute)
	{
		pAttribute->get_value(bstr.GetAddress());
		return bstr;
	}

	return bstr;
}

//////////////////////////////////////////////////////////////////////////
//
// CHtmlAssDocument
//
//////////////////////////////////////////////////////////////////////////

CHtmlAssElement CHtmlAssDocument::SelectNode(LPCTSTR lpszXPath)
{
	IHTMLDocument3Ptr pDocument3 = m_pDocument;
	IHTMLElementPtr pElement = HTMLDOMGetNode((IHTMLDocument3*)pDocument3, lpszXPath);
	return CHtmlAssElement(pElement);
}

//////////////////////////////////////////////////////////////////////////
//
// CHtmlAssElement
//
//////////////////////////////////////////////////////////////////////////

_bstr_t CHtmlAssElement::GetText()
{
	_bstr_t bstr;
	if (m_pElement)
		m_pElement->get_innerText(bstr.GetAddress());
	return bstr;
}

CHtmlAssElementList CHtmlAssElement::GetChildren()
{
	CHtmlAssElementList elements;

	if (m_pElement)
	{
		IDispatchPtr pDispatch;
		m_pElement->get_children(&pDispatch);
		elements.m_pElementCollection = pDispatch;
	}

	return elements;
}

CHtmlAssElement CHtmlAssElement::SelectNode(LPCTSTR lpszXPath)
{
	IHTMLDOMNodePtr pNode = m_pElement;
	IHTMLElementPtr pElement = HTMLDOMGetNode((IHTMLDOMNode*)pNode, lpszXPath);
	return CHtmlAssElement(pElement);
}

//////////////////////////////////////////////////////////////////////////
//
// CHtmlAssElementList
//
//////////////////////////////////////////////////////////////////////////

size_t CHtmlAssElementList::size()
{
	long nCount = 0;
	if (m_pElementCollection)
		m_pElementCollection->get_length(&nCount);

	return (size_t)nCount;
}

CHtmlAssElement CHtmlAssElementList::operator[](size_t nIndex)
{
	CHtmlAssElement element;

	if (nIndex < size())
	{
		element.m_pElement = m_pElementCollection->item(_variant_t(nIndex), _variant_t(nIndex));
	}

	return element;
}
