#include "stdafx.h"
#include "UIList.h"
#include "GameInstance.h"



CUIList::CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag)
	: CUIPanel(pDevice, pContext), m_eBackTexProtoLev(eBackTexProtoLev), m_szBackTexProtoTag(szBackTexProtoTag)
{
}

CUIList::CUIList(const CUIList& Prototype)
	: CUIPanel(Prototype), m_eBackTexProtoLev(Prototype.m_eBackTexProtoLev), m_szBackTexProtoTag(Prototype.m_szBackTexProtoTag)
{
}



HRESULT CUIList::Set_Data(list<UIListItemData*>& listData)
{

	m_pChilds.clear();
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_uint iXMaxCount = fSize.x / m_fItemWidth;
	_uint iYMaxCount = fSize.y / m_fItemHeight;
	_uint iItemIdx = 0;
	for (auto& data : listData)
	{

		CUIObject::UIObjectDesc desc{};
		desc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		desc.ePivotType = CORNOR_TYPE::LEFT_TOP;
		desc.fXOffset = iItemIdx % iXMaxCount * (m_fItemWidth + m_fItemMarginX) + m_fItemMarginX / 2;
		desc.fYOffset = iItemIdx / iYMaxCount * (m_fItemHeight + m_fItemMarginY) + m_fItemMarginY / 2;
		desc.fSizeX = m_fItemWidth;
		desc.fSizeY = m_fItemHeight;

		CGameObject* pItem = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, m_eBackTexProtoLev, m_szBackTexProtoTag, &desc));
		Add_Child(pItem);
		if (FAILED(dynamic_cast<IUIListItemEntry*>(pItem)->On_ListItemDataSet(data)))
			return E_FAIL;
		iItemIdx++;
	}
	return S_OK;
}