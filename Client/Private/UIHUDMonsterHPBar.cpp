#include "stdafx.h"
#include "UIHUDMonsterHPBar.h"
#include "Monster.h"
#include "GameInstance.h"
#include "UIFont.h"
#include "Engine_Utility.h"

CUIHUDMonsterHPBar::CUIHUDMonsterHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIHPBar(pDevice, pContext)
{
}

CUIHUDMonsterHPBar::CUIHUDMonsterHPBar(const CUIHUDMonsterHPBar& Prototype)
	: CUIHPBar(Prototype)
{
}

HRESULT CUIHUDMonsterHPBar::Initialize(void* pArg)
{
	HUDMONSTERHPBAR_DESC* pDesc = static_cast<HUDMONSTERHPBAR_DESC*>(pArg);
	m_bBoss = pDesc->bBoss;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CUIFont::FONT_DESC tFontDesc;
	tFontDesc.bShade = true;
	if (false == m_bBoss)
	{
		tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		tFontDesc.ePivotType = CORNOR_TYPE::CENTER;
		tFontDesc.fXOffset = m_f2LevelFontPos.x;
		tFontDesc.fYOffset = m_f2LevelFontPos.y;
		tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_13");

		m_pLevelFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
		Add_Child(m_pLevelFont);
	}

	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = - pDesc->vFramePadding.w - 6;
	tFontDesc.fYOffset = pDesc->vFramePadding.x + m_fFillHeight*0.5;
	if (false == m_bBoss)
		tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_15");
	else
		tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_30");
	m_pRemainHPFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pRemainHPFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = pDesc->vFramePadding.z + 6;
	m_pNameFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pNameFont);
	return S_OK;
}

void CUIHUDMonsterHPBar::Set_Character(CCharacter* pCharacter)
{
	__super::Set_Character(pCharacter);
	CMonster* pMonster = static_cast<CMonster*>(pCharacter);
	Stat tStat = pMonster->Get_Stat();
	
	wstring wstr; 
	if (false == m_bBoss)
	{
		wstr = to_wstring(tStat.iLEVEL);
		m_pLevelFont->Set_Text(wstr.c_str());
	}
	wstr = to_wstring(tStat.iHP);
	m_pRemainHPFont->Set_Text(wstr.c_str());
	m_pNameFont->Set_Text(CEngineUtility::ConvertStringToWString(pMonster->Get_Name()).c_str());
}

CUIHUDMonsterHPBar* CUIHUDMonsterHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIHUDMonsterHPBar* pInstance = new CUIHUDMonsterHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIHUDMonsterHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIHUDMonsterHPBar::Clone(void* pArg)
{
	CUIHUDMonsterHPBar* pInstance = new CUIHUDMonsterHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIHUDMonsterHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIHUDMonsterHPBar::Free()
{
	__super::Free();
}
