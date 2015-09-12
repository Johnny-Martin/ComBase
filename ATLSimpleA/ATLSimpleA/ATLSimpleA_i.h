

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Aug 20 16:09:54 2015
 */
/* Compiler settings for .\ATLSimpleA.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ATLSimpleA_i_h__
#define __ATLSimpleA_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAdd_FWD_DEFINED__
#define __IAdd_FWD_DEFINED__
typedef interface IAdd IAdd;
#endif 	/* __IAdd_FWD_DEFINED__ */


#ifndef __Add_FWD_DEFINED__
#define __Add_FWD_DEFINED__

#ifdef __cplusplus
typedef class Add Add;
#else
typedef struct Add Add;
#endif /* __cplusplus */

#endif 	/* __Add_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAdd_INTERFACE_DEFINED__
#define __IAdd_INTERFACE_DEFINED__

/* interface IAdd */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdd;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D9F8844F-DE89-46F5-A489-4E9801BA5390")
    IAdd : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add_I( 
            /* [in] */ LONG paraA,
            /* [in] */ LONG paraB,
            /* [out] */ LONG *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CatBSTR( 
            /* [in] */ BSTR paraA,
            /* [in] */ BSTR paraB,
            /* [out] */ BSTR *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAddVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdd * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdd * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add_I )( 
            IAdd * This,
            /* [in] */ LONG paraA,
            /* [in] */ LONG paraB,
            /* [out] */ LONG *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CatBSTR )( 
            IAdd * This,
            /* [in] */ BSTR paraA,
            /* [in] */ BSTR paraB,
            /* [out] */ BSTR *result);
        
        END_INTERFACE
    } IAddVtbl;

    interface IAdd
    {
        CONST_VTBL struct IAddVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAdd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAdd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAdd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAdd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAdd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAdd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAdd_Add_I(This,paraA,paraB,result)	\
    ( (This)->lpVtbl -> Add_I(This,paraA,paraB,result) ) 

#define IAdd_CatBSTR(This,paraA,paraB,result)	\
    ( (This)->lpVtbl -> CatBSTR(This,paraA,paraB,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAdd_INTERFACE_DEFINED__ */



#ifndef __ATLSimpleALib_LIBRARY_DEFINED__
#define __ATLSimpleALib_LIBRARY_DEFINED__

/* library ATLSimpleALib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ATLSimpleALib;

EXTERN_C const CLSID CLSID_Add;

#ifdef __cplusplus

class DECLSPEC_UUID("0990362A-100A-4B26-82CF-3D38095AC1A6")
Add;
#endif
#endif /* __ATLSimpleALib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


