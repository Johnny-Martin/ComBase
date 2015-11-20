#include "stdafx.h"
#include "png.h"

using namespace std;

#ifndef RESOURCRMANAGER_H
#define RESOURCRMANAGER_H
enum RESERROR{
	RES_SUCCESS = 0,
	RES_ERROR_ILLEGAL_ID,
	RES_ERROR_FILE_NOT_FOUND,
	RES_ERROR_ILLEGAL_FILE_TYPE,
	RES_ERROR_PARSE_FILE_FALIED,
	RES_ERROR_UNKNOWN,
	RES_ERROR_COUNT
};

enum PICLIST_TYPE{
	TEXTURELIST = 0,
	IMAGELIST,
};
//'R' is short for "Render"

class RPicture
{
public:
	RPicture():m_hResHandle(NULL)
			  ,m_szResID("")
			  ,m_szResTypeInfo("")
			  ,m_pngWidth(0)
			  ,m_pngHeight(0)
			  ,m_colorType(0)
			  ,m_bitDepth(0)
			  ,m_rowPointers(NULL)
			  ,m_pngStructPtr(NULL)
			  ,m_pngInfoPtr(NULL)
			  ,m_resError(RES_SUCCESS)
	{};
	virtual ~RPicture()
	{
		//m_rowPointers is allocated by malloc() in function png_create_read_struct(),
		//thus release with free()
		for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
			free(m_rowPointers[rowIndex]);
		
		free(m_rowPointers);
		cout<<"ID: "<<m_szResID<<" free m_rowPointers done"<<endl;
	}
	void SetResID(string resID)
	{
		m_szResID = resID;
	}
	RESERROR GetLastResError(){ return m_resError;}
	RESERROR WritePngFile(LPCWSTR wszFilePath);
	RESERROR CreatePicByData(LPCSTR szResID, png_bytep* rowPointers, unsigned int width, unsigned int height, png_byte bitDepth = 8, png_byte colorType = 6);
protected:
	virtual RESERROR LoadResource(LPCWSTR wszResPath) = 0;
	virtual RESERROR Draw() = 0;
	RESERROR ReadPngFile(LPCWSTR wszFilePath);
	
	RESERROR WritePngFileEx(LPCWSTR wszFilePath, png_bytep *rowPointers, unsigned int width, unsigned int height);
	bool IsVerticalLine(unsigned int horizontalPos, COLORREF lineColor);
	bool IsHorizontalLine(unsigned int verticalPos, COLORREF lineColor);
	png_uint_32 m_pngWidth;
	png_uint_32 m_pngHeight;
	png_byte    m_colorType;
	png_byte    m_bitDepth;
	png_bytep * m_rowPointers; //In fact, m_rowPointers is a two-dimensional array
	png_structp m_pngStructPtr;
	png_infop   m_pngInfoPtr;

	HBITMAP m_hResHandle;//??????
	string m_szResID;
	string m_szResTypeInfo;
	RESERROR m_resError;

};

class RImage: public RPicture
{
public:
	RImage(){};
	RImage(LPCWSTR wszResPath){
		//LoadResource(wszResPath);
		ReadPngFile(wszResPath);
	}
	RImage(LPCSTR szResID, png_bytep* rowPointers, unsigned int width, unsigned int height, png_byte bitDepth = 8, png_byte colorType = 6)
	{
		m_resError = CreatePicByData(szResID, rowPointers, width, height, bitDepth, colorType);
	}
	RESERROR LoadResource(LPCWSTR wszResPath);
	RESERROR Draw();
protected:
private:
	
};

//a texture object's resource ID *must* begin with "texture"
//and next is texture's type, which include :
//    ** vertical three in one¡¢horizontal three in one¡¢ nine in one **
//    a texture should be used in xml like this:
//* 	 <texture id="bkg">
//*			<attr>
//*				<position>0,20,parent.width, parent.height-20</position>
//*				<resid>texture.ThreeInOne.customRectBkg</resid>
//*			</attr>
//*			......
//*		 </texture>
//the texture's dividing line's color *must* be (255,0,255), and *one* pixel wide
class RTexture: public RPicture
{
public:
	RTexture():m_purpleLineColor(RGBA(255,0,255,255)){};
	RTexture(LPCWSTR wszResPath):m_purpleLineColor(RGBA(255,0,255,255))
	{
		//LoadResource(wszResPath);
		ReadPngFile(wszResPath);
	};
	//create a texture object with a two-dimensional array, and assign the png width and height
	RTexture(LPCSTR szResID, png_bytep* rowPointers, unsigned int width, unsigned int height, png_byte bitDepth = 8, png_byte colorType = 6):m_purpleLineColor(RGBA(255,0,255,255))
	{
		m_resError = CreatePicByData(szResID, rowPointers, width, height, bitDepth, colorType);
	}
	RESERROR LoadResource(LPCWSTR wszResPath);
	RESERROR Draw();
protected:
	RESERROR DetectVerticalLine();
	RESERROR DetectHorizontalLine();
private:
	//vertical dividing line's position in horizontal direction
	vector<unsigned int> m_arrVerticalLinePos;
	//horizontal dividing line's position in vertical direction
	vector<unsigned int> m_arrHorizontalLinePos;
	const COLORREF m_purpleLineColor;
};

