#include "stdafx.h"
#include "WorldUIHPBar.h"
#include "GameInstance.h"
#include "Character.h"

CWorldUIHPBar::CWorldUIHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWorldUIObject(pDevice, pContext)
{
}

CWorldUIHPBar::CWorldUIHPBar(const CWorldUIHPBar& Prototype)
	: CWorldUIObject(Prototype)
{
	
}

HRESULT CWorldUIHPBar::Initialize(void* pArg)
{
	WORLDHPBAR_DESC* pDesc = static_cast<WORLDHPBAR_DESC*>(pArg);
	m_pCharacter = pDesc->pCharacter;
	pDesc->eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->ePivotType = CORNOR_TYPE::CENTER;
	pDesc->fSizeX = 62;
	pDesc->fSizeY = 8;
	pDesc->fXOffset = 0;
	pDesc->fYOffset = 0;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUIHPBar::HPBAR_DESC tDesc;
	tDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tDesc.ePivotType = CORNOR_TYPE::CENTER;
	tDesc.fSizeX = 62;
	tDesc.fSizeY = 8;
	tDesc.fXOffset = 0;
	tDesc.fYOffset = 0;
	tDesc.pCharacter = m_pCharacter;
	tDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("worldhpbar_frame.dds")));
	tDesc.pFillTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("worldhpbar_monster_fill.dds")));
	tDesc.vBorder= { 1,2,2,2 };
	tDesc.vFramePadding = { 1,2,2,2 };
	tDesc.vFillBorder = { 1,0,0,0 };
	m_pHPBar = static_cast<CUIHPBar*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIHPBar::m_szProtoTag, &tDesc));
	Add_Child(m_pHPBar);
	return S_OK;
}

void CWorldUIHPBar::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

CWorldUIHPBar* CWorldUIHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldUIHPBar* pInstance = new CWorldUIHPBar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWorldUIHPBar::Clone(void* pArg)
{
	CWorldUIHPBar* pInstance = new CWorldUIHPBar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CWorldUIHPBar::Free()
{
	__super::Free();
}

