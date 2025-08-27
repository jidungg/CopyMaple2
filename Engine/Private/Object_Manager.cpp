#include "..\Public\Object_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Layer.h"
#include "UIObject.h"
#include "Collider.h"

CObject_Manager::CObject_Manager()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(LEVEL_ID iPrototypeLevelIndex, const _wstring & strPrototypeTag, LEVEL_ID iLevelIndex, LAYER_ID iLayerId, void * pArg, bool bDontDestroy)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	return Add_GameObject_ToLayer(iLevelIndex, iLayerId, pGameObject, bDontDestroy);
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(LEVEL_ID iLevelIndex, LAYER_ID iLayerId, CGameObject* pObj, bool bDontDestroy)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerId);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		m_pLayers[iLevelIndex].emplace(iLayerId, pLayer);
	}

	pLayer->Add_GameObject(pObj);

		pObj->Set_DontDestroy(bDontDestroy);
	pObj->Set_LayerID(iLayerId);
	CUIObject* pUI = dynamic_cast<CUIObject*>(pObj);
	if(pUI && pUI->Is_RegisterUIManager())
	{
		if (bDontDestroy)
			m_pGameInstance->Register_DontDestroy_UIObject(pUI);
		else
			m_pGameInstance->Register_UIObject(pUI);
	}

	return S_OK;
}




void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])		
			Pair.second->Priority_Update(fTimeDelta);		
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}

}

void CObject_Manager::Final_Update()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Final_Update();
	}
}

void CObject_Manager::Clear(LEVEL_ID iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& pLayerPair : m_pLayers[iLevelIndex])
	{

		Safe_Release(pLayerPair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

void CObject_Manager::Move_DontDestroyObjects(LEVEL_ID iOldLevel, LEVEL_ID iNewLevel)
{
	for (auto& pLayer : m_pLayers[iOldLevel])
	{
		pLayer.second->HandOver_DontDestroyObjects(&m_DontDestroyLevel[pLayer.first]);
	}
	for (auto& LayerPair : m_DontDestroyLevel)
	{
		for (auto& pObj : LayerPair.second)
		{
			Add_GameObject_ToLayer(iNewLevel, LayerPair.first, pObj, true);
			pObj = nullptr;
		}
		LayerPair.second.clear();
	}
	m_DontDestroyLevel.clear();
}

bool CObject_Manager::RayCast(LAYER_ID iLayerId, const Ray& tRay, RaycastHit* pOut)
{
	return m_pLayers[m_pGameInstance->Get_CurrentLevelID()][iLayerId]->Check_Collision(tRay, pOut);
}

bool CObject_Manager::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	bool bIsHit = false;
	RaycastHit tMinHit;
	tMinHit.fDist = 9999;
	for (auto& Pair : m_pLayers[m_pGameInstance->Get_CurrentLevelID()])
	{
		if (Pair.second->Check_Collision(tRay, pOut))
		{
			bIsHit = true;
			if (pOut->fDist < tMinHit.fDist)
				tMinHit = *pOut;
		}
	}
	if (bIsHit)
		*pOut = tMinHit;
	return bIsHit;
}

void CObject_Manager::Check_Collision(LAYER_ID iLayerId, CGameObject* pObject, list<CGameObject*>* pOutList)
{
	list<CGameObject*>* pList = Get_GameObjectList(iLayerId);
	for (auto& pObj : *pList)
	{
		if (pObj->Is_Dead())
			continue;
		if (pObject->Check_Collision(pObj))
			pOutList->push_back(pObj);
	}
}

CGameObject* CObject_Manager::Get_FirstGameObject(LEVEL_ID iLevIdx, LAYER_ID iLayerId)
{
	CLayer* pLayer= Find_Layer(iLevIdx, iLayerId);
	
	return pLayer->Get_FirstGameObject();
}

list<CGameObject*>* CObject_Manager::Get_GameObjectList(LAYER_ID iLayerId)
{
	auto pLayer = Find_Layer(m_pGameInstance->Get_CurrentLevelID(), iLayerId);
	if (pLayer == nullptr)
		return nullptr;
	auto pList = pLayer->Get_GameObjectList();
	return pList;
}

CLayer * CObject_Manager::Find_Layer(LEVEL_ID iLevelIndex, LAYER_ID iLayerId)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(iLayerId);
	if (iter == m_pLayers[iLevelIndex].end())		
		return nullptr;

	return iter->second;

}


CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*		pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}
	for (auto& Pair : m_DontDestroyLevel)
	{
		for (auto& pObj : Pair.second)
			Safe_Release(pObj);
		Pair.second.clear();
	}
	m_DontDestroyLevel.clear();
	Safe_Delete_Array(m_pLayers);

	Safe_Release(m_pGameInstance);
}