//a RPicList object's resource ID *must* begin with "imagelist" or "texturelist"
//    a texture should be used in xml like this:
//* 	 <texture id="bkg">
//*			<attr>
//*				<position>0,20,parent.width, parent.height-20</position>
//*				<resid>texturelist.ThreeInOne.customRectBkg.3</resid>
//*			</attr>
//*			......
//*		 </texture>
//*		 <image id="bkg">
//*			<attr>
//*				<position>0,20,parent.width, parent.height-20</position>
//*				<resid>imagelist.customRectBkg.3</resid>
//*			</attr>
//*			......
//*		 </image>
//the last part of resid must be a number, the index of pic in picture list
//the texture's dividing line's color *must* be (127,0,127),different from 
//texture's dividing line, and *one* pixel wide
class RPicList: public RPicture
{
public:
	RPicList():m_purpleLineColor(RGBA(127,0,127,255)){};

	RPicList(LPCWSTR wszResPath, LPCSTR resID):m_purpleLineColor(RGBA(127,0,127,255))
	{
		//LoadResource(wszResPath);
		SetResID(resID);
		SetPicListType(resID);
		m_resError = ReadPngFile(wszResPath);
		if (RES_SUCCESS != m_resError)
			return;

		m_resError = DetectVerticalLine();
		if (RES_SUCCESS != m_resError)
			return;

		m_resError = CreatePicFromMem();
	}
	~RPicList()
	{
		for (unsigned int i=0; i<m_picListVector.size(); ++i)
			delete m_picListVector[i];
	}
	RPicture* GetPicByIndex(unsigned int index)
	{
		if (m_picListVector.size() <= index)
			return NULL;
		return m_picListVector[index];
	}
	void SetPicListType(LPCSTR resID)
	{
		string strResID = resID;
		if (0 == strResID.find("texturelist"))
			m_picListType = TEXTURELIST;
		else if (0 == strResID.find("imagelist"))
			m_picListType = IMAGELIST;
		else
			abort();
	}
	RESERROR LoadResource(LPCWSTR wszResPath);
	RESERROR Draw(){ return RES_SUCCESS;};//RPicList do not need a draw function
protected:
	RESERROR DetectVerticalLine();
	RESERROR CreatePicFromMem();
private:
	//vertical dividing line's position in horizontal direction
	vector<unsigned int> m_arrVerticalLinePos;
	const COLORREF m_purpleLineColor;
	vector<RPicture*> m_picListVector;
	PICLIST_TYPE m_picListType;
	
};

class ResManager
{
public:
	ResManager(){};
	ResManager(LPWSTR szResPath){
		SetResPath(szResPath);
	}
	~ResManager()
	{
		map<string, RPicture*>::iterator iter = m_resID2HandleMap.begin();
		for (;iter != m_resID2HandleMap.end(); ++iter)
		{
			delete iter->second;
		}
	}
	RESERROR SetResPath(LPWSTR wszResPath){
		m_wszResPath = wszResPath;
		if (::PathFileExists(wszResPath))
			return RES_SUCCESS;
		
		return RES_ERROR_FILE_NOT_FOUND;
	}
	static bool CheckPngFileHead(LPWSTR wszFilePath);
	RESERROR GetResPicHandle(LPCSTR szResID, RPicture** hRes);
protected:
private:
	unsigned int GetIndexFromPicListId(LPCSTR szPicListID);
	string GetRealIdFromPicListId(LPCSTR szPicListID);
	wstring GetPicPathByID(LPCSTR szResID);
	wstring m_wszResPath;
	map<string, RPicture*> m_resID2HandleMap;
};
#endif