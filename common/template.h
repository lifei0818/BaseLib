#ifndef BaseLib_TEMPLATE_H
#define BaseLib_TEMPLATE_H

#include "common/sys_incl.h"
#include "common/macro.h"
#include "common/debug.h"

namespace BaseLib
{
typedef std::vector<std::string> VECTOR_STRING;
typedef std::list<std::string> LIST_STRING;
typedef std::list<std::wstring> LIST_WSTRING;
typedef std::list<DWORD> LIST_DWORD;
typedef std::map<std::string,DWORD> MAP_DWORD;
typedef std::set<std::string> SET_STRING;
typedef std::set<DWORD> SET_DWORD;

/************************************************************************/
/*	
/************************************************************************/

///	指针循环删除
template <typename TYPE,typename ITER>
ITER
PointerDeleteOne(TYPE &t, ITER iter)
{
	delete (*iter);
	return t.erase (iter);
};

///	指针循环删除
template <typename TYPE>
void
PointerDeleteAll(TYPE &t)
{
    typename TYPE::iterator iter;
	for (iter = t.begin();
		iter != t.end();
		iter++)
	{
		delete (*iter);
	}
	t.clear();
};

/// 格式化容器
template <typename T>
std::string
ToString(T &continer, LPCSTR lpChar)
{
	std::ostringstream o;
	for (typename T::iterator iter = continer.begin();
		iter != continer.end();
		iter++)
	{
		o << *iter << lpChar;
	}
	return o.str().c_str();
}

///	指针小于比较
template
<typename TYPE>
struct Pointer_LessCompare
{
	bool operator()(const TYPE& X, const TYPE& Y) const
	{
		return (*X) < (*Y); 
	}
};

///	字符串比较
struct String_Compare 
{
	bool operator()(const std::string &X, const std::string &Y) const
	{
		return _stricmp (X.c_str(), Y.c_str()) < 0;
	}
};



/************************************************************************/
/*	该模板实现Singleton模式 
/************************************************************************/

/**
* 推荐使用 Singleton 模式
* 
* 类定义:
* class CSGetDbgCode : public BaseLib::ISingleton<CSGetDbgCode> {}
*
* 类使用:
* CSGetDbgCode::Instance()
*/
template <typename T>
class ISingleton
{
public:
	static  T* Instance()
	{
		if (!m_Instance.get())
		{
			T* p = NULL;
			try
			{
				p = new T;
			}
			catch (...)
			{
				p = NULL;
			}

			if (NULL == p)
			{
				OutputDebugString (_T("*** 内存申请失败 ***") NEW_LINE);
			}

			m_Instance.reset (p);
		}

		return m_Instance.get();
	}

protected:
	ISingleton(void) {}
	~ISingleton(void) {}

	static std::auto_ptr<T> m_Instance;
};

template <typename T>
std::auto_ptr<T> ISingleton<T>::m_Instance;



/// 自动释放内存
/**
* 与 ISingleton 区别: T 可以有一个全局实例和多个局部实例,之间无任何关联
*/
template <typename T>
class TSingleton
{
public:
	static  T* Instance();
private:
	TSingleton(void) {}
	~TSingleton(void) {}
	static std::auto_ptr<T> m_Instance;
};

template <typename T>
T* TSingleton<T>::Instance()
{
	if (!m_Instance.get())
	{
		T* p = NULL;
		try
		{
			p = new T;
		}
		catch (...)
		{
			p = NULL;
		}

		if (NULL == p)
		{
			OutputDebugStringA ("*** 内存申请失败 ***");
		}

		m_Instance.reset (p);
	}

	return m_Instance.get();
}

template <typename T>
std::auto_ptr<T> TSingleton<T>::m_Instance;


typedef std::map<std::string,std::string,String_Compare> MAP_STRING;
typedef std::multimap<std::string,std::string,String_Compare> MULTIMAP_STRING;


/************************************************************************/
/* 该模板实现资源的自动释放
/************************************************************************/
template
<typename DestroyResourcePolicy>
class TAutoResource
{
public:
	typedef typename DestroyResourcePolicy::ReturnResourceType ResourceType;

	TAutoResource (typename DestroyResourcePolicy::ReturnResourceType resource)
		:_resource (resource)
	{}

	~TAutoResource ()
	{
		DestroyResourcePolicy::Destroy (_resource);
	}

	operator ResourceType* ()	{ return &_resource; }
	operator const ResourceType* () const	{ return &_resource; }
	ResourceType Get() { return _resource; }
	ResourceType* GetAddr() { return &_resource; }

	TAutoResource& operator= (typename DestroyResourcePolicy::ReturnResourceType resource)
	{
		DestroyResourcePolicy::Destroy (_resource);
		_resource = resource;
		return *this;
	}

private:
	TAutoResource (const TAutoResource&);
	TAutoResource& operator=(const TAutoResource &);

protected:
	typename DestroyResourcePolicy::ReturnResourceType _resource;
};

// c释放策略
template
<typename ResourceType,int(*lpDestroyResource)(ResourceType)>   
struct ReturnIntDestroyPolicy
{
	typedef ResourceType ReturnResourceType;

	static void Destroy (ResourceType ptr)
	{
		if (ptr)
		{
			lpDestroyResource (ptr);
		}
	}
};

// api释放策略
template <typename ResourceType,void(__stdcall *lpDestroyResource)(ResourceType)>   
struct ReturnVoidDestroyPolicy
{
	typedef ResourceType ReturnResourceType;

	static void Destroy (ResourceType ptr)
	{
		lpDestroyResource (ptr);
	}
};

template <typename ResourceType,BOOL(__stdcall *lpDestroyResource)(ResourceType)>   
struct ReturnApiDestroyPolicy
{
	typedef ResourceType ReturnResourceType;

	static void Destroy (ResourceType ptr)
	{
		lpDestroyResource (ptr);
	}
};

template
<typename ResourceType,LONG(__stdcall *lpDestroyResource)(ResourceType)>   
struct ReturnLongApiDestroyPolicy
{
	typedef ResourceType ReturnResourceType;

	static void Destroy (ResourceType ptr)
	{
		lpDestroyResource (ptr);
	}
};

typedef TAutoResource<ReturnIntDestroyPolicy<FILE*, fclose> > CAutoFile;

#ifdef _MACRO_OS_WIN32
typedef TAutoResource<ReturnApiDestroyPolicy<HANDLE, CloseHandle> > CAutoHandle;
#endif
typedef TAutoResource<ReturnApiDestroyPolicy<HMODULE, FreeLibrary> > CAutoLibrary;
#ifdef _MACRO_OS_WIN32
typedef TAutoResource<ReturnLongApiDestroyPolicy<HKEY, RegCloseKey> > CAutoHKey;
#endif


/************************************************************************/
/* 该模板实现对申请的矩阵内存的自动释放
/************************************************************************/

template <typename ResourceType>   
BOOL
WINAPI
DeleteMatrix (ResourceType ptr)
{
	delete []ptr;
	return TRUE;
}

template <typename TYPE>
class TArray : public TAutoResource<ReturnApiDestroyPolicy<TYPE*, BaseLib::DeleteMatrix> >
{
public:
	TArray (size_t iSize = 0) 
		: TAutoResource<ReturnApiDestroyPolicy<TYPE*, BaseLib::DeleteMatrix> >(InitializeResource (iSize))
	{
		Initialize();
	}

	BOOL IsRight()	
	{ return this->_resource ? TRUE : FALSE; }

	//	数组(矩阵)的大小
	size_t  Size()
	{ return m_iSize; }

	//	安全数组(矩阵)的大小
	size_t  SafeSize()
	{ return m_iSize - 1; }

	//	初始化
	void Initialize() 
	{ if (this->_resource) memset (this->_resource, 0, Size() * sizeof (TYPE)); }

	//	容易造成误用
	//TYPE& operator [] (unsigned i)
	//{ 
	//	return _resource[i]; 
	//}

	TYPE& At (unsigned i)
	{ 
		return this->_resource[i]; 
	}

	void Reset (size_t iSize)
	{
		DeleteMatrix (this->_resource);
		this->_resource = InitializeResource (iSize);
		Initialize();
	}

	void AppendAlloc (size_t iNewSize)
	{
		size_t iOldSize = m_iSize;
		if (iNewSize <= iOldSize)
			return ;

		TYPE* lpResource = this->_resource;
		this->_resource = InitializeResource (iNewSize);
		memcpy (this->_resource, lpResource, iOldSize);
		memset (this->_resource + iOldSize, 0, iNewSize - iOldSize);
		DeleteMatrix (lpResource);
	}


private:
	TYPE* InitializeResource (size_t iSize)
	{
		TYPE* lpResource = NULL;
		if (iSize)
		{
//			NEW (lpResource, TYPE[iSize]);

    		try
    		{
    			lpResource = new TYPE[iSize]; 
    		}
    		catch (...)
    		{
    			lpResource = NULL;
    			OutputDebugString ("*** 内存申请失败 ***" NEW_LINE);
    		}
		}
		m_iSize = iSize;
		return lpResource;
	}

protected:
	size_t	m_iSize;
};

typedef TArray<TCHAR> TArrayString;
typedef TArray<WCHAR> TArrayWString;
/************************************************************************/
/* 指针容器,并且自动释放指针
/************************************************************************/

template <typename TYPE>
class TPointerContainer
{
public:
	typedef TYPE TPTRCONTAINER;

	virtual ~TPointerContainer()
	{
		Clear();
	}

	virtual void Clear()
	{
		PointerDeleteAll<TYPE>(m_Container);
	}

	template <typename Iterator>
	Iterator Erase (Iterator iter)
	{
		return PointerDeleteOne (m_Container, iter);
	}

	TYPE& Container ()
	{
		return m_Container;
	}

protected:
	TYPE m_Container;
};

}

#endif
