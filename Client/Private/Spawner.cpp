#include "stdafx.h"
#include "Spawner.h"
#include "GameInstance.h"
#include "Monster.h"
#include "MonsterDataBase.h"

CSpawner::CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTerrainObject(pDevice, pContext)
{
}

CSpawner::CSpawner(const CSpawner& Prototype)
	: CTerrainObject(Prototype)
{
}


HRESULT CSpawner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpawner::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	SPAWNER_DESC* pDesc = (SPAWNER_DESC*)pArg;
	fSpawnTime = pDesc->vecFData.front();
	m_iObjectId = pDesc->vecIData.front();
	if (FAILED(Ready_SpawnObject(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CSpawner::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pSpawnObject->Is_Dead()	)
	{
		m_pSpawnObject->Set_Dead(false);
		m_pSpawnObject->Set_Active(false);
		m_pSpawnObject->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		fTimeAcc = 0.f;
	}
	else if(false == m_pSpawnObject->Is_Active())
	{
		if (fSpawnTime <= fTimeAcc)
		{
			fTimeAcc = 0.f;
			static_cast<CCharacter*>(m_pSpawnObject)->Respawn();
		}
		else
		{
			fTimeAcc += fTimeDelta;
		}
	}

}

HRESULT CSpawner::Render()
{
	return S_OK;
}

json CSpawner::ToJson()
{
	json j;
	j["ItemId"] = m_eBuildItemID;
	vector<_int> iData = {(_int)m_iObjectId};
	j["IntData"] = iData;
	vector<_float> fData = {fSpawnTime};
	j["FloatData"] = fData;
	j["Iteration"] = 1;
	j["Direction"] = m_eTerrainDir;
	j["Index"] = m_iIndex;
	return j;
}


void CSpawner::Free()
{
	__super::Free();
	Safe_Release(m_pSpawnObject);
}
