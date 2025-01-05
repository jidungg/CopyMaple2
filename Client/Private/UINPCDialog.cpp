#include "stdafx.h"
#include "UINPCDialog.h"
#include "GameInstance.h"
#include "UIPanel.h"
#include "UIFont.h"
#include "UIChatOption.h"
#include "UIBundle.h"
#include "QuestDataBase.h"
#include "NPC.h"
#include "UIIcon.h"

CUINPCDialog::CUINPCDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUINPCDialog::CUINPCDialog(const CUINPCDialog& Prototype)
	: CUIContainer(Prototype)
{
}

HRESULT CUINPCDialog::Initialize(void* pArg)
{
	NPCDIALOG_DESC* pDesc = static_cast<NPCDIALOG_DESC*>(pArg);
	pDesc->eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->ePivotType = CORNOR_TYPE::CENTER;
	pDesc->fSizeX = g_iWinSizeX;
	pDesc->fSizeX = g_iWinSizeY;
	pDesc->fXOffset= 0;
	pDesc->fYOffset= 0;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUIPanel::PANEL_DESC tPanelDesc{};
	tPanelDesc.eAnchorType = CORNOR_TYPE::BOT;
	tPanelDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tPanelDesc.fSizeX =450;
	tPanelDesc.fSizeY = 450;
	tPanelDesc.fXOffset = -100;
	tPanelDesc.fYOffset =-m_fBoxHeight;
	tPanelDesc.vBorder = { 0,0,0,0 };
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("11000508.dds"), nullptr));
	m_pLeftPortrait = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pLeftPortrait);
	tPanelDesc.ePivotType = CORNOR_TYPE::LEFT_BOT;
	tPanelDesc.fXOffset = 100;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("30_f.dds"), nullptr));
	m_pRightPortrait = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pRightPortrait);

	tPanelDesc.eAnchorType = CORNOR_TYPE::BOT;
	tPanelDesc.ePivotType = CORNOR_TYPE::BOT;
	tPanelDesc.fSizeX = g_iWinSizeX;
	tPanelDesc.fSizeY = m_fBoxHeight;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	tPanelDesc.vBorder = { 5,5,5,5 };
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("npcdialog_chatbox.dds"), nullptr));
	m_pChatBox = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pChatBox);

	CUIFont::FONT_DESC tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.fXOffset = m_fHorizontalMargin;
	tFontDesc.fYOffset = 10;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_15");
	tFontDesc.vColor = { 0.74,0.97,0.42,1 };
	tFontDesc.bShade = true;
	tFontDesc.pText = TEXT("NPC Name");
	m_pNPCName = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	m_pChatBox->Add_Child(m_pNPCName);

	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.fXOffset = m_fHorizontalMargin;
	tFontDesc.fYOffset = 40;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_13");
	tFontDesc.vColor = { 1,1,1,1 };
	tFontDesc.bShade = true;
	tFontDesc.pText = TEXT("Script");
	m_pScript = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	m_pChatBox->Add_Child(m_pScript);

	m_vecUIOption.resize(3);
	CUIChatOption::CHATOPTION_DESC tOptionDesc{};
	tOptionDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tOptionDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tOptionDesc.fSizeX = g_iWinSizeX  - m_fHorizontalMargin*2 - m_fOptionLeftOffset;
	tOptionDesc.fSizeY = m_fOptionHeight;
	tOptionDesc.fXOffset = m_fHorizontalMargin+m_fOptionLeftOffset;
	tOptionDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_NPCDialogOptionButton"), nullptr));
	for (_uint i = 0; i < 3; i++)
	{
		tOptionDesc.iUIIndex = i;
		tOptionDesc.fYOffset = m_fOptionHeight * i;
		m_vecUIOption[i] = static_cast<CUIChatOption*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIChatOption::m_szProtoTag, &tOptionDesc));
		m_vecUIOption[i]->Register_OnClickCallback(bind(&CUINPCDialog::On_OptionSelected, this, placeholders::_1));
		m_pChatBox->Add_Child(m_vecUIOption[i]);
	}


	CUIButton::BUTTON_DESC tButtonDesc{};
	tButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	tButtonDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tButtonDesc.fXOffset = -m_fHorizontalMargin;
	tButtonDesc.fYOffset = -10;
	tButtonDesc.fSizeX = m_fButtonWidth;
	tButtonDesc.fSizeY = m_fButtonHeight;
	tButtonDesc.vBorder = { 5,5,5,5 };
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_NPCDialogButton"), nullptr));
	m_pNextButton = static_cast<CUIButton*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIButton::m_szProtoTag, &tButtonDesc));
	m_pChatBox->Add_Child(m_pNextButton);

	tButtonDesc.fXOffset = -m_fHorizontalMargin - m_fButtonWidth -10;
	m_pESCButton = static_cast<CUIButton*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIButton::m_szProtoTag, &tButtonDesc));
	m_pChatBox->Add_Child(m_pESCButton);

	tFontDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFontDesc.ePivotType = CORNOR_TYPE::CENTER;
	tFontDesc.fXOffset = 0;
	tFontDesc.fYOffset = 0;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_10");
	tFontDesc.vColor = { 0,0,0,1 };
	tFontDesc.bShade = false;
	tFontDesc.pText = TEXT("닫기\nESC");
	m_pESCButtonFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	m_pESCButton->Add_Child(m_pESCButtonFont);
	tFontDesc.pText = TEXT("다음\nENTER");
	m_pNextButtonFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	m_pNextButton->Add_Child(m_pNextButtonFont);

	m_pESCButton->Register_OnClickCallback(bind(&CUINPCDialog::Escape, this));
	m_pNextButton->Register_OnClickCallback(bind(&CUINPCDialog::To_NextConversation, this));

	CUIIcon::PANEL_DESC tHighlighterDesc;
	tHighlighterDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tHighlighterDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tHighlighterDesc.fSizeX = tOptionDesc.fSizeX;
	tHighlighterDesc.fSizeY = tOptionDesc.fSizeY;
	tHighlighterDesc.fXOffset = 0;
	tHighlighterDesc.fYOffset = 0;
	tHighlighterDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("npcdialog_highlighter.dds")));
	tHighlighterDesc.bDraggableX = false;
	tHighlighterDesc.bDraggableY = false;
	tHighlighterDesc.vBorder = m_f4HighlighterBorderSize;
	m_pHighlighter = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tHighlighterDesc));
	if (m_pHighlighter)
		m_pChatBox->Add_Child(m_pHighlighter);

	return S_OK;
}

