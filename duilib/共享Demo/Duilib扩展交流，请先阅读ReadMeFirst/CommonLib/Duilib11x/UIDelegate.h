#ifndef __UIDELEGATE_H__
#define __UIDELEGATE_H__

#pragma once

namespace DuiLib {

/// ί�еĻ����ӿ�
class UILIB_API CDelegateBase	 
{
public:
    CDelegateBase(void* pObject, void* pFn);
    CDelegateBase(const CDelegateBase& rhs);
    virtual ~CDelegateBase();
    bool Equals(const CDelegateBase& rhs) const;
    bool operator() (void* param); //���ز�����()���ú���
    virtual CDelegateBase* Copy() const = 0; // add const for gcc

protected:
    void* GetFn();
    void* GetObject();
    virtual bool Invoke(void* param) = 0;

private:
    void* m_pObject;
    void* m_pFn;
};

/// ��̬ί�У�ʵ�ʰ�װ��һ����ͨ�ĺ���ָ��p
/// ����ί�У����ǵ�����һ������p()
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

/// ��ͨί�У�ʵ�ʰ�װ��ĳ������o�ĳ�Ա����ָ��m_p
/// ����ί�У����ǵ�����ĳ������ĳ�Ա����o->m_p()
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

/// ����һ����ͨί��
template <class O, class T>
CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(void*))
{
    return CDelegate<O, T>(pObject, pFn);
}

/// ����һ����̬ί��
inline CDelegateStatic MakeDelegate(bool (*pFn)(void*))
{
    return CDelegateStatic(pFn); 
}


/// �¼�Դ���ڲ�������һϵ��ί�С�
/// ����()��ʱ�򣬵��ڵ�����һ�麯����������ͨ������ĳЩ����ĳ�Ա��������
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
    bool operator() (void* param); //���ز�����()����һ�麯��

protected:
    CStdPtrArray m_aDelegates;
};

} // namespace DuiLib

#endif // __UIDELEGATE_H__