#include "stdafx.h"
#include "UIPlayerInfoDashBoard.h"
#include "GameInstance.h"
#include "UIFont.h"
#include "PlayerInfo.h"
#include "UIStatUpButton.h"

CUIPlayerInfoDashBoard::CUIPlayerInfoDashBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIPanel(pDevice, pContext)
{
}

CUIPlayerInfoDashBoard::CUIPlayerInfoDashBoard(const CUIPlayerInfoDashBoard& Prototype)
	:CUIPanel(Prototype)
{
}

HRESULT CUIPlayerInfoDashBoard::Initialize(void* pArg)
{
	PLAYERINFODASHBOARD_DESC* pDesc = static_cast<PLAYERINFODASHBOARD_DESC*>(pArg);
	m_pPlayerInfo = pDesc->pPlayerInfo;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUIFont::FONT_DESC tFontDesc;
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding + m_fBigCellSize.y * 0.5f;
	tFontDesc.fSizeX = 0;
	tFontDesc.fSizeY = 0;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_15";
	tFontDesc.pText = L"전투력";
	m_pBattleTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pBattleTextFont);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -m_fPadding - m_fMargin;
	m_pBattlePointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pBattlePointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding*2 + m_fBigCellSize.y * 1.5f ;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_15";
	tFontDesc.pText = L"공격력";
	m_pAttackTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pAttackTextFont);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -m_fPadding - m_fMargin;
	m_pAttackPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pAttackPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 3 + m_fBigCellSize.y * 2.5f;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_15";
	tFontDesc.pText = L"방어력";
	m_pDefenseTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pDefenseTextFont);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -m_fPadding - m_fMargin;
	m_pDefensePointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pDefensePointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4 + m_fBigCellSize.y * 3.f + m_fSmallCellSize.y*0.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"STR";
	m_pSTRTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pSTRTextFont);

	CUIStatUpButton::UIStatUpButtonDesc tButtonDesc;
	tButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tButtonDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tButtonDesc.fXOffset = -m_fPadding - m_fMargin;
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.fSizeX = m_fUpButtonSize.x;
	tButtonDesc.fSizeY = m_fUpButtonSize.y;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 0;
	m_pSTRUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pSTRUpButton);

	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset - m_fUpButtonSize.x - m_fMargin;
	m_pSTRPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pSTRPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4 + m_fBigCellSize.y * 3.f + m_fSmallCellSize.y * 1.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"DEX";
	m_pDEXTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pDEXTextFont);
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 1;
	m_pDEXUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pDEXUpButton);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset - m_fUpButtonSize.x - m_fMargin;
	m_pDEXPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pDEXPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4 + m_fBigCellSize.y * 3.f + m_fSmallCellSize.y * 2.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"INT";
	m_pINTTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pINTTextFont);
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 2;
	m_pINTUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pINTUpButton);
	
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset - m_fUpButtonSize.x - m_fMargin;
	m_pINTPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pINTPointFont);


	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4+ m_fBigCellSize.y * 3.f + m_fSmallCellSize.y * 3.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"LUK";
	m_pLUKTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pLUKTextFont);
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 3;
	m_pLUKUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pLUKUpButton);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset - m_fUpButtonSize.x - m_fMargin;
	m_pLUKPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pLUKPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4 + m_fBigCellSize.y * 3.f + m_fSmallCellSize.y * 4.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"HP";
	m_pHPTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pHPTextFont);
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 4;
	m_pHPUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pHPUpButton);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset  - m_fUpButtonSize.x- m_fMargin;
	m_pHPPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pHPPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = m_fPadding * 4 + m_fBigCellSize.y * 3.f + m_fSmallCellSize.y * 5.5;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"크리티컬 명중";
	m_pCRITTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pCRITTextFont);
	tButtonDesc.fYOffset = tFontDesc.fYOffset;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_PlayerStatIncButton"), nullptr));
	tButtonDesc.iStatType = 5;
	m_pCRITUpButton = static_cast<CUIStatUpButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIStatUpButton::m_szProtoTag, &tButtonDesc));
	Add_Child(m_pCRITUpButton);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = tButtonDesc.fXOffset - m_fUpButtonSize.x - m_fMargin;
	m_pCRITPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pCRITPointFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_BOT;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = m_fPadding + m_fMargin;
	tFontDesc.fYOffset = -m_fPadding - m_fMediumCellSize.y * 0.5f;
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.pText = L"남은 포인트";
	m_pLeftStatTextFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pLeftStatTextFont);
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -m_fPadding - m_fMargin;
	m_pLeftStatPointFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pLeftStatPointFont);

	return S_OK;
}

HRESULT CUIPlayerInfoDashBoard::Render()
{
	wstring str = to_wstring(m_pPlayerInfo->Get_BattlePoint());
	m_pBattlePointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_AttackPoint());
	m_pAttackPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalDefense());
	m_pDefensePointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalSTR());
	m_pSTRPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalDEX());
	m_pDEXPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalINT());
	m_pINTPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalLUK());
	m_pLUKPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_TotalHP());
	m_pHPPointFont->Set_Text(str.data());
	str = to_wstring((_int)(m_pPlayerInfo->Get_TotalCRIT() * 100));
	m_pCRITPointFont->Set_Text(str.data());
	str = to_wstring(m_pPlayerInfo->Get_LeftStatPoint());
	m_pLeftStatPointFont->Set_Text(str.data());
	if(FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

CUIPlayerInfoDashBoard* CUIPlayerInfoDashBoard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPlayerInfoDashBoard* pInstance = new CUIPlayerInfoDashBoard(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfoDashBoard");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIPlayerInfoDashBoard::Clone(void* pArg)
{
	CUIPlayerInfoDashBoard* pInstance = new CUIPlayerInfoDashBoard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfoDashBoard");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIPlayerInfoDashBoard::Free()
{
	__super::Free();
}
