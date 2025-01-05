#include "stdafx.h"
#include "UIChatOption.h"
#include "UIFont.h"
#include "UIIcon.h"
#include "GameInstance.h"
#include "QuestDataBase.h"

CUIChatOption::CUIChatOption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIButton(pDevice, pContext)
{
}

CUIChatOption::CUIChatOption(const CUIChatOption& Prototype)
	:CUIButton(Prototype)
{
}

HRESULT CUIChatOption::Initialize(void* pArg)
{
	CHATOPTION_DESC* pDesc = static_cast<CHATOPTION_DESC*>(pArg);
	m_iUIIndex = pDesc->iUIIndex;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUIIcon::PANEL_DESC tIconDesc{};
	tIconDesc.eAnchorType = CORNOR_TYPE::LEFT;
	tIconDesc.ePivotType = CORNOR_TYPE::LEFT;
	tIconDesc.fXOffset = 0;
	tIconDesc.fYOffset = 0;
	tIconDesc.fSizeX = 40;
	tIconDesc.fSizeY = 40;
	m_pIcon = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tIconDesc));
	Add_Child(m_pIcon);

	CUIFont::FONT_DESC tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFontDesc.fXOffset = 40;
	tFontDesc.fYOffset = 0;
	tFontDesc.pFontTag = TEXT("LV2Gothic_Bold_13");
	tFontDesc.vColor = { 1,1,1,1 };
	tFontDesc.bShade = true;
	tFontDesc.pText = TEXT("OPTION");
	m_pFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pFont);
	return S_OK;
}

void CUIChatOption::Set_OptionData(_uint iIdx,const  ChatOptionData& tOptionData)
{
	m_iDataOptionIdx = iIdx;
	m_tOptionData = tOptionData;

	CHAT_OPT_TYPE eType = tOptionData.eOptType;
	switch (eType)
	{
	case Client::CHAT_OPT_TYPE::NORMAL:
	case Client::CHAT_OPT_TYPE::ABANDON_QUEST:
		m_pIcon->Set_Texture(static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("npcdialog_opt_icon_normal.dds"), nullptr)));
		m_pFont->Set_Text(tOptionData.szText);
		break;
	case Client::CHAT_OPT_TYPE::QUEST:
	{
		//표시된 퀘스트 : 진행중 & 완료가능 & 수락 가능 중 하나

		assert(false == QUESTDB->Is_QuestCompleted(tOptionData.eQuestID));
		QuestData* pQuestData = QUESTDB->Get_Data(tOptionData.eQuestID);
		if (QUESTDB->Is_QuestSatisfiedCompleteCondition(tOptionData.eQuestID))
		{
			m_pIcon->Set_Texture(static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("npcdialog_opt_icon_quest_1.dds"), nullptr)));
			wstring wstrName = pQuestData->strName;
			wstrName = TEXT("[완료 가능]") + wstrName;
			m_pFont->Set_Text(wstrName.c_str());
		}
		else if (QUESTDB->Is_QuestAccepted(tOptionData.eQuestID))
		{
			m_pIcon->Set_Texture(static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("npcdialog_opt_icon_quest_2.dds"), nullptr)));
			wstring wstrName = pQuestData->strName;
			wstrName = TEXT("[진행 중]") + wstrName;
			m_pFont->Set_Text(wstrName.c_str());
		}
		else
		{
			m_pIcon->Set_Texture(static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("npcdialog_opt_icon_quest_0.dds"), nullptr)));
			wstring wstrName = pQuestData->strName;
			wstrName = TEXT("[시작 가능]") + wstrName;
			m_pFont->Set_Text(wstrName.c_str());
		}
		break;
	}
	case Client::CHAT_OPT_TYPE::LAST:
	default:
		break;
	}
	//폰트 세팅

}



CUIChatOption* CUIChatOption::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIChatOption* pInstance = new CUIChatOption(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIChatOption");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIChatOption::Clone(void* pArg)
{
	CUIChatOption* pInstance = new CUIChatOption(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIChatOption");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIChatOption::Free()
{
	__super::Free();
}

