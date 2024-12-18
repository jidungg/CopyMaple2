#include "stdafx.h"
#include "UIList.h"
#include "GameInstance.h"
#include "UIItemIndicator.h"
#include "ObjectPool.h"
#include <UIInvenSlotEntry.h>

CUIList::CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUIList::CUIList(const CUIList& Prototype)
	: CUIContainer(Prototype)
	, m_eItemEntryProtoLev(Prototype.m_eItemEntryProtoLev)
	, m_szItemEntryProtoTag(Prototype.m_szItemEntryProtoTag)
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
	m_eItemEntryProtoLev = pDesc->eItemEntryProtoLev;
	m_szItemEntryProtoTag = pDesc->szItemEntryProtoTag;
	m_iColCount = pDesc->iColumnCount;
	m_iRowCount = pDesc->iRowCount;

	CUIPanel::PANEL_DESC desc{};
	desc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	desc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	desc.fSizeX = m_fItemWidth;
	desc.fSizeY = m_fItemHeight;
	desc.vBorder = { 3,3,3,3 };
	CUIObject* pItem = static_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, m_eItemEntryProtoLev, m_szItemEntryProtoTag, &desc));
	if(pItem)
		m_pItemEntryPool = CObjectPool< CUIObject>::Create(pItem,&desc, pDesc->iObjectPoolSize);
	else
		return E_FAIL;

	Resize(m_iRowCount, m_iColCount);

	//if (FAILED(Set_ItemData(pDesc->listData)))
	//	return E_FAIL;
	return S_OK;
}
HRESULT CUIList::Set_ItemData(list<const ITEM_DATA*>* listData)
{
	if (listData->size() > Get_ItemCount())
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




HRESULT CUIList::Resize(_uint iRow, _uint iCol)
{
	_uint iDiff = iRow * iCol- Get_ItemCount();
	//늘리기
	if (iDiff > 0)
	{
		CUIPanel::PANEL_DESC desc{};
		desc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		desc.ePivotType = CORNOR_TYPE::LEFT_TOP;
		desc.fSizeX = m_fItemWidth;
		desc.fSizeY = m_fItemHeight;
		desc.vBorder = { 3,3,3,3 };
		while (m_vecUIItem.size() < iRow*iCol)
		{
			CUIObject* pItem = m_pItemEntryPool->Get_Object();
			Add_Child(pItem);
			Safe_AddRef(pItem);
			IUIListItemEntry* pEntry = dynamic_cast<IUIListItemEntry*>(pItem);
			pEntry->On_CreateListItemEntry(this, m_vecUIItem.size());
			m_vecUIItem.push_back(pEntry);
		}
	}
	//줄이기
	else
	{
		while (m_vecUIItem.size() > iRow * iCol)
		{
			CUIObject* pItem = dynamic_cast<CUIObject*>(m_vecUIItem.back());
			if (pItem)
			{
				Safe_Release(pItem);
				Remove_Child(pItem);
				m_pItemEntryPool->Return_Object(pItem);
				m_vecUIItem.pop_back();
			}
		}
	}
	m_iRowCount = iRow;
	m_iColCount = iCol;
	Reposition();
	return S_OK;
}

void CUIList::Reposition()
{
	_uint iItemCount = Get_ItemCount();
	for (_uint i = 0; i < iItemCount; i++)
	{
		_float fXOffset = i % m_iColCount * (m_fItemWidth + m_fItemMarginX);
		_float fYOffset = i / m_iColCount * (m_fItemHeight + m_fItemMarginY);
		m_vecUIItem[i]->Set_Offset(fXOffset, fYOffset);
	}

}



HRESULT CUIList::Render()
{
	if (false == m_bActive) return S_OK;
	_uint iItemCount = Get_ItemCount();
	for (_uint i = 0; i < iItemCount; i++)
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

