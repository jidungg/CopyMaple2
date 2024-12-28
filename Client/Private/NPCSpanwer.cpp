#include "stdafx.h"
#include "NPCSpanwer.h"
#include "NPC.h"
#include "GameInstance.h"
#include "Client_Utility.h"

CNPCSpanwer::CNPCSpanwer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpawner(pDevice, pContext)
{
}

CNPCSpanwer::CNPCSpanwer(const CNPCSpanwer& Prototype)
	: CSpawner(Prototype)
{
}

HRESULT CNPCSpanwer::Ready_SpawnObject(CSpawner::SPAWNER_DESC* pDesc)
{
	m_iObjectId = pDesc->vecIData.front();
	fSpawnTime = pDesc->vecFData.front();
	CNPC::NPCDesc tNPCDesc;
	tNPCDesc.eNPCId = (NPC_ID)m_iObjectId;
	tNPCDesc.pCubeTerrain = pDesc->pCubeTerrain;
	tNPCDesc.vHomePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pSpawnObject = static_cast<CNPC*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CNPC::m_szProtoTag, &tNPCDesc));
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_INTERACTION, m_pSpawnObject);

	m_pSpawnObject->Get_Transform()->Set_State(CTransform::STATE_POSITION,Get_TransformPosition());
	m_pSpawnObject->Get_Transform()->LookToward(Get_Direction_Vector(m_eTerrainDir));


	return S_OK;
}

CTerrainObject* CNPCSpanwer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNPCSpanwer* pInstance = new CNPCSpanwer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNPCSpanwer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPCSpanwer::Clone(void* pArg)
{
	CNPCSpanwer* pInstance = new CNPCSpanwer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNPCSpanwer");
		Safe_Release(pInstance);
	}

	return pInstance;
}
