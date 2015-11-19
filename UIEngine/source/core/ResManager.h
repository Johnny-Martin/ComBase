#include "stdafx.h"
#include "png.h"

using namespace std;

#ifndef RESOURCRMANAGER_H
#define RESOURCRMANAGER_H
enum RESERROE{
	RES_SUCCESS = 0,
	RES_ERROR_ILLEGAL_ID,
	RES_ERROR_FILE_NOT_FOUND,
	RES_ERROR_ILLEGAL_FILE_TYPE,
	RES_ERROR_PARSE_FILE_FALIED,
	RES_ERROR_UNKNOWN,
	RES_ERROR_COUNT
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
	{};
	virtual ~RPicture()
	{
		//m_rowPointers is allocated by malloc() in function png_create_read_struct(),
		//thus release with free()
		for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
			free(m_rowPointers[rowIndex]);
		
		free(m_rowPointers);
		cout<<"free m_rowPointers done"<<endl;
	}
	

protected:
	virtual RESERROE LoadResource(LPCWSTR wszResPath) = 0;
	virtual RESERROE Draw() = 0;
	RESERROE ReadPngFile(LPCWSTR wszFilePath);
	RESERROE WritePngFile(LPCWSTR wszFilePath, png_bytep *rowPointers, unsigned int width, unsigned int height);
	bool IsVerticalLine(unsigned int horizontalPos, COLORREF lineColor);
	
	png_uint_32 m_pngWidth;
	png_uint_32 m_pngHeight;
	png_byte    m_colorType;
	png_byte    m_bitDepth;
	png_bytep * m_rowPointers; //In fact, m_rowPointers is a two-dimensional array
	png_structp m_pngStructPtr;
	png_infop   m_pngInfoPtr;

private:
	HBITMAP m_hResHandle;//??????
	string m_szResID;
	string m_szResTypeInfo;
};

class RImage: public RPicture
{
public:
	RImage(){};
	RImage(LPCWSTR wszResPath){
		//LoadResource(wszResPath);
		ReadPngFile(wszResPath);
	}
	RESERROE LoadResource(LPCWSTR wszResPath);
	RESERROE Draw();
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
	RTexture();
	RTexture(LPCWSTR wszResPath){
		//LoadResource(wszResPath);
		ReadPngFile(wszResPath);
	};
	RESERROE LoadResource(LPCWSTR wszResPath);
	RESERROE Draw();
protected:
	RESERROE DetectPurpleLine();
private:
	//vertical dividing line's position in horizontal direction
	vector<unsigned int> m_arrDivideLinePosH;
	//horizontal dividing line's position in vertical direction
	vector<unsigned int> m_arrDivideLinePosV;
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

	RPicList(LPCWSTR wszResPath):m_purpleLineColor(RGBA(127,0,127,255))
	{
		//LoadResource(wszResPath);
		ReadPngFile(wszResPath);
		DetectPurpleLine();
		CreatePicFromMem();
	}
	RESERROE LoadResource(LPCWSTR wszResPath);
	RESERROE Draw(){ return RES_SUCCESS;};//RPicList do not need a draw function
protected:
	RESERROE DetectPurpleLine();
	RESERROE CreatePicFromMem();
private:
	//vertical dividing line's position in horizontal direction
	vector<unsigned int> m_arrDivideLinePosH;
	const COLORREF m_purpleLineColor;
	map<unsigned int, RPicture*> m_picIndex2HandleMap;
	
};

class ResManager
{
public:
	ResManager(){};
	ResManager(LPWSTR szResPath){
		SetResPath(szResPath);
	}
	RESERROE SetResPath(LPWSTR wszResPath){
		m_wszResPath = wszResPath;
		if (::PathFileExists(wszResPath))
			return RES_SUCCESS;
		
		return RES_ERROR_FILE_NOT_FOUND;
	}
	static bool CheckPngFileHead(LPWSTR wszFilePath);
	RESERROE GetResPicHandle(LPCSTR szResID, RPicture** hRes);
protected:
private:
	wstring GetPicPathByID(LPCSTR szResID);
	wstring m_wszResPath;
	map<string, RPicture*> m_resID2HandleMap;
};
#endif