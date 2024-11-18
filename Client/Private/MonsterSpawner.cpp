#include "stdafx.h"
#include "MonsterSpawner.h"
#include "Monster.h"	
#include "GameInstance.h"
#include "MonsterDataBase.h"
#include "Client_Utility.h"

CMonsterSpawner::CMonsterSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpawner(pDevice, pContext)
{
}

CMonsterSpawner::CMonsterSpawner(const CMonsterSpawner& Prototype)
	: CSpawner(Prototype)
{
}

HRESULT CMonsterSpawner::Ready_SpawnObject(CSpawner::SPAWNER_DESC* pDesc)
{
	m_iObjectId = pDesc->vecIData.front();
	fSpawnTime = pDesc->vecFData.front();
	CMonster::MONSTER_DESC tMonDesc;
	tMonDesc.eMonID = (MONSTER_ID)m_iObjectId;
	tMonDesc.vHomePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	tMonDesc.pCubeTerrain =pDesc->pCubeTerrain;
	if (tMonDesc.eMonID == MONSTER_ID::BAYAR)
		m_pSpawnObject = static_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, Get_CurrentTrueLevel(), TEXT("Prototype_GameObject_Bayar"), &tMonDesc));
	else
		m_pSpawnObject = static_cast<CMonster*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CMonster::m_szProtoTag,&tMonDesc));
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_MONSTER, m_pSpawnObject);
	Safe_AddRef(m_pSpawnObject);
    return S_OK;
}

CTerrainObject* CMonsterSpawner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterSpawner* pInstance = new CMonsterSpawner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonsterSpawner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterSpawner::Clone(void* pArg)
{
	CMonsterSpawner* pInstance = new CMonsterSpawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonsterSpawner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

