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


void CPhysics::Free()
{
	__super::Free();
}
