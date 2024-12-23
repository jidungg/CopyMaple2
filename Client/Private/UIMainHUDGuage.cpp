#include "stdafx.h"
#include "UIMainHUDGuage.h"
#include "GameInstance.h"
#include "UIPanel.h"
#include "UIVerticalFill.h"
#include "UIFont.h"

CUIMainHUDGuage::CUIMainHUDGuage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUIMainHUDGuage::CUIMainHUDGuage(const CUIMainHUDGuage& Prototype)
	: CUIContainer(Prototype)
{
}


HRESULT CUIMainHUDGuage::Initialize(void* pArg)
{
	UIMAINHUDGUAGE_DESC* pDesc = static_cast<UIMAINHUDGUAGE_DESC*>(pArg);
	m_pStat = pDesc->pStat;
	m_pDefaultStat = pDesc->pDefaultStat;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CUIPanel::PANEL_DESC tPanelDesc;
	tPanelDesc.eAnchorType = CORNOR_TYPE::BOT;
	tPanelDesc.ePivotType = CORNOR_TYPE::BOT;
	tPanelDesc.fSizeX = pDesc->fSizeX;
	tPanelDesc.fSizeY = pDesc->fSizeY;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = -25;
	tPanelDesc.vBorder = { 0,0,0,0 };
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_back.dds")));
	m_pBack = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	if (nullptr == m_pBack)
		return E_FAIL;
	Add_Child(m_pBack);

	CUIVerticalFill<_int>::UIVERTICALFILL_DESC tFillDesc;
	tFillDesc.eAnchorType = CORNOR_TYPE::BOT;
	tFillDesc.ePivotType = CORNOR_TYPE::BOT;
	tFillDesc.fSizeX = pDesc->fSizeX;
	tFillDesc.fSizeY = pDesc->fSizeY;
	tFillDesc.fXOffset = 0;
	tFillDesc.fYOffset = -25;
	tFillDesc.vBorder = { 0,0,0,0 };
	tFillDesc.pValue = &(m_pStat->iHP);
	tFillDesc.pDefaultValue = &(m_pDefaultStat->iHP);
	tFillDesc.fVerticalEnd = { 0.85f };
	tFillDesc.fVerticalStart = { 0.054f };
	tFillDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_red.dds")));
	m_pRedFill = static_cast<CUIVerticalFill<_int>*>(m_pGameInstance->Clone_Proto_Object_Stock(TEXT("Prototype_UI_IntVerticalFill"), &tFillDesc));
	if (nullptr == m_pRedFill)
		return E_FAIL;
	Add_Child(m_pRedFill);

	tFillDesc.pValue = &(m_pStat->iSP);
	tFillDesc.pDefaultValue = &(m_pDefaultStat->iSP);
	tFillDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_blue.dds")));
	m_pBlueFill = static_cast<CUIVerticalFill<_int>*>(m_pGameInstance->Clone_Proto_Object_Stock(TEXT("Prototype_UI_IntVerticalFill"), &tFillDesc));
	if (nullptr == m_pBlueFill)
		return E_FAIL;
	Add_Child(m_pBlueFill);

	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_white0.dds")));
	m_pWhiteFill[0] = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	if (nullptr == m_pWhiteFill[0])
		return E_FAIL;
	Add_Child(m_pWhiteFill[0]);

	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_white1.dds")));
	m_pWhiteFill[1] = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	if (nullptr == m_pWhiteFill[1])
		return E_FAIL;
	Add_Child(m_pWhiteFill[1]);

	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_white2.dds")));
	m_pWhiteFill[2] = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	if (nullptr == m_pWhiteFill[2])
		return E_FAIL;
	Add_Child(m_pWhiteFill[2]);

	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("mainhpbar_front.dds")));
	m_pFront = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	if (nullptr == m_pFront)
		return E_FAIL;
	Add_Child(m_pFront);

	CUIFont::UIFontDesc tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -5;
	tFontDesc.fYOffset = 30;
	tFontDesc.pText = L"HP";
	tFontDesc.pFontTag = L"LV2Gothic_Bold_15";
	tFontDesc.vColor = _vector{ 0.6f, 0.2f, 0.2f, 1.f };
	tFontDesc.bShade = true;
	m_pHPFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pHPFont)
		return E_FAIL;
	Add_Child(m_pHPFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = 5;
	tFontDesc.fYOffset = 30;
	tFontDesc.pText = L"SP";
	tFontDesc.pFontTag = L"LV2Gothic_Bold_15";
	tFontDesc.vColor = _vector{ 0.2f, 0.2f, 0.6f, 1.f };
	tFontDesc.bShade = true;
	m_pSPFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pSPFont)
		return E_FAIL;
	Add_Child(m_pSPFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -5;
	tFontDesc.fYOffset = -2.5;
	wstring str = to_wstring(m_pStat->iHP);
	tFontDesc.pText = str.c_str();
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.vColor = _vector{ 1.f, 1.f, 1.f, 1.f };
	tFontDesc.bShade = true;
	m_pHPCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pHPCountFont)
		return E_FAIL;
	Add_Child(m_pHPCountFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = 5;
	tFontDesc.fYOffset = -2.5;
	str = to_wstring(m_pStat->iSP);
	tFontDesc.pText = str.c_str();
	tFontDesc.pFontTag = L"LV2Gothic_Bold_13";
	tFontDesc.vColor = _vector{ 1.f, 1.f, 1.f, 1.f };
	tFontDesc.bShade = true;
	m_pSPCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pSPCountFont)
		return E_FAIL;
	Add_Child(m_pSPCountFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT;
	tFontDesc.fXOffset = -5;
	tFontDesc.fYOffset = 10;
	str = to_wstring(m_pDefaultStat->iHP);
	tFontDesc.pText = str.c_str();
	tFontDesc.pFontTag = L"LV2Gothic_Bold_10";
	tFontDesc.vColor = _vector{ 0.7f, 0.7, 0.7, 1.f };
	tFontDesc.bShade = true;
	m_pDefaultHPCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pDefaultHPCountFont)
		return E_FAIL;
	Add_Child(m_pDefaultHPCountFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = 5;
	tFontDesc.fYOffset = 10;
	str = to_wstring(m_pDefaultStat->iSP);
	tFontDesc.pText = str.c_str();
	tFontDesc.pFontTag = L"LV2Gothic_Bold_10";
	tFontDesc.vColor = _vector{ 0.7f, 0.7, 0.7, 1.f };
	tFontDesc.bShade = true;
	m_pDefaultSPCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	if (nullptr == m_pDefaultSPCountFont)
		return E_FAIL;
	Add_Child(m_pDefaultSPCountFont);
	return S_OK;
}

void CUIMainHUDGuage::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIMainHUDGuage::Render()
{
	m_pHPCountFont->Set_Text(to_wstring(m_pStat->iHP).c_str());
	m_pSPCountFont->Set_Text(to_wstring(m_pStat->iSP).c_str());
	m_pDefaultHPCountFont->Set_Text(to_wstring(m_pDefaultStat->iHP).c_str());
	m_pDefaultSPCountFont->Set_Text(to_wstring(m_pDefaultStat->iSP).c_str());
	for (_uint i = 0; i < 3; i++)
	{
		if(i < m_pStat->iEP )
			m_pWhiteFill[i]->Set_Active(true);
		else
			m_pWhiteFill[i]->Set_Active(false);
	} 
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


CUIMainHUDGuage* CUIMainHUDGuage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIMainHUDGuage* pInstance = new CUIMainHUDGuage(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CHUDBundle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIMainHUDGuage::Clone(void* pArg)
{
	CUIMainHUDGuage* pInstance = new CUIMainHUDGuage(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIMainHPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIMainHUDGuage::Free()
{
	__super::Free();
}
