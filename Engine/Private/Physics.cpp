#include "Physics.h"
#include "GameInstance.h"

CPhysics::CPhysics(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY)
	: m_pGameInstance(pGameInstance),
	m_iWinSizeX(iWinSizeX),
	m_iWinSizeY(iWinSizeY)
{
}


HRESULT CPhysics::Initialize()
{
	return S_OK;
}

CPhysics* CPhysics::Create(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY)
{
	CPhysics* pInstance = new CPhysics(pGameInstance, iWinSizeX, iWinSizeY);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CPhysics");
		Safe_Release(pInstance);
	}

	return pInstance;
}
bool CPhysics::RayCast(const POINT& tScreenPos, CCollider* pCollider, RaycastHit* pOut)
{
	POINT tPosition = m_pGameInstance->Get_MousePos();
	Ray ray{ tPosition ,m_iWinSizeX, m_iWinSizeY,
		m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW),
		m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW) };

	return RayCast(ray, pCollider, pOut);
}
bool CPhysics::RayCast(const XMFLOAT4& vWorldOrigin, const XMFLOAT4& vWorldDir, CCollider* pCollider, RaycastHit* pOut)
{
	Ray ray{ vWorldOrigin, vWorldDir };

	return RayCast(ray, pCollider,pOut);
}
bool CPhysics::RayCast(const Ray& tRay, CCollider* pCollider, RaycastHit* pOut)
{
	return false;
}


void CPhysics::Free()
{
	__super::Free();
}
