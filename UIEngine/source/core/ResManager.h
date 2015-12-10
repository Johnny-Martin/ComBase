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
	RES_ERROR_ILLEGAL_PNG_FILE,
	RES_ERROR_CREATE_D2D1BITMAP_FAILED,
	RES_ERROR_UNKNOWN,
	RES_ERROR_COUNT
};

enum PICLIST_TYPE{
	TEXTURELIST = 0,
	IMAGELIST,
};
enum TEXTURE_TYPE{
	THREE_V = 0,
	NINE,
	THREE_H,
};
//'R' is short for "Resource"

class RPicture
{
public:
									RPicture();
	virtual							~RPicture();
	void							SetResID(string resID);
	RESERROR						GetLastResError() const;
	RESERROR						WritePngFile(LPCWSTR wszFilePath);
	RESERROR						CreatePicByData(LPCSTR szResID, 
													png_bytep* rowPointers, 
													unsigned int width, 
													unsigned int height, 
													png_byte bitDepth, 
													png_byte colorType);
	png_bytep*						GetRowPointers() const;
	png_infop						GetPngInfo() const;
	virtual RESERROR				Draw(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom) = 0;
protected:
	virtual RESERROR				LoadResource(LPCWSTR wszResPath) = 0;
	RESERROR						ReadPngFile(LPCWSTR wszFilePath);
	
	
	bool							IsVerticalLine(unsigned int horizontalPos, COLORREF lineColor);
	bool							IsHorizontalLine(unsigned int verticalPos, COLORREF lineColor);
	png_uint_32						m_pngWidth;
	png_uint_32						m_pngHeight;
	png_byte						m_colorType;
	png_byte						m_bitDepth;
	png_byte						m_pixelDepth;
	png_bytep*						m_rowPointers; //In fact, m_rowPointers is a two-dimensional array
	png_structp						m_pngStructPtr;
	png_infop						m_pngInfoPtr;

	HBITMAP							m_hResHandle;//??????
	string							m_szResID;
	string							m_szResTypeInfo;
	RESERROR						m_resError;

};

class RImage: public RPicture
{
public:
									RImage(){};
									RImage(LPCWSTR wszResPath, LPCSTR szResID);
									RImage(LPCSTR szResID, 
										   png_bytep* rowPointers, 
										   unsigned int width, 
										   unsigned int height,
										   png_byte bitDepth, 
										   png_byte colorType);
	RESERROR						LoadResource(LPCWSTR wszResPath);
	RESERROR						Draw(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
protected:
private:
	RESERROR						CreateD2D1Bitmap(ID2D1RenderTarget* pRenderTarget);
	vector<ID2D1Bitmap*>			m_arrD2D1Bitmap;
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
									RTexture(LPCWSTR wszResPath, LPCSTR szResID);
									//create a texture object with a two-dimensional array, and assign the png width and height
									RTexture(LPCSTR szResID, 
											 png_bytep* rowPointers, 
											 unsigned int width, 
											 unsigned int height, 
											 png_byte bitDepth, 
											 png_byte colorType);

	RESERROR						LoadResource(LPCWSTR wszResPath);
	RESERROR						Draw(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
protected:
	RESERROR						DetectVerticalLine();
	RESERROR						DetectHorizontalLine();
private:
	void							SetTextureType(LPCSTR resID)
	{
		string strResID = resID;
		if (string::npos != strResID.find("ThreeV"))
			m_textureType = THREE_V;
		else if (string::npos != strResID.find("Nine"))
			m_textureType = NINE;
		else if (string::npos != strResID.find("ThreeH"))
			m_textureType = THREE_H;
		else
			abort();
	}
	void							InitMemberVariable()
	{

	}
	RESERROR						ProcessTexture();
	RESERROR						CreateD2D1Bitmap(ID2D1RenderTarget* pRenderTarget);
	RESERROR						_CreateD2D1Bitmap_Nine(ID2D1RenderTarget* pRenderTarget);
	RESERROR						_CreateD2D1Bitmap_ThreeV(ID2D1RenderTarget* pRenderTarget);
	RESERROR						_CreateD2D1Bitmap_ThreeH(ID2D1RenderTarget* pRenderTarget);

	RESERROR						_Draw_Nine(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
	RESERROR						_Draw_ThreeV(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
	RESERROR						_Draw_ThreeH(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
									//vertical dividing line's position in horizontal direction
	vector<unsigned int>			m_arrVerticalLinePos;
									//horizontal dividing line's position in vertical direction
	vector<unsigned int>			m_arrHorizontalLinePos;
	vector<ID2D1Bitmap*>			m_arrD2D1Bitmap;
	const COLORREF					m_purpleLineColor;
	TEXTURE_TYPE					m_textureType;
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
									RPicList();
									RPicList(LPCWSTR wszResPath, LPCSTR resID);
									~RPicList();
	RPicture*						GetPicByIndex(unsigned int index);
	RESERROR						LoadResource(LPCWSTR wszResPath);

									//RPicList do not need a draw function
	RESERROR						Draw(ID2D1RenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom);
protected:
	RESERROR						DetectVerticalLine();
	RESERROR						CreatePicFromMem();
private:
	void							SetPicListType(LPCSTR resID);
	
									//vertical dividing line's position in horizontal direction
	vector<unsigned int>			m_arrVerticalLinePos;
	const COLORREF					m_purpleLineColor;
	vector<RPicture*>				m_picListVector;
	PICLIST_TYPE					m_picListType;
	
};

class ResManager
{
public:
									ResManager(){};
									ResManager(LPWSTR szResPath);
									~ResManager();
	RESERROR						SetResPath(LPWSTR wszResPath);
	static bool						CheckPngFileHead(LPWSTR wszFilePath);
	RESERROR						GetResPicHandle(LPCSTR szResID, RPicture** hRes);
protected:
private:
	unsigned int					GetIndexFromPicListId(LPCSTR szPicListID);
	string							GetRealIdFromPicListId(LPCSTR szPicListID);
	wstring							GetPicPathByID(LPCSTR szResID);
	wstring							m_wszResPath;
	map<string, RPicture*>			m_resID2HandleMap;
};
#endif