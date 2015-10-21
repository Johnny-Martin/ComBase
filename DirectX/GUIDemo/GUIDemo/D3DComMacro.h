/*--------------------------------------------------------------
【头文件】：D3DComMacro.h    Create by Beyond Ray,2014年2月
（描述）：公共辅助宏的定义
----------------------------------------------------------------*/
#pragma once

#ifndef HR
#define HR(hr) {if(FAILED(hr)) return E_FAIL ;}                //定义HR宏，便于错误检测
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)  {if(p) { (p)->Release(); p = NULL ;}}  //定义一个安全释放宏，便于COM接口的释放
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)   {if(p) {delete(p); p = NULL ;}}		//定义一个安全删除宏，便于new分配内存的删除      
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) {if(p) {delete [] (p); p = NULL ;}}   //定义一个安全删除宏，便于new[]分配内存的删除
#endif

#ifndef IF
#define IF(p)	{if(p != D3D_OK) return false;}			   //定义一个IF宏，用于返回BOOL型的检测
#endif