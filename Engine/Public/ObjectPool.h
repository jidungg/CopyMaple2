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
	void Retrive_False_Object(_bool(*func)(T* ));
	T* Get_Object();
	void Return_Object(T* pGameObject);
	void Update_LentObject(_float fTimeDelta);
	void LateUpdate_LentObject(_float fTimeDelta);
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
	m_iCloneCOunt = iCloneCount;
	m_pArg = pArg;
	for (_uint i = 0; i < m_iCloneCOunt; ++i)
	{
		T* pClone = static_cast<T*>(m_pOriginal->Clone(m_pArg));
		m_queStockObject.push(pClone);
	}
}



template<class T>
inline void CObjectPool<T>::Retrive_False_Object(_bool(*func)(T*))
{
	for (auto& iter = m_listLentObject.begin(); iter != m_listLentObject.end(); )
	{
		if (false == func(*iter))
		{
			m_queStockObject.push(*iter);
			iter = m_listLentObject.erase(iter);
			//cout << "Retrieved. Stock :" << m_queStockObject.size() << ", Lent :" << m_listLentObject.size() << endl;
		}
		else
		{
			++iter;
		}
	}
}

template<class T>
inline T* CObjectPool<T>::Get_Object()
{
	assert(false == m_queStockObject.empty());

	T* pObject = m_queStockObject.front();
	m_queStockObject.pop();
	m_listLentObject.push_back(pObject);
	//cout << "Lented. Stock :" << m_queStockObject.size() << ", Lent :" << m_listLentObject.size() << endl;
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
inline void CObjectPool<T>::Update_LentObject(_float fTimeDelta)
{
	for (auto& pObj : m_listLentObject)
	{
		if(pObj->Is_Valid())
			pObj->Update(fTimeDelta);
	}
}

template<class T>
inline void CObjectPool<T>::LateUpdate_LentObject(_float fTimeDelta)
{
	for (auto& pObj : m_listLentObject)
	{
		if (pObj->Is_Valid())
			pObj->Late_Update(fTimeDelta);
	}
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