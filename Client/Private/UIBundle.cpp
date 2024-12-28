

#include "stdafx.h"
#include "UIBundle.h"
#include "GameInstance.h"

#include "UIQuickSlotBundle.h"
#include "UIQuickSlot.h"
#include "UIInventory.h"
#include "Inventory.h"
#include "UIBar.h"
#include "UIMainHUDGuage.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "UIPlayerInfo.h"
#include "UINPCDialog.h"

IMPLEMENT_SINGLETON(CUIBundle)

CUIBundle::CUIBundle()
	: CBase()
{
	m_pGameInstance = CGameInstance::GetInstance();
}

HRESULT CUIBundle::Initialize(void* pArg)
{
	CPlayer* pPlayer = PLAYERINIFO->Get_Player();

	UIBUNDLE_DESC* pDesc = static_cast<UIBUNDLE_DESC*>(pArg);
	m_pDevice = pDesc->pDevice;
	m_pContext = pDesc->pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	CUIInventory::UIINVENTORY_DESC tDesc;
	tDesc.pInventory = INVENTORY;
	m_pInventory = static_cast<CUIInventory*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIInventory::m_szProtoTag, &tDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pInventory, true)))
		return E_FAIL;
	m_pInventory->Set_Active(false);
	Safe_AddRef(m_pInventory);

	CUIQuickSlotBundle::QUICKSLOTBUNDLE_DESC tQuickDesc;
	tQuickDesc.fSlotSize = 50.f;
	tQuickDesc.fSlotMargin = 10.f;
	tQuickDesc.iSlotColCount = 4;
	tQuickDesc.iSlotRowCount = 2;
	tQuickDesc.vecHotKey = { KEY::Q, KEY::W, KEY::E, KEY::R, KEY::A, KEY::S, KEY::D, KEY::F };
	tQuickDesc.eAnchorType = CORNOR_TYPE::BOT;
	tQuickDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tQuickDesc.fSizeX= 240;
	tQuickDesc.fSizeY = 120;
	tQuickDesc.fXOffset= -100;
	tQuickDesc.fYOffset = -30;

	m_pQuickSlotBundle1 = static_cast<CUIQuickSlotBundle*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIQuickSlotBundle::m_szProtoTag, &tQuickDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pQuickSlotBundle1, true)))
		return E_FAIL;
	Safe_AddRef(m_pQuickSlotBundle1);


	tQuickDesc.vecHotKey = { KEY::NUM1, KEY::NUM2, KEY::NUM3, KEY::NUM4, KEY::NUM5, KEY::NUM6, KEY::NUM7, KEY::NUM8 };
	tQuickDesc.eAnchorType = CORNOR_TYPE::BOT;
	tQuickDesc.ePivotType = CORNOR_TYPE::LEFT_BOT;
	tQuickDesc.fSizeX = 240;
	tQuickDesc.fSizeY = 120;
	tQuickDesc.fXOffset = 100;
	tQuickDesc.fYOffset = -30;

	m_pQuickSlotBundle2 = static_cast<CUIQuickSlotBundle*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIQuickSlotBundle::m_szProtoTag, &tQuickDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pQuickSlotBundle2, true)))
		return E_FAIL;
	Safe_AddRef(m_pQuickSlotBundle2);

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
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pCastingBar, true)))
		return E_FAIL;
	Safe_AddRef(m_pCastingBar);
	m_pCastingBar->Set_Active(false);

	CUINPCDialog::NPCDIALOG_DESC tNPCDesc;
	tNPCDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tNPCDesc.ePivotType = CORNOR_TYPE::CENTER;
	tNPCDesc.fSizeX = g_iWinSizeX;
	tNPCDesc.fSizeY = g_iWinSizeY;
	tNPCDesc.fXOffset = 0;
	tNPCDesc.fYOffset = 0;
	m_pNPCDialog = static_cast<CUINPCDialog*>(m_pGameInstance->Clone_Proto_Object_Stock(CUINPCDialog::m_szProtoTag, &tNPCDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pNPCDialog, true)))
		return E_FAIL;
	Safe_AddRef(m_pNPCDialog);
	m_pNPCDialog->Set_Active(false);

	return S_OK;
}


