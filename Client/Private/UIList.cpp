#include "stdafx.h"
#include "UIList.h"
#include "GameInstance.h"



CUIList::CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIList::CUIList(const CUIList& Prototype)
	: CUIPanel(Prototype)
	, m_eBackTexProtoLev(Prototype.m_eBackTexProtoLev)
	, m_szBackTexProtoTag(Prototype.m_szBackTexProtoTag)
	, m_fItemHeight(Prototype.m_fItemHeight)
	, m_fItemWidth(Prototype.m_fItemWidth)
	, m_fItemMarginX(Prototype.m_fItemMarginX)
	, m_fItemMarginY(Prototype.m_fItemMarginY)
{
}



HRESULT CUIList::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	UILIST_DESC* pDesc = static_cast<UILIST_DESC*>(pArg);
	m_fItemHeight = pDesc->fItemHeight;
	m_fItemWidth = pDesc->fItemWidth;
	m_fItemMarginX = pDesc->fItemMarginX;
	m_fItemMarginY = pDesc->fItemMarginY;
	m_eBackTexProtoLev = pDesc->eBackTexProtoLev;
	m_szBackTexProtoTag = pDesc->szBackTexProtoTag;


	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_uint iXMaxCount = _uint(fSize.x / m_fItemWidth);
	_uint iYMaxCount = _uint(fSize.y / m_fItemHeight);
	_uint iItemIdx = 0;
	for (auto& data : *pDesc->listData)
	{

		CUIObject::UIObjectDesc desc{};
		desc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		desc.ePivotType = CORNOR_TYPE::LEFT_TOP;
		desc.fXOffset = iItemIdx % iXMaxCount * (m_fItemWidth + m_fItemMarginX) + m_fItemMarginX / 2;
		desc.fYOffset = iItemIdx / iXMaxCount * (m_fItemHeight + m_fItemMarginY) + m_fItemMarginY / 2;
		desc.fSizeX = m_fItemWidth;
		desc.fSizeY = m_fItemHeight;

		CGameObject* pItem = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, m_eBackTexProtoLev, m_szBackTexProtoTag, &desc));
		Add_Child(pItem);
		IUIListItemEntry* pListItemEntry = dynamic_cast<IUIListItemEntry*>(pItem);
		pListItemEntry->m_iListItemIndex = iItemIdx;
		if (FAILED(pListItemEntry->On_ListItemDataSet(data)))
			return E_FAIL;
		iItemIdx++;
	}
	return S_OK;
}

_float2 CUIList::Get_ItemPos(_int iIndex)
{
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_uint iXMaxCount = fSize.x / m_fItemWidth;
	_uint iYMaxCount = fSize.y / m_fItemHeight;
	_float2 vPos;
	vPos.x = iIndex % iXMaxCount * (m_fItemWidth + m_fItemMarginX) + m_fItemMarginX / 2;
	vPos.y = iIndex / iXMaxCount * (m_fItemHeight + m_fItemMarginY) + m_fItemMarginY / 2;
	return vPos;
}
