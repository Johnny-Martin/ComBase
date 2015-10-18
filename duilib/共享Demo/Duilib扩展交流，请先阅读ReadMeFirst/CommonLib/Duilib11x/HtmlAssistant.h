#pragma once


#import "libid:{3050F1C5-98B5-11CF-BB82-00AA00BDCE0B}" named_guids
using namespace MSHTML;


IDispatchPtr HTMLDOMGetNode(IHTMLDocument3* pDocument, LPCTSTR lpszXPath);	//select node or attribute
IDispatchPtr HTMLDOMGetNode(IHTMLDOMNode* pHTMLDOMNode, LPCTSTR lpszXPath);	//select node or attribute
_bstr_t      HTMLDOMGetTextValue(IDispatch* pElementOrAttribute);			//get text or attribute value

class CHtmlAssDocument; //文档
class CHtmlAssElement;  //元素
class CHtmlAssElementList; //元素集合


class CHtmlAssDocument
{
public:
	CHtmlAssDocument(IHTMLDocument* pDocument = NULL) : m_pDocument(pDocument) {}
	
	CHtmlAssElement SelectNode(LPCTSTR lpszXPath);

public:
	IHTMLDocumentPtr m_pDocument;
};


class CHtmlAssElement
{
public:
	CHtmlAssElement (IHTMLElement* pElement = NULL) : m_pElement(pElement) {}

	_bstr_t             GetText();
	CHtmlAssElementList GetChildren();
	CHtmlAssElement     SelectNode(LPCTSTR lpszXPath);

public:
	IHTMLElementPtr m_pElement;
};


class CHtmlAssElementList
{
public:
	CHtmlAssElementList(IHTMLElementCollection* pCollection = NULL) : m_pElementCollection(pCollection) {}
	size_t          size();
	CHtmlAssElement operator[](size_t nIndex);

public:
	IHTMLElementCollectionPtr m_pElementCollection;
};