void CUIBundle::Set_QuickItem(KEY eHotKey, IQuickItem* pItem)
{
	m_pQuickSlotBundle1->Set_QuickItem(eHotKey, pItem);
}

void CUIBundle::Toggle_Inventory()
{
	m_pInventory->Toggle_Active();
}

void CUIBundle::Toggle_PlayerInfo()
{
	m_pPlayerInfoUI->Toggle_Active();
}
void CUIBundle::Toggle_HUD()
{
	m_pMainHPBar->Toggle_Active();
	m_pQuickSlotBundle1->Toggle_Active();
	m_pQuickSlotBundle2->Toggle_Active();
}
void CUIBundle::Set_HUDActive(_bool bActive)
{
	m_pMainHPBar->Set_Active(bActive);
	m_pQuickSlotBundle1->Set_Active(bActive);
	m_pQuickSlotBundle2->Set_Active(bActive);
}
void CUIBundle::Toggle_NPCDialog()
{
	m_pNPCDialog->Toggle_Active();
}
void CUIBundle::Set_NPCDialogActive(_bool bActive)
{
	m_pNPCDialog->Set_Active(bActive);
}

void CUIBundle::Set_NPCDialogNPC(CNPC* pNPC)
{
	m_pNPCDialog->Set_NPC(pNPC);
}

void CUIBundle::Set_NPCDialogData(const CONVERSATION_NODE_DATA& pNode)
{
	m_pNPCDialog->Set_ConversationNode(pNode);
}
void CUIBundle::Initialize_PlayerInfo(CPlayer* pPalyer)
{
	CUIMainHUDGuage::UIMAINHUDGUAGE_DESC tMainBarDesc;
	tMainBarDesc.eAnchorType = CORNOR_TYPE::BOT;
	tMainBarDesc.ePivotType = CORNOR_TYPE::BOT;
	tMainBarDesc.fSizeX = 168;
	tMainBarDesc.fSizeY = 186;
	tMainBarDesc.fXOffset = -10;
	tMainBarDesc.fYOffset = 0;
	tMainBarDesc.pStat = pPalyer->Get_Stat_Ref();
	tMainBarDesc.pDefaultStat = pPalyer->Get_DefaultStat_Ref();
	m_pMainHPBar = static_cast<CUIMainHUDGuage*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIMainHUDGuage::m_szProtoTag, &tMainBarDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pMainHPBar, true)))
		return ;
	Safe_AddRef(m_pMainHPBar);
	m_pMainHPBar->Set_Active(true);

	CUIPlayerInfo::UIPLAYERINFO_DESC tPlayerInfoDesc;
	tPlayerInfoDesc.pPlayerInfo = PLAYERINIFO;
	m_pPlayerInfoUI = static_cast<CUIPlayerInfo*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPlayerInfo::m_szProtoTag, &tPlayerInfoDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI, m_pPlayerInfoUI, true)))
		return;
	Safe_AddRef(m_pPlayerInfoUI);
	m_pPlayerInfoUI->Set_Active(false);
}

void CUIBundle::Update_Inven_Slot(_uint iIndex,CInvenSlot* pSlot)
{
	m_pInventory->Update_Slot(iIndex, pSlot);
}


void CUIBundle::Update_CastingRatio(_float fRatio)
{
	m_pCastingBar->Update_Ratio(fRatio);
}

void CUIBundle::Update_PlayerInfo_Slot(_uint iSlotId, CPlayerInfoSlot* pItemSlot)
{
	m_pPlayerInfoUI->Update_Slot((CUIPlayerInfo::SLOT_ID)iSlotId, pItemSlot);
}


void CUIBundle::Set_CastingBarVisible(_bool bVisible)
{
	m_pCastingBar->Set_Active(bVisible);
}

void CUIBundle::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pQuickSlotBundle1);
	Safe_Release(m_pQuickSlotBundle2);
	Safe_Release(m_pInventory);
	Safe_Release(m_pCastingBar);
	Safe_Release(m_pMainHPBar);
	Safe_Release(m_pPlayerInfoUI);
	Safe_Release(m_pNPCDialog);
}
