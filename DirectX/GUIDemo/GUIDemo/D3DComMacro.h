/*--------------------------------------------------------------
��ͷ�ļ�����D3DComMacro.h    Create by Beyond Ray,2014��2��
��������������������Ķ���
----------------------------------------------------------------*/
#pragma once

#ifndef HR
#define HR(hr) {if(FAILED(hr)) return E_FAIL ;}                //����HR�꣬���ڴ�����
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)  {if(p) { (p)->Release(); p = NULL ;}}  //����һ����ȫ�ͷź꣬����COM�ӿڵ��ͷ�
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)   {if(p) {delete(p); p = NULL ;}}		//����һ����ȫɾ���꣬����new�����ڴ��ɾ��      
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) {if(p) {delete [] (p); p = NULL ;}}   //����һ����ȫɾ���꣬����new[]�����ڴ��ɾ��
#endif

#ifndef IF
#define IF(p)	{if(p != D3D_OK) return false;}			   //����һ��IF�꣬���ڷ���BOOL�͵ļ��
#endif