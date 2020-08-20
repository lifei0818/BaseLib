#ifndef DEQUESERVICE_H
#define DEQUESERVICE_H

#include "common/sys_incl.h"
#include "common/mutex.h"

#ifdef _MACRO_OS_WIN32
typedef HANDLE EVENTHANDLE;
#else
typedef sem_t* EVENTHANDLE;
#endif

///	���з���ģ��
template
<class T,class M=BaseLib::CThreadMutex>
class CDequeService
{
public:
	typedef std::deque<T*> CONTAINER;
	CDequeService ()
		: m_hDequeEvent (NULL)
	{}

	virtual ~CDequeService() {}

	///	��ӵ�����
	virtual void Push (T* lpObject, BOOL bFront = FALSE)
	{
		if (lpObject)
		{
			BaseLib::TGuard<M> guard(&m_dequeMutex);
			if (bFront)
				m_deque.insert (m_deque.begin(), lpObject);
			else
				m_deque.push_back (lpObject);

			if (m_hDequeEvent)
			{
#ifdef _MACRO_OS_WIN32
				::SetEvent (m_hDequeEvent);
#else
                ::sem_post(m_hDequeEvent);
#endif
			}
		}
	}

	/// ���ظ����
	virtual void PushOnce (T* lpObject)
	{
		if (lpObject)
		{
			BaseLib::TGuard<M> guard(&m_dequeMutex);
			for (typename CONTAINER::iterator iter = Container().begin();
				iter != Container().end();
				iter++)
			{
				if (lpObject == *iter)
				{
					return ;
				}
			}

			Container().push_back (lpObject);
		}
	}

	/// �Ӷ�����ȡ��
	T* Pop ()
	{
		T* lpObject = NULL;
		BaseLib::TGuard<M> guard(&m_dequeMutex);

		if (!m_deque.empty())
		{
			lpObject = m_deque.front();
			m_deque.pop_front();
		}
		return lpObject;
	}

	/// �Ӷ���ɾ��
	virtual BOOL Erase (T* lpObject)
	{
		if (lpObject)
		{
			BaseLib::TGuard<M> guard(&m_dequeMutex);
			for (typename CONTAINER::iterator iter = m_deque.begin();
				iter != m_deque.end();
				iter++)
			{
				if ((*iter) == lpObject)
				{
					Container().erase (iter);
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	/// �Ӷ��в�ѯ
	virtual BOOL Query (T* lpObject)
	{
		if (lpObject)
		{
			BaseLib::TGuard<M> guard(&m_dequeMutex);
			for (typename CONTAINER::iterator iter = m_deque.begin();
				iter != m_deque.end();
				iter++)
			{
				if ((*iter) == lpObject)
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}
	EVENTHANDLE DequeEvent() const { return m_hDequeEvent; }
	void DequeEvent(EVENTHANDLE val) 
	{
		BaseLib::TGuard<M> guard(&m_dequeMutex);
		m_hDequeEvent = val; 
	}

	CONTAINER& Container() { return m_deque; }

protected:
	CONTAINER m_deque;
	M m_dequeMutex;
	EVENTHANDLE m_hDequeEvent;
};
#endif
