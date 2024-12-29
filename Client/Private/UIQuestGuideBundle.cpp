#include "stdafx.h"
#include "UIQuestGuideBundle.h"
#include "UIQuestGuide.h"
#include "GameInstance.h"

CUIQuestGuideBundle::CUIQuestGuideBundle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}


CUIQuestGuideBundle::CUIQuestGuideBundle(const CUIQuestGuideBundle& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIQuestGuideBundle::Initialize(void* pArg)
{
	QUESTGUIDEBUNDLE_DESC* pDesc = static_cast<QUESTGUIDEBUNDLE_DESC*>(pArg);
	pDesc->eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	pDesc->ePivotType = CORNOR_TYPE::RIGHT_TOP;
	pDesc->fSizeX = m_fGuideWidth;
	pDesc->fSizeY = g_iWinSizeY;
	pDesc->fXOffset = 0;
	pDesc->fYOffset = 0;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

HRESULT CUIQuestGuideBundle::Render()
{
	return __super::Render();
}

void CUIQuestGuideBundle::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIQuestGuideBundle::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIQuestGuideBundle::Add_QuestGuide(QUEST_ID eQuestID)
{
	if (m_mapQuestGuide.find(eQuestID) != m_mapQuestGuide.end())
		return;
	CUIQuestGuide::QuestGuideDesc tDesc;
	tDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	tDesc.fSizeX = m_fGuideWidth;
	tDesc.fXOffset = 0;
	tDesc.eQuestID= eQuestID;
	CUIQuestGuide* pGuide = static_cast<CUIQuestGuide*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIQuestGuide::m_szProtoTag, &tDesc));
	m_mapQuestGuide.insert({ eQuestID, pGuide });
	Add_Child(pGuide);
	Arrange_Guide();
}

void CUIQuestGuideBundle::Remove_QuestGuide(QUEST_ID eQuestID)
{
	auto iter = m_mapQuestGuide.find(eQuestID);
	if (iter == m_mapQuestGuide.end())
		return;
	Remove_Child(iter->second);
	Safe_Release(iter->second);
	m_mapQuestGuide.erase(iter);
	Arrange_Guide();
}

void CUIQuestGuideBundle::Update_Data()
{
	for (auto& pGuide : m_mapQuestGuide)
	{
		pGuide.second->Update_Data();
	}
	Arrange_Guide();
}

void CUIQuestGuideBundle::Update_Data(QUEST_ID eQuestID)
{
	auto iter = m_mapQuestGuide.find(eQuestID);
	if (iter == m_mapQuestGuide.end())
		return;
	iter->second->Update_Data();
	Arrange_Guide();
}

void CUIQuestGuideBundle::Arrange_Guide()
{
	_float fBottomOffset = 0;
	for (auto& pGuide : m_mapQuestGuide)
	{
		CRect_Transform* pTransform =static_cast<CRect_Transform*>(pGuide.second->Get_Transform()); 
		pTransform->Set_Offset(0, fBottomOffset);
		fBottomOffset += pGuide.second->Get_Height() + m_fGuideMargin;
	}
}

CUIQuestGuideBundle* CUIQuestGuideBundle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIQuestGuideBundle* pInstance = new CUIQuestGuideBundle(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIQuestGuideBundle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIQuestGuideBundle::Clone(void* pArg)
{
	CUIQuestGuideBundle* pInstance = new CUIQuestGuideBundle(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIQuestGuideBundle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIQuestGuideBundle::Free()
{
	__super::Free();
}

