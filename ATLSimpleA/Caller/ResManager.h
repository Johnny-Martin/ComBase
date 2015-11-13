#include "stdafx.h"

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
	RPicture();
protected:
	virtual RESERROE LoadResource(LPSTR szResPath) = 0;
	virtual RESERROE Draw() = 0;
private:
	HBITMAP m_hResHandle;//??????
	string m_szResID;
	string m_zsResTypeInfo;
};

class RImage: public RPicture
{
public:
	RImage();
	RImage(LPSTR szResPath);
	RESERROE LoadResource(LPSTR szResPath);
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
	RTexture(LPSTR szResPath);
	RESERROE LoadResource(LPSTR szResPath);
	RESERROE Draw();
protected:
private:
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
//the texture's dividing line's color *must* be (128,0,128),different from 
//texture's dividing line, and *one* pixel wide
class RPicList: public RPicture
{
public:
	RPicList();
	RPicList(LPSTR szResPath);
	RESERROE LoadResource(LPSTR szResPath);
	RESERROE Draw(){};
protected:
private:
};

class ResManager
{
public:
	ResManager(){};
	ResManager(LPWSTR szResPath){
		SetResPath(szResPath);
	}
	RESERROE SetResPath(LPWSTR wszResPath){
		if (::PathFileExists(wszResPath))
		{
			m_wszResPath = wszResPath;
			return RES_SUCCESS;
		}
		return RES_ERROR_FILE_NOT_FOUND;
	}
	
	RESERROE GetResPicHandle(LPSTR szResID, RPicture** hRes);
protected:
private:
	RESERROE GetPicPathByID(LPSTR szResID);
	wstring m_wszResPath;
	map<string, RPicture*> m_resID2HandleMap;
};
#endif