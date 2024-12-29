#include "stdafx.h"
#include "UIQuestGuide.h"
#include "QuestDataBase.h"
#include "GameInstance.h"
#include "UIFont.h"

CUIQuestGuide::CUIQuestGuide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}


CUIQuestGuide::CUIQuestGuide(const CUIQuestGuide& Prototype)
	: CUIObject(Prototype)
{
	m_pQuestDB = QUESTDB;
}


HRESULT CUIQuestGuide::Initialize(void* pArg)
{
	QUESGUIDE_DESC* pDesc = static_cast<QUESGUIDE_DESC*>(pArg);
	m_pQuestData = m_pQuestDB->Get_Data(pDesc->eQuestID);


	CUIFont::FONT_DESC tFontDesc;
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.fSizeX = pDesc->fSizeX;
	tFontDesc.fSizeY = 0;
	tFontDesc.fXOffset = m_fCommonPadding;
	tFontDesc.fYOffset = m_fCommonPadding + m_f2TitleSize.y;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_13");
	wstring wstrContent = L"";
	for (auto& pCondition : m_pQuestData->listCompleteCondition)
	{
		wstrContent += pCondition->To_String();
		wstrContent += L"\n";
	}
	tFontDesc.pText = wstrContent.c_str();
	tFontDesc.vColor = { 1,1,1,1 };
	tFontDesc.bShade = true;
	m_pContentFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));


	pDesc->fSizeY = Get_Height();
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pBackPanel->Add_Child(m_pContentFont);
	return S_OK;
}

HRESULT CUIQuestGuide::Render()
{
	return __super::Render();
}

_float CUIQuestGuide::Get_Height()
{
	return  m_f2TitleSize.y + m_fCommonPadding*2 + m_pContentFont->Get_TextSize().y;
}

void CUIQuestGuide::Update_Data()
{
	wstring wstrContent = L"";
	for (auto& pCondition : m_pQuestData->listCompleteCondition)
	{
		wstrContent += pCondition->To_String();
		wstrContent += L"\n";
	}
	m_pContentFont->Set_Text(wstrContent.c_str());
	CRect_Transform* pTransform = static_cast<CRect_Transform*>(m_pBackPanel->Get_Transform());
	pTransform->Set_Size(pTransform->Get_ScreenSize().x, Get_Height());

	m_pHighlightPanel->Set_Active(m_pQuestData->Is_SatisfiedCompleteCondition());
}



HRESULT CUIQuestGuide::Ready_Components()
{
	_float2 f2Size = static_cast<CRect_Transform*>( m_pTransformCom)->Get_ScreenSize();
	CUIPanel::PANEL_DESC tPanelDesc;
	tPanelDesc.eAnchorType = CORNOR_TYPE::BOT;
	tPanelDesc.ePivotType = CORNOR_TYPE::BOT;
	tPanelDesc.fSizeX = f2Size.x;
	tPanelDesc.fSizeY = f2Size.y;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	tPanelDesc.vBorder = { 4,4,4,4 };
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("questguide_back.dds")));
	m_pBackPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pBackPanel);

	tPanelDesc.eAnchorType = CORNOR_TYPE::TOP;
	tPanelDesc.ePivotType = CORNOR_TYPE::TOP;
	tPanelDesc.fSizeX = f2Size.x;
	tPanelDesc.fSizeY = m_f2TitleSize.y;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("questguide_title.dds")));
	tPanelDesc.vBorder = { 3,1,3,3 };
	m_pTitlePanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pTitlePanel);

	CUIFont::FONT_DESC tFontDesc;
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_BOT;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_BOT;
	tFontDesc.fSizeX = m_f2TitleSize.x;
	tFontDesc.fSizeY = m_f2TitleSize.y;
	tFontDesc.fXOffset = m_fCommonPadding;
	tFontDesc.fYOffset = -m_fCommonPadding;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_13");
	tFontDesc.pText = m_pQuestData->strName.c_str();
	tFontDesc.vColor = { 0.9,0.69,0.255,1 };
	tFontDesc.bShade = true;
	m_pTitleFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	m_pTitlePanel->Add_Child(m_pTitleFont);

	tPanelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tPanelDesc.ePivotType = CORNOR_TYPE::CENTER;
	tPanelDesc.fSizeX = f2Size.x;
	tPanelDesc.fSizeY = f2Size.y;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	tPanelDesc.vBorder = { 10,10,10,10 };
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("questguide_highlighter.dds")));
	m_pHighlightPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pHighlightPanel);
	m_pHighlightPanel->Set_Active(false);

	return S_OK;
}

CUIQuestGuide* CUIQuestGuide::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIQuestGuide* pInstance = new CUIQuestGuide(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIQuestGuide");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIQuestGuide::Clone(void* pArg)
{
	CUIQuestGuide* pInstance = new CUIQuestGuide(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIQuestGuide");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIQuestGuide::Free()
{
	__super::Free();

}
