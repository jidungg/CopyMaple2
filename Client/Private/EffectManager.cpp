#include "stdafx.h"
#include "EffectManager.h"
#include "Effect.h"
#include "GameInstance.h"
#include "ObjectPool.h"

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
	return S_OK;
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
			//CEffModelObject::EFFECTOBJ_DESC tDesc(jData);
			//LEVELID eLevel = jData["LevelProto"];
			//CEffModelObject* pObj = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, (_uint)eLevel, CEffModelObject::m_szProtoTag, &tDesc));
			//m_mapEffectModel[jData["Id"]] =CObjectPool< CEffModelObject>::Create(pObj, &tDesc,1);
			//break;
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
	pObj->Set_Transform(vPos, vRotation, fScale);
	pObj->Start_Animation();
}

void CEffectManager::Play_EffectModel(EFF_MODEL_ID eID, CGameObject* pTarget, _vector vPos, _vector vRotation, _float fScale)
{
	if (nullptr == m_mapEffectModel[eID])
		return;
		
	CEffModelObject* pObj = static_cast<CEffModelObject*>(m_mapEffectModel[eID]->Get_Object());
	pObj->Set_Transform(vPos, vRotation, fScale);
	pObj->Start_Animation();

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
