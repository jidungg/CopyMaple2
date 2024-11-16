#include "stdafx.h"
#include "WayFinder.h"
#include "GameInstance.h"
#include "CubeTerrain.h"
#include "Client_Utility.h"
#include "TerrainObject.h"

CWayFinder::CWayFinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CWayFinder::CWayFinder(const CWayFinder& Prototype)
	:CComponent(Prototype)
{
}

HRESULT CWayFinder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWayFinder::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_pCubeTerrain = static_cast<CCubeTerrain*>( m_pGameInstance->Get_FirstGameObject(Get_CurrentTrueLevel(),LAYER_TERRAIN));
	if (nullptr == m_pCubeTerrain)
		return E_FAIL;
	return S_OK;
}

void CWayFinder::Update(_float fTimeDelta)
{
}

void CWayFinder::Late_Update(_float fTimeDelta)
{
}

void CWayFinder::MakeRoute(_uint iCurIdx, _uint iDestIdx)
{
	//AStart 알고리즘
	// 주변의 갈 수 있는 노드를 찾음.
	// 갈 수 있는 노드들 중에서 가장 가까운 노드를 선택
		//g : 시작점에서 현재 노드까지의 비용
		//h : 현재 노드에서 목적지까지의 비용
		//f = g + h
		//f가 가장 작은 노드를 선택

}


CWayFinder* CWayFinder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWayFinder* pInstance = new CWayFinder(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent* CWayFinder::Clone(void* pArg)
{
	CComponent* pInstance = new CWayFinder(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
