#include "stdafx.h"
#include "UIPlayerInfoSlot.h"
#include "UIIcon.h"
#include "GameInstance.h"
#include "UIPlayerInfo.h"
#include "SlotItem.h"
#include "PlayerInfoSlot.h"
#include "PlayerInfo.h"

CUIPlayerInfoSlot::CUIPlayerInfoSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUISlot(pDevice, pContext)
{
}

CUIPlayerInfoSlot::CUIPlayerInfoSlot(const CUIPlayerInfoSlot& Prototype)
	:CUISlot(Prototype)
{
}
HRESULT CUIPlayerInfoSlot::Initialize(void* pArg)
{
	UIPLAYERINFO_SLOT_DESC* pDesc = static_cast<UIPLAYERINFO_SLOT_DESC*>(pArg);
	m_eSlotId = pDesc->eSlotId;
	m_pIUPlayerInfo = pDesc->pPlayerInfo;

	switch (m_eSlotId)
	{
	case Client::CUIPlayerInfo::HAT:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("playerinfo_slot_hat.dds"), nullptr));
		break;
	case Client::CUIPlayerInfo::TOP:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_top.dds")));
		break;
	case Client::CUIPlayerInfo::BOTTOM:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_bot.dds")));
		break;
	case Client::CUIPlayerInfo::GLOVES:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_glove.dds")));
		break;
	case Client::CUIPlayerInfo::SHOES:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_shoe.dds")));
		break;
	case Client::CUIPlayerInfo::CAPE:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_cape.dds")));
		break;
	case Client::CUIPlayerInfo::WEAPON:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_weapon.dds")));
		break;
	case Client::CUIPlayerInfo::EAR:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_ear.dds")));
		break;
	case Client::CUIPlayerInfo::FACE:
		pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("playerinfo_slot_face.dds")));
		break;
	case Client::CUIPlayerInfo::SLOT_LAST:
		break;
	default:
		break;
	}

	pDesc->tIconDesc.eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.ePivotType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.fSizeX = 60;
	pDesc->tIconDesc.fSizeY = 60;
	pDesc->tIconDesc.fXOffset = 0;
	pDesc->tIconDesc.fXOffset = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CUIIcon::PANEL_DESC tPanelDesc;
	tPanelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tPanelDesc.ePivotType = CORNOR_TYPE::CENTER;
	tPanelDesc.fSizeX = 60;
	tPanelDesc.fSizeY = 60;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Texture_PlayerInfoSlotBack")));
	m_pBackPanel = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pBackPanel);

	Set_Item(nullptr);
	return S_OK;
}
HRESULT CUIPlayerInfoSlot::Render()
{
	if (m_iSRVIndex != UINT_MAX)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;
		if (m_pShaderCom)
			m_pShaderCom->Begin(0);
		if (m_pVIBufferCom)
		{
			m_pVIBufferCom->Bind_BufferDesc();
			m_pVIBufferCom->Render();
		}
	}
	if (m_pBackPanel->Is_Active())
		m_pBackPanel->Render();
	if (m_pIcon->Is_Active())
		m_pIcon->Render();
	return S_OK;
}
bool CUIPlayerInfoSlot::Has_Item()
{
	return nullptr == m_pItemData;
}


void CUIPlayerInfoSlot::On_MouseEnter()
{
	__super::On_MouseEnter();
	m_pIUPlayerInfo->Select_Slot(m_eSlotId);
}
void CUIPlayerInfoSlot::On_MouseRightClick()
{
	__super::On_MouseRightClick();
	switch (m_eSlotId)
	{
	case Client::CUIPlayerInfo::HAT:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::HAT);
		break;
	case Client::CUIPlayerInfo::TOP:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::TOP);
		break;
	case Client::CUIPlayerInfo::BOTTOM:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::BOTTOM);
		break;
	case Client::CUIPlayerInfo::GLOVES:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::GLOVES);
		break;
	case Client::CUIPlayerInfo::SHOES:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::SHOES);
		break;
	case Client::CUIPlayerInfo::CAPE:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::CAPE);
		break;
	case Client::CUIPlayerInfo::WEAPON:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::WEAPON);
		break;
	case Client::CUIPlayerInfo::EAR:
		PLAYERINIFO->UnEquip(EQUIP_ITEM_TYPE::EAR);
		break;
	case Client::CUIPlayerInfo::FACE:
		PLAYERINIFO->UnEquip(DECO_ITEM_TYPE::FACE);
		break;
	case Client::CUIPlayerInfo::SLOT_LAST:
		break;
	default:
		break;
	}

}

void CUIPlayerInfoSlot::Set_Item(const ITEM_DATA* pItem)
{
	m_pItemData = pItem;
	if (nullptr == m_pItemData)
	{
		m_pBackPanel->Set_Active(false);
		m_pIcon->Set_Active(false);
	}
	else
	{
		ITEM_GRADE eGrade = pItem->eItemGrade;
		m_pBackPanel->Set_SRVIndex((_uint)eGrade);
		CTexture* pTexure =static_cast<CTexture*>( m_pGameInstance->Clone_Proto_Component_Stock(pItem->szIconImageTag));
		m_pIcon->Set_Texture(pTexure);
		m_pBackPanel->Set_Active(true);
		m_pIcon->Set_Active(true);
	}
}


CUIPlayerInfoSlot* CUIPlayerInfoSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPlayerInfoSlot* pInstance = new CUIPlayerInfoSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfoSlot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIPlayerInfoSlot::Clone(void* pArg)
{
	CUIPlayerInfoSlot* pInstance = new CUIPlayerInfoSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfoSlot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIPlayerInfoSlot::Free()
{
	__super::Free();
}

