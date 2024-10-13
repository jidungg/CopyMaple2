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
		if (pGameObject->Is_Active())
			pGameObject->Priority_Update(fTimeDelta);

}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Is_Active())
			pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Is_Active())
			pGameObject->Late_Update(fTimeDelta);
}

bool CLayer::Check_Collision(const Ray& tRay, RaycastHit* pOut)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject->Is_Active() == false || pGameObject->Is_Dead())
			continue;
		return	pGameObject->Check_Collision(tRay, pOut);
	}
	return false;
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