void CUINPCDialog::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::ESC) == KEY_STATE::DOWN)
	{
		Escape();
	}
	if (m_pGameInstance->GetKeyState(KEY::ENTER) == KEY_STATE::DOWN)
	{
		To_NextConversation();
	}
	__super::Update(fTimeDelta);
}

void CUINPCDialog::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUINPCDialog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}



void CUINPCDialog::Set_ConversationNode(const CONVERSATION_NODE_DATA& tNode)
{
	//PORTRAIT
	CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock( tNode.vecPortrait[0].szPortraitTag));
	m_pLeftPortrait->Set_Texture(pTexture);
	m_pLeftPortrait->Set_Gray(tNode.vecPortrait[0].bGray);
	if (tNode.vecPortrait.size() > 1)
	{
		pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(tNode.vecPortrait[1].szPortraitTag));
		m_pRightPortrait->Set_Texture(pTexture);
		m_pRightPortrait->Set_Gray(tNode.vecPortrait[1].bGray);
		m_pRightPortrait->Set_Active(true);
	}
	else
	{
		m_pRightPortrait->Set_Active(false);
	}
	//NAME
	m_pNPCName->Set_Text(tNode.szSpeakerName);
	//SCRIPT
	m_pScript->Set_Text(tNode.szScript);
	//OPTIONS


	for (auto& pUIOption : m_vecUIOption)
		pUIOption->Set_Active(false);
	m_pHighlighter->Set_Active(false);

	_uint iDataOptionCount = tNode.vecOption.size();
	_uint iUIOptIndex = 0;
	m_iSelectedOptionDataIndex = UINT_MAX;
	for (_uint iDataOptIndex = 0; iDataOptIndex < iDataOptionCount; iDataOptIndex++)
	{
		if (tNode.vecOption[iDataOptIndex].eOptType == CHAT_OPT_TYPE::QUEST)
		{
			QUEST_ID eQuestID = tNode.vecOption[iDataOptIndex].eQuestID;
			//표시되어야 하는 퀘스트 : 진행중 & 완료가능 & 수락 가능
			if (false == QUESTDB->Is_SatisfiedAcceptCondition(eQuestID))
				continue;
			if (QUESTDB->Is_QuestCompleted(eQuestID))
				continue;
		}
		_float2 fOptionOffset = Get_OptionOffset(iUIOptIndex);
		static_cast<CRect_Transform*>(m_vecUIOption[iUIOptIndex]->Get_Transform())->Set_Offset(fOptionOffset.x, fOptionOffset.y);
		m_vecUIOption[iUIOptIndex]->Set_OptionData(iDataOptIndex, tNode.vecOption[iDataOptIndex]);
		if(m_iSelectedOptionDataIndex == UINT_MAX)m_iSelectedOptionDataIndex = iDataOptIndex;
		m_vecUIOption[iUIOptIndex]->Set_Active(true);
		iUIOptIndex++;
	}
	if (iUIOptIndex > 0)
	{
		_float2 fOptionOffset0 = Get_OptionOffset(0);
		static_cast<CRect_Transform*>(m_pHighlighter->Get_Transform())->Set_Offset(fOptionOffset0.x, fOptionOffset0.y);
		m_pHighlighter->Set_Active(true);
	}

}

void CUINPCDialog::To_NextConversation()
{
	m_pNPC->To_NextConversation(m_iSelectedOptionDataIndex);
}

void CUINPCDialog::On_OptionSelected(void* pArg)
{
	CUIChatOption* pOptionButton = reinterpret_cast<CUIChatOption*>(pArg);
	m_iSelectedOptionDataIndex = pOptionButton->Get_DataOptionIdx();
	_float2 fOptionOffset = Get_OptionOffset(pOptionButton->Get_UIOptionIdx());
	static_cast<CRect_Transform*>(m_pHighlighter->Get_Transform())->Set_Offset(fOptionOffset.x, fOptionOffset.y);
}

void CUINPCDialog::Escape()
{
	m_pNPC->End_Conversation();

}

_float2 CUINPCDialog::Get_OptionOffset(_uint iOptionUIIndex)
{
	_float2 fOffset = { 0,0 };
	fOffset.x = m_fHorizontalMargin + m_fOptionLeftOffset;
	fOffset.y = 10 + 40 + m_fOptionHeight*iOptionUIIndex + m_pScript->Get_TextSize().y;
	return fOffset;
}



CUINPCDialog* CUINPCDialog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUINPCDialog* pInstance = new CUINPCDialog(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUINPCDialog");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;

}

CGameObject* CUINPCDialog::Clone(void* pArg)
{
	CUINPCDialog* pInstance = new CUINPCDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUINPCDialog");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUINPCDialog::Free()
{
	__super::Free();
}
