#include "..\Public\Object_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Layer.h"
#include "UIObject.h"

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

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg, bool bDontDestroy)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	return Add_GameObject_ToLayer(iLevelIndex, strLayerTag, pGameObject, bDontDestroy);
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* pObj, bool bDontDestroy)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}

	pLayer->Add_GameObject(pObj);

	pObj->Set_DontDestroy(bDontDestroy);
	CUIObject* pUI = dynamic_cast<CUIObject*>(pObj);
	if(pUI)
		m_pGameInstance->Register_UIObject(pUI);

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
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}

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

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& pLayerPair : m_pLayers[iLevelIndex])
	{
		pLayerPair.second->Get_DontDestroyObjects(&m_DontDestroyLevel[pLayerPair.first]);

		Safe_Release(pLayerPair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

void CObject_Manager::On_OpenLevel(_uint iLevelIndex)
{
	for (auto& pObjectPair : m_DontDestroyLevel)
	{
		for (auto& pObj : pObjectPair.second)
		{
			Add_GameObject_ToLayer(iLevelIndex, pObjectPair.first, pObj);
			pObj = nullptr;
		}
	}
}

bool CObject_Manager::RayCast(const _wstring& strLayerTag, const Ray& tRay, RaycastHit* pOut)
{
	return m_pLayers[m_pGameInstance->Get_CurrentLevelID()][strLayerTag]->Check_Collision(tRay, pOut);
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

CGameObject* CObject_Manager::Get_FirstGameObject(_uint iLevIdx, const _wstring& strLayerTag)
{
	CLayer* pLayer= Find_Layer(iLevIdx, strLayerTag);
	
	return pLayer->Get_FirstGameObject();
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);
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

