#include "stdafx.h"
#include "UIQuickSlot.h"
#include "GameInstance.h"
#include "SlotItem.h"
CUIQuickSlot::CUIQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUISlot(pDevice, pContext)
{
}

CUIQuickSlot::CUIQuickSlot(const CUIQuickSlot& Prototype)
	: CUISlot(Prototype )
	, m_pQuickItem(Prototype.m_pQuickItem)
{
}

HRESULT CUIQuickSlot::Initialize_Prototype()
{
  	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, L"Texture_QuickSlot_Normal"));

	return S_OK;
}

HRESULT CUIQuickSlot::Initialize(void* pArg)
{
	QUICKLSLOT_DESC* pDesc = static_cast<QUICKLSLOT_DESC*>(pArg);
	m_eHotKey = pDesc->eHotKey;

	pDesc->tIconDesc.eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.ePivotType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.fSizeX = pDesc->fSizeX ;
	pDesc->tIconDesc.fSizeY = pDesc->fSizeY ;
	pDesc->tIconDesc.fXOffset = 0;
	pDesc->tIconDesc.fYOffset = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUIQuickSlot::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
	m_pQuickItem->Update_Cool(fTimeDelta);
}




void CUIQuickSlot::Receive_Input()
{
	if (m_pQuickItem)
		m_pQuickItem->Use();
}

void CUIQuickSlot::On_MouseClick()
{
	if (m_pQuickItem)
		m_pQuickItem->Use();
}

void CUIQuickSlot::Set_QuickItem(IQuickItem* pItem)
{
	m_pQuickItem = pItem;
	if (pItem == nullptr)
	{
		Set_IconTexture(nullptr);
		
	}
	else
	{
		string str = m_pQuickItem->Get_IconTag();
		wstring wstrIconTag(str.begin(), str.end());
		CTexture* pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, wstrIconTag));

		Set_IconTexture(pTextureCom);
	}
}


CUIQuickSlot* CUIQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIQuickSlot* pInstance = new CUIQuickSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CQuickSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIQuickSlot::Clone(void* pArg)
{
	CUIQuickSlot* pInstance = new CUIQuickSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CQuickSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIQuickSlot::Free()
{
	__super::Free();

}

bool CUIQuickSlot::Has_Item()
{
	return m_pQuickItem != nullptr;
}
