#include "stdafx.h"
#include "UIList.h"
#include "GameInstance.h"
#include "UIItemIndicator.h"


CUIList::CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUIList::CUIList(const CUIList& Prototype)
	: CUIContainer(Prototype)
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
	m_iItemCount = pDesc->listData->size();
	m_iColCount = pDesc->iColumnCount;
	m_iRowCount = pDesc->iRowCount;

	_uint iUIItemCount = Get_ColCount() * Get_RowCount();
	m_vecUIItem.resize(iUIItemCount);
	CUIPanel::PANEL_DESC desc{};
	desc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	desc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	desc.fSizeX = m_fItemWidth;
	desc.fSizeY = m_fItemHeight;
	desc.vBorder = { 3,3,3,3 };
	for (_uint i = 0; i < iUIItemCount; i++)
	{
		desc.fXOffset = i % m_iColCount * (m_fItemWidth + m_fItemMarginX);
		desc.fYOffset = i / m_iColCount * (m_fItemHeight + m_fItemMarginY);
		CUIObject* pItem = static_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, m_eBackTexProtoLev, m_szBackTexProtoTag, &desc));
		Add_Child(pItem);
		IUIListItemEntry* pIListItemEntry = dynamic_cast<IUIListItemEntry*>(pItem);
		if (pIListItemEntry == nullptr)
			return E_FAIL;
		pIListItemEntry->On_CreateListItemEntry(this, i);
		m_vecUIItem[i] = pIListItemEntry;
	}

	if (FAILED(Set_ItemData(pDesc->listData)))
		return E_FAIL;
	return S_OK;
}
HRESULT CUIList::Set_ItemData(list<UIListItemData*>* listData)
{
	if (listData->size() > m_iItemCount)
		return E_FAIL;

	_uint iItemIdx = 0;
	for (auto& data : *listData)
	{
		if (FAILED(m_vecUIItem[iItemIdx]->On_ListItemDataSet(data)))
			return E_FAIL;
		iItemIdx++;
	}
	return S_OK;
}



HRESULT CUIList::Render()
{
	if (false == m_bActive) return S_OK;

	for (_uint i = 0; i < m_iItemCount; i++)
	{
		if(Is_VisibleRow(Get_ItemRow(i)))
			m_vecUIItem[i]->Render_ListEntry();
	}
	return S_OK;
}


void CUIList::Set_YOffset(_float fYPos)
{
	static_cast<CRect_Transform*>(m_pTransformCom)->Set_Offset(static_cast<CRect_Transform*>(m_pTransformCom)->Get_Offset().x, fYPos);
}


_bool CUIList::Is_VisibleRow(_uint iRow)
{
	return iRow >= m_iVisibleRowStart && iRow < m_iVisibleRowStart + m_iVisibleRowCount;
}

_uint CUIList::Get_ItemRow(_uint iIndex)
{
	return iIndex / m_iColCount;
}

_float2 CUIList::Get_ItemPos(_int iIndex)
{
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_uint iXMaxCount = fSize.x / m_fItemWidth;
	_uint iYMaxCount = fSize.y / m_fItemHeight;
	_float2 vPos;
	vPos.x = iIndex % iXMaxCount * (m_fItemWidth + m_fItemMarginX) ;
	vPos.y = iIndex / iXMaxCount * (m_fItemHeight + m_fItemMarginY) ;
	return vPos;
}

//_uint CUIList::Get_RowCount()
//{
//	return (_uint)ceilf((_float)m_iItemCount / (_float)m_iColCount);
//}

_float CUIList::Get_YOffset()
{
	return 	static_cast<CRect_Transform*>(m_pTransformCom)->Get_Offset().y;
}

