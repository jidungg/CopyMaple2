#include "stdafx.h"
#include "UIQuickSlotBundle.h"
#include "UIQuickSlot.h"
#include "GameInstance.h"

CUIQuickSlotBundle::CUIQuickSlotBundle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUIQuickSlotBundle::CUIQuickSlotBundle(const CUIQuickSlotBundle& Prototype)
	: CUIContainer(Prototype)
{
}

HRESULT CUIQuickSlotBundle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIQuickSlotBundle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	QUICKSLOTBUNDLE_DESC* pBundleDesc = static_cast<QUICKSLOTBUNDLE_DESC*>(pArg);


	CUIQuickSlot::QUICKLSLOT_DESC quickSlotDesc;
	quickSlotDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	quickSlotDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	quickSlotDesc.fSizeX = pBundleDesc->fSlotSize;
	quickSlotDesc.fSizeY = pBundleDesc->fSlotSize;
	_uint iSlotCount = pBundleDesc->vecHotKey.size();
	assert(iSlotCount == pBundleDesc->iSlotColCount * pBundleDesc->iSlotRowCount);
	for (_uint i = 0; i < iSlotCount; i++)
	{
		_uint iRow = i / pBundleDesc->iSlotColCount;
		_uint iCol = i % pBundleDesc->iSlotColCount;
		quickSlotDesc.fXOffset = iCol * (pBundleDesc->fSlotSize + pBundleDesc->fSlotMargin/2);
		quickSlotDesc.fYOffset = iRow * (pBundleDesc->fSlotSize + pBundleDesc->fSlotMargin/2);
		quickSlotDesc.eHotKey = pBundleDesc->vecHotKey[i];
		m_mapQuickSlot[quickSlotDesc.eHotKey] = static_cast<CUIQuickSlot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIQuickSlot::m_szProtoTag, &quickSlotDesc));
	}

	for (auto& pQuickSlot : m_mapQuickSlot)
	{
		Add_Child(pQuickSlot.second);
	}


	return S_OK;
}

void CUIQuickSlotBundle::Update(_float fTImeDelta)
{
	for (auto& pQuickSlot : m_mapQuickSlot)
	{
		if(pQuickSlot.second && pQuickSlot.second->Is_Valid())
			pQuickSlot.second->Update(fTImeDelta);
	}
}

void CUIQuickSlotBundle::Recevie_Input(KEY eKey)
{
	if (m_mapQuickSlot.find(eKey) != m_mapQuickSlot.end())
		m_mapQuickSlot[eKey]->Receive_Input();
}

void CUIQuickSlotBundle::Set_QuickItem(KEY eHotKey, IQuickItem* pItem)
{
	m_mapQuickSlot[eHotKey]->Set_QuickItem(pItem);
}

CUIQuickSlotBundle* CUIQuickSlotBundle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIQuickSlotBundle* pInstance = new CUIQuickSlotBundle(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CQuickSlotBundle");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUIQuickSlotBundle::Clone(void* pArg)
{
	CUIQuickSlotBundle* pInstance = new CUIQuickSlotBundle(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CQuickSlotBundle");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUIQuickSlotBundle::Free()
{
	__super::Free();
}
