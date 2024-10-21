#include "..\Public\Layer.h"
#include "GameObject.h"
#include "Collider.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Is_Dead() == false && pGameObject->Is_Active())
			pGameObject->Priority_Update(fTimeDelta);

}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Is_Dead() == false && pGameObject->Is_Active())
			pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Is_Dead() == false && pGameObject->Is_Active())
			pGameObject->Late_Update(fTimeDelta);
}

void CLayer::Final_Update()
{
	for (auto& m_GameObjects_iter = m_GameObjects.begin(); m_GameObjects_iter != m_GameObjects.end();)
	{
		if ((*m_GameObjects_iter)->Is_Dead())
		{
			Safe_Release(*m_GameObjects_iter);
			m_GameObjects_iter = m_GameObjects.erase(m_GameObjects_iter);
		}
		else
		{
			(*m_GameObjects_iter)->Final_Update();
			++m_GameObjects_iter;
		}
	}

}

bool CLayer::Check_Collision(const Ray& tRay, RaycastHit* pOut)
{
	bool bIsHit = false;
	RaycastHit tMinHit;
	tMinHit.fDist = 9999;
	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject->Is_Active() == false || pGameObject->Is_Dead())
			continue;

		if (pGameObject->Check_Collision(tRay, pOut))
		{
			bIsHit = true;
			if (pOut->fDist < tMinHit.fDist)
				tMinHit = *pOut;
		}
	}
	if (bIsHit)
		*pOut = tMinHit;
	return bIsHit;
}

CLayer * CLayer::Create()
{
	 return new CLayer();
}


void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
