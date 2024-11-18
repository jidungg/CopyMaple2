#include "stdafx.h"
#include "PortalTerrainObject.h"
#include "Portal.h"
#include "GameInstance.h"
#include "Client_Utility.h"

CPortalTerrainObject::CPortalTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTerrainObject(pDevice, pContext)
{
}

CPortalTerrainObject::CPortalTerrainObject(const CPortalTerrainObject& Prototype)
	:CTerrainObject(Prototype)
{
}

HRESULT CPortalTerrainObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
    return S_OK;
}

HRESULT CPortalTerrainObject::Initialize(void* pArg)
{
	CTerrainObject::TERRAINOBJ_DESC* pDesc = (CTerrainObject::TERRAINOBJ_DESC*)pArg;
	pDesc->eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(pDesc->strModelProtoName, "EmptyModel.model");
	m_eLevelID = (LEVELID)pDesc->vecIData.front();
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CPortal::PORTAL_DESC tPortalDesc;
	tPortalDesc.eLevelID = m_eLevelID;
	CPortal* pPortal = static_cast<CPortal*>( m_pGameInstance->Clone_Proto_Object_Stock(CPortal::m_szProtoTag ,&tPortalDesc));
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(),LAYER_INTERACTION,pPortal);
	pPortal->Get_Transform()->Set_State(CTransform::STATE_POSITION, Get_Position());

    return S_OK;
}

json CPortalTerrainObject::ToJson()
{
	json j;
	j["ItemId"] = m_eBuildItemID;
	list<_int> iData;
	iData.push_back(m_eLevelID);
	j["IntData"] = iData;
	list<_float> fData;
	j["FloatData"] = fData;
	j["Iteration"] = 1;
	j["Direction"] = m_eTerrainDir;
	j["Index"] = m_iIndex;
	return j;
}

CPortalTerrainObject* CPortalTerrainObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortalTerrainObject* pInstance = new CPortalTerrainObject(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed CPortalTerrainObject ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CPortalTerrainObject::Clone(void* pArg)
{
	CGameObject* pInstance = new CPortalTerrainObject(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed CPortalTerrainObject ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
