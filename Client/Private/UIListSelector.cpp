#include "stdafx.h"
#include "UIListSelector.h"
#include "GameInstance.h"
#include "UIButton.h"
#include "UIItemIndicator.h"
CUIListSelector::CUIListSelector(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIList(pDevice, pContext)
{
}

CUIListSelector::CUIListSelector(const CUIListSelector& Prototype)
	: CUIList(Prototype)
{
}

HRESULT CUIListSelector::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIListSelector::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UILISTSELECTOR_DESC* pDesc = static_cast<UILISTSELECTOR_DESC*>(pArg);
	m_iSelectedIndex = pDesc->iInitialSelectedIndex;
	CUIPanel::PANEL_DESC panelDesc;
	panelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	panelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	panelDesc.fSizeX = pDesc->fItemWidth;
	panelDesc.fSizeY = pDesc->fItemHeight;
	panelDesc.fXOffset = 0;
	panelDesc.fYOffset = 0;
	panelDesc.pTextureCom = static_cast<CTexture*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, pDesc->eHighlighterTexProtoLev, pDesc->szHighlighterTexProtoTag));

	m_pHighlighter = static_cast<CUIPanel*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING,CUIPanel::m_szProtoTag, &panelDesc));
	if(m_pHighlighter)	
		Add_Child(m_pHighlighter);

	function<void(void*)> f = bind(&CUIListSelector::On_ClickItem, this, placeholders::_1);
	Register_OnClickCallback(f);
	return S_OK;
}

HRESULT CUIListSelector::Render()
{
	__super::Render();
	if (Is_VisibleRow(Get_ItemRow(m_iSelectedIndex)) && m_pHighlighter->Is_Valid())
		m_pHighlighter->Render();
	return S_OK;
}

void CUIListSelector::Register_OnClickCallback(const ButtonCallback& fCallback)
{
	for (auto& child : m_pChilds)
	{
		CUIButton* pButton = dynamic_cast<CUIButton*>(child);
		if(pButton)
			pButton->Register_OnClickCallback(fCallback);
	}
}

void CUIListSelector::On_ClickItem(void* pArg)
{
	Select_Item(reinterpret_cast<CUIButtonItemIndicator*>(pArg)->Get_ListItemIndex());
}

void CUIListSelector::Select_Item(_uint iIndex)
{
	m_iSelectedIndex = iIndex;
	_float2 fPos = Get_ItemPos((_int)m_iSelectedIndex);
	static_cast<CRect_Transform*>(m_pHighlighter->Get_Transform())->Set_Offset(fPos.x, fPos.y);

}

_uint CUIListSelector::Select_NextItem()
{
	_uint iNextIndex = m_iSelectedIndex + 1;
	if (iNextIndex >= Get_ItemCount())
		iNextIndex = 0;
	Select_Item(iNextIndex);
	return iNextIndex;
}
