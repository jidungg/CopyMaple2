#pragma once
#include "Base.h"
BEGIN(Engine)
class CGameObject;
template<class T>
class CObjectPool :
    public CBase
{
private:
	explicit CObjectPool(T* pOriginal, void* pArg, _uint iCloneCount = 1);
	virtual ~CObjectPool() = default;

public:
	T* Get_Object();
	void Return_Object(T* pGameObject);
protected:
	T* m_pOriginal = { nullptr };
	void* m_pArg = { nullptr };
	_uint m_iCloneCOunt = 1;
	queue<T*> m_queStockObject;
	list<T*> m_listLentObject;
public:
	static CObjectPool* Create(T* pOriginal, void* pArg, _uint iCloneCount = 1)
	{
		CObjectPool* pInstance = new CObjectPool(pOriginal, pArg, iCloneCount);

		return pInstance;
	}
	virtual void Free() override;
};




template<class T>
inline CObjectPool<T>::CObjectPool(T* pOriginal, void* pArg, _uint iCloneCount)
{
	m_pOriginal = pOriginal;
	Safe_AddRef(m_pOriginal);
	m_iCloneCOunt = iCloneCount;
	m_pArg = pArg;
	for (_uint i = 0; i < m_iCloneCOunt; ++i)
	{
		T* pClone = static_cast<T*>(m_pOriginal->Clone(m_pArg));
		m_queStockObject.push(pClone);
		Safe_AddRef(pClone);
	}
}

template<class T>
inline T* CObjectPool<T>::Get_Object()
{
	if (m_queStockObject.empty())
	{
		T* pClone = static_cast<T*>(m_pOriginal->Clone(m_pArg));
		m_queStockObject.push(pClone);
		Safe_AddRef(pClone);
	}
	T* pObject = m_queStockObject.front();
	m_queStockObject.pop();
	m_listLentObject.push_back(pObject);

	return pObject;
}

template<class T>
inline void CObjectPool<T>::Return_Object(T* pGameObject)
{
	auto iter = find_if(m_listLentObject.begin(), m_listLentObject.end(), [&pGameObject](T* pObject) {return pObject == pGameObject; });
	if (iter == m_listLentObject.end())
		return;

	m_queStockObject.push(*iter);
	m_listLentObject.erase(iter);
}


template<class T>
inline void CObjectPool<T>::Free()
{
	__super::Free();
	while (!m_queStockObject.empty())
	{
		Safe_Release(m_queStockObject.front());
		m_queStockObject.pop();
	}

	for (T* pObject : m_listLentObject)
	{
		Safe_Release(pObject);
	}
	m_listLentObject.clear();
	Safe_Release(m_pOriginal);
}
END