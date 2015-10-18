#ifndef __UIDELEGATE_H__
#define __UIDELEGATE_H__

#pragma once

namespace DuiLib {

/// 委托的基本接口
class UILIB_API CDelegateBase	 
{
public:
    CDelegateBase(void* pObject, void* pFn);
    CDelegateBase(const CDelegateBase& rhs);
    virtual ~CDelegateBase();
    bool Equals(const CDelegateBase& rhs) const;
    bool operator() (void* param); //重载操作符()调用函数
    virtual CDelegateBase* Copy() const = 0; // add const for gcc

protected:
    void* GetFn();
    void* GetObject();
    virtual bool Invoke(void* param) = 0;

private:
    void* m_pObject;
    void* m_pFn;
};

/// 静态委托，实质包装了一个普通的函数指针p
/// 调用委托，就是调用了一个函数p()
class CDelegateStatic: public CDelegateBase
{
    typedef bool (*Fn)(void*);
public:
    CDelegateStatic(Fn pFn) : CDelegateBase(NULL, pFn) { } 
    CDelegateStatic(const CDelegateStatic& rhs) : CDelegateBase(rhs) { } 
    virtual CDelegateBase* Copy() const { return new CDelegateStatic(*this); }

protected:
    virtual bool Invoke(void* param)
    {
        Fn pFn = (Fn)GetFn();
        return (*pFn)(param); 
    }
};

/// 普通委托，实质包装了某个对象o的成员函数指针m_p
/// 调用委托，就是调用了某个对象的成员函数o->m_p()
template <class O, class T>
class CDelegate : public CDelegateBase
{
    typedef bool (T::* Fn)(void*);
public:
    CDelegate(O* pObj, Fn pFn) : CDelegateBase(pObj, &pFn), m_pFn(pFn) { }
    CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn; } 
    virtual CDelegateBase* Copy() const { return new CDelegate(*this); }

protected:
    virtual bool Invoke(void* param)
    {
        O* pObject = (O*) GetObject();
        return (pObject->*m_pFn)(param); 
    }  

private:
    Fn m_pFn;
};

/// 创建一个普通委托
template <class O, class T>
CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(void*))
{
    return CDelegate<O, T>(pObject, pFn);
}

/// 创建一个静态委托
inline CDelegateStatic MakeDelegate(bool (*pFn)(void*))
{
    return CDelegateStatic(pFn); 
}


/// 事件源，内部保存了一系列委托。
/// 调用()的时候，等于调用了一组函数（包括普通函数和某些对象的成员函数）。
class UILIB_API CEventSource
{
    typedef bool (*FnType)(void*);
public:
    ~CEventSource();
    operator bool();
    void operator+= (const CDelegateBase& d); // add const for gcc
    void operator+= (FnType pFn);
    void operator-= (const CDelegateBase& d);
    void operator-= (FnType pFn);
    bool operator() (void* param); //重载操作符()调用一组函数

protected:
    CStdPtrArray m_aDelegates;
};

} // namespace DuiLib

#endif // __UIDELEGATE_H__