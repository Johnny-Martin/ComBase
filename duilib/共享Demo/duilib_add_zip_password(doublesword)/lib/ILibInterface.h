#ifndef __ILibInterface_H
#define __ILibInterface_H

/** 对lib文件的更新和添加工作

*   FileName  : ILibInterface.h

*   Author    : Double Sword

*   date      : 2011-8-16

*   Copyright : Copyright belong to Author and ZhiZunNet.CO.

*   RefDoc    : --

*/

/** 打开zip文件
*   @Author : Double sword
*   @Params : sourcrZipPath-zip文件全路径,password-密码	
*   @Return : 返回该zip文件的id,关闭时需要传这个参数,0失败
*   @Date   : 2011-8-16
*/
extern int OpenZip(LPCSTR sourcrZipPath,LPCSTR password);

/** 关闭zip文件
*   @Author : Double sword
*   @Params : fileId-从openzip获得,bHasUpdateOpt是否调用UpdateZipFile
*   @Return : 无
*   @Date   : 2011-8-16
*/
extern void CloseZip(int nZip,bool bHasUpdateOpt = true);

/** 对比文件crc
*   @Author : Double sword
*   @Params : nZip-OpenZip产生,filename-需要对比的文件名(zip内的文件名可以带目录1/1.txt)，ulCrc-文件的crc		
*   @Return : true表示需要更新，false不需要
*   @Date   : 2011-8-16
*/
extern bool CompareCrc(int nZip,LPCSTR filename,unsigned long ulCrc);

/** 添加需要更新的文件缓冲
*   @Author : Double sword
*   @Params : nZip-OpenZip产生,filename-文件名(zip内的文件名可以带目录1/1.txt),pBuf-内存流,nLen-长度		
*   @Return : 
*   @Date   : 2011-8-16
*/
extern bool AddUpdateFile(int nZip,LPCSTR filename,char* pBuf,int nLen);

/** 添加完所有的文件调用次函数进行更新zip文件
*   @Author : Double sword
*   @Params : nZip-OpenZip产生,		
*   @Return : true成功,false失败
*   @Date   : 2011-8-16
*/
extern bool UpdateZipFile(int nZip);

/** 获取文件列表
*   @FuncName : GetZipFileList
*   @Author   : Double sword
*   @Params   : pszList \0分割项 \0\0表示结束	
*   @Return   :	void			
*   @Date     :	2012-10-31
*/
extern void GetZipFileList(int nZip,char* pszList);


/** 读取zip文件解压后的大小
*   @FuncName : 
*   @Author   : Double sword
*   @Params   : 	
*   @Return   :					
*   @Date     :	2012-10-31
*/
extern int GetZipFileSize(int nZip,char* pszFileName);


/** 读取zip文件字节流
*   @FuncName : ReadZipFile
*   @Author   : Double sword
*   @Params   : nZip,pszFileName,pPwd不需要密码时传NULL,pBuf ,lenth of pBuf

*   @Return   :	文件的大小

*   @Date     :	2012-10-31
*/
extern int ReadZipFile(int nZip,char* pszFileName,char* pPwd,void* pBuf,int nBufLen);








/************************************************************************/
/* 库加载                                                               */
/************************************************************************/
#if defined(_MT) && defined(_DLL)	//[MD]
#	ifdef _DEBUG			//MDD
#		ifdef _UNICODE 
#			pragma comment(lib,"ZipLibud.lib")
#		else
#			pragma comment(lib,"ZipLibd.lib")
#		endif
#	else					//MD
#		ifdef _UNICODE 
#			pragma comment(lib,"ZipLibu.lib")
#		else
#			pragma comment(lib,"ZipLib.lib")
#		endif
#	endif
#elif defined(_MT) && !defined(_DLL) //[MT]
#	ifdef _DEBUG			//MTD
#		ifdef _UNICODE 
#			pragma comment(lib,"ZipLibumtd.lib")
#		else
#			pragma comment(lib,"ZipLibmtd.lib")
#		endif
#	else					//MT
#		ifdef _UNICODE 
#			pragma comment(lib,"ZipLibumt.lib")
#		else
#			pragma comment(lib,"ZipLibmt.lib")
#		endif
#	endif
#endif




//sample
//
//int main()
//{
//
//#define __WINDOWS__
//#if defined(__WINDOWS__)
//	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//	int nZip = OpenZip( TEXT("texture.paq"),TEXT("123456") );
//	if (nZip)
//	{
//		char* pBuf = NULL;
//		int nSize = 0;
//		FILE* f = fopen("bk31.ani","rb");
//		if (f)
//		{
//			fseek(f,0,SEEK_END);
//			nSize = ftell(f);
//			fseek(f,0,SEEK_SET);
//			pBuf = new char[nSize];
//			fread(pBuf,1,nSize,f);
//
//		}
//		if( CompareCrc(nZip,"texture/bk31.ani",0) )
//		{
//			AddUpdateFile( nZip,"texture/bk31.ani",pBuf,nSize );
//		}
//
//		UpdateZipFile(nZip);
//
//		CloseZip(nZip);
//
//		if(pBuf)
//			delete[] pBuf;
//	}
//
//	return 0;
//}

#endif //__ILibInterface_H