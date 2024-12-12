#include "stdafx.h"
#include "EffectManager.h"
#include "Effect.h"
#include "GameInstance.h"
#include "ObjectPool.h"
#include "Client_Utility.h"
#include "UIDamgCount.h"

IMPLEMENT_SINGLETON(CEffectManager)

CEffectManager::CEffectManager()
{
	m_pGameInstance = CGameInstance::GetInstance();
}


HRESULT CEffectManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (_uint i = 0; i < (_uint)EFF_MODEL_ID::LAST; ++i)
	{
		m_mapEffectModel.insert(std::make_pair((EFF_MODEL_ID)i, nullptr));
	}

	CUIDamgCount:: UIDMGCOUNT_DESC tUIDamgCountDesc;
	tUIDamgCountDesc.fLifeTime = 1.f;
	tUIDamgCountDesc.fRisingSpeed = 0.5f;
	tUIDamgCountDesc.iDamg = 0;
	for (_uint i = 0; i < (_uint)DAMG_TYPE::LAST; i++)
	{
		tUIDamgCountDesc.eDamgType = DAMG_TYPE(i);
		CUIDamgCount* pDmgCount = static_cast<CUIDamgCount*>( m_pGameInstance->Clone_Proto_Object_Stock(CUIDamgCount::m_szProtoTag, &tUIDamgCountDesc));
		m_DmgCountPool[i] = CObjectPool<CUIDamgCount>::Create(pDmgCount, &tUIDamgCountDesc, 20);
	}

	return S_OK;
}
void CEffectManager::Update(_float fTimeDelta)
{
	auto& iterEnd = m_listPlayingEffect.end();
	for (auto& iter = m_listPlayingEffect.begin(); iter != iterEnd;)
	{
		EFF_MODEL_ID eID = iter->first;
		CEffModelObject* pObj = iter->second;
		if (pObj->Is_Active())
		{
			pObj->Update(fTimeDelta);
			m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, pObj);
			++iter;
		}
		else
		{
			m_mapEffectModel[eID]->Return_Object(pObj);
			iter = m_listPlayingEffect.erase(iter);
		}
	}

}
HRESULT CEffectManager::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/EffectData.json", &j)))
		return E_FAIL;
	for (auto& jData : j["Effects"])
	{
		EFF_TYPE eType = jData["Type"];
		switch (eType)
		{
		case Client::EFF_TYPE::MODEL:
		{
			CEffModelObject::EFFECTOBJ_DESC tDesc(jData);
			LEVELID eLevel = jData["LevelProto"];
			CEffModelObject* pObj = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, (_uint)eLevel, CEffModelObject::m_szProtoTag, &tDesc));
			m_mapEffectModel[jData["Id"]] =CObjectPool< CEffModelObject>::Create(pObj, &tDesc,20);
			break;
		}
		case Client::EFF_TYPE::SOUND:
			break;
		case Client::EFF_TYPE::DAMAGE_FONT:
			break;
		case Client::EFF_TYPE::VIBRATION:
			break;
		case Client::EFF_TYPE::LAST:
			break;
		default:
			break;
		}

	}
	return S_OK;
}

void CEffectManager::Play_EffectModel(EFF_MODEL_ID eID, _vector vPos, _vector vRotation, _float fScale)
{
	if (nullptr == m_mapEffectModel[eID])
		return;
	CEffModelObject* pObj =static_cast<CEffModelObject*>(m_mapEffectModel[eID]->Get_Object());
	//m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYERID::LAYER_NONCOLLISION, pObj);
	m_listPlayingEffect.push_back({ eID,pObj });
 	pObj->Set_Transform(vPos, vRotation, fScale);
	pObj->Start_Animation();
}

void CEffectManager::Play_DamgCount(DAMG_TYPE eID, _int iDamg, _vector vPos, _vector vRotation, _float fScale)
{
	if (nullptr == m_DmgCountPool[(_uint)eID])
		return;
	CUIDamgCount* pObj = m_DmgCountPool[(_uint)eID]->Get_Object();
	m_listPlayingDamgCount.push_back({ eID,pObj });
	pObj->Set_Transform(vPos, vRotation, fScale);
	pObj->Set_Damge(iDamg);
	pObj->Start();
}



void CEffectManager::Free()
{
	__super::Free();
	for (auto& pair : m_mapEffectModel)
	{
		Safe_Release(pair.second);
	}
	m_mapEffectModel.clear();
}
