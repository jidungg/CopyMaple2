

#include "stdafx.h"
#include "UIBundle.h"
#include "GameInstance.h"

#include "UIQuickSlotBundle.h"
#include "UIQuickSlot.h"
#include "UIInventory.h"
#include "Inventory.h"
#include "UIBar.h"

IMPLEMENT_SINGLETON(CUIBundle)

CUIBundle::CUIBundle()
	: CUIContainer(nullptr, nullptr)
{

}

HRESULT CUIBundle::Initialize(void* pArg)
{
	UIBUNDLE_DESC* pDesc = static_cast<UIBUNDLE_DESC*>(pArg);
	m_pDevice = pDesc->pDevice;
	m_pContext = pDesc->pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUIInventory::UIINVENTORY_DESC tDesc;
	tDesc.pInventory = INVENTORY;
	m_pInventory = static_cast<CUIInventory*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIInventory::m_szProtoTag, &tDesc));
	Add_Child(m_pInventory);	
	m_pInventory->Set_Active(false);
	Safe_AddRef(m_pInventory);

	CUIQuickSlotBundle::QUICKSLOTBUNDLE_DESC tQuickDesc;
	tQuickDesc.fSlotSize = 50.f;
	tQuickDesc.fSlotMargin = 10.f;
	tQuickDesc.iSlotColCount = 4;
	tQuickDesc.iSlotRowCount = 2;
	tQuickDesc.vecHotKey = { KEY::Q, KEY::W, KEY::E, KEY::R, KEY::A, KEY::S, KEY::D, KEY::F };
	tQuickDesc.eAnchorType = CORNOR_TYPE::BOT;
	tQuickDesc.ePivotType = CORNOR_TYPE::BOT;
	tQuickDesc.fSizeX= 240;
	tQuickDesc.fSizeY = 120;
	tQuickDesc.fXOffset= -200;
	tQuickDesc.fYOffset = -30;

	m_pQuickSlotBundle = static_cast<CUIQuickSlotBundle*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIQuickSlotBundle::m_szProtoTag, &tQuickDesc));
	Add_Child(m_pQuickSlotBundle);
	Safe_AddRef(m_pQuickSlotBundle);

	CUIBar::UIBarDesc tBarDesc;
	tBarDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tBarDesc.ePivotType = CORNOR_TYPE::CENTER;
	tBarDesc.fSizeX = 300;
	tBarDesc.fSizeY = 30;
	tBarDesc.fXOffset = 0;
	tBarDesc.fYOffset = 0; 
	tBarDesc.vBorder = { 3,3,3,3};
	tBarDesc.vFillBorder = { 2,2,2,2 };

	tBarDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock( TEXT("castingbar_mainfrm.dds")));
	tBarDesc.pFillTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("castingbar_fill.dds")));
	m_pCastingBar = static_cast<CUIBar*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIBar::m_szProtoTag, &tBarDesc));
	Add_Child(m_pCastingBar);
	Safe_AddRef(m_pCastingBar);
	m_pCastingBar->Set_Active(false);
	return S_OK;
}

void CUIBundle::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pGameInstance->GetKeyState(KEY::I) == KEY_STATE::DOWN)
		m_pInventory->Toggle_Active();
	if (m_pGameInstance->GetKeyState(KEY::P) == KEY_STATE::DOWN)
	{
	}
}

void CUIBundle::Set_QuickItem(KEY eHotKey, IQuickItem* pItem)
{
	m_pQuickSlotBundle->Set_QuickItem(eHotKey, pItem);
}

void CUIBundle::Update_Slot(ITEM_TYPE eType, _uint iIndex)
{
	m_pInventory->Update_Slot(eType, iIndex);
}

void CUIBundle::Update_CastingRatio(_float fRatio)
{
	m_pCastingBar->Update_Ratio(fRatio);
}

void CUIBundle::Set_CastingBarVisible(_bool bVisible)
{
	m_pCastingBar->Set_Active(bVisible);
}

void CUIBundle::Free()
{
	__super::Free();
	Safe_Release(m_pQuickSlotBundle);
	Safe_Release(m_pInventory);
	Safe_Release(m_pCastingBar);
}
