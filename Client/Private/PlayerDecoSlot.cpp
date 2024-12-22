#include "stdafx.h"
#include "PlayerDecoSlot.h"

CPlayerDecoSlot::CPlayerDecoSlot(DECO_ITEM_TYPE eType)
	: CBase()
	, m_eDecoType(eType)
{
}

const _char* CPlayerDecoSlot::Get_IconTag()
{
	if (m_pItemData != nullptr)
		return m_pItemData->strIconImageTag;
	else
		return nullptr;
}

_bool CPlayerDecoSlot::Has_Item()
{
	return m_pItemData != nullptr;
}

HRESULT CPlayerDecoSlot::Insert_Item(const DECO_ITEM_DATA* pData)
{
	assert(pData != nullptr);
	if (pData->eITemType != ITEM_TYPE::DECO)
		return E_FAIL;
	if (pData->eDecoType != m_eDecoType)
		return E_FAIL;
	m_pItemData = pData;
	return S_OK;
}

const DECO_ITEM_DATA* CPlayerDecoSlot::Pop_Item()
{
	const DECO_ITEM_DATA* pItem = m_pItemData;
	m_pItemData = nullptr;
	return pItem;
}

CPlayerDecoSlot* CPlayerDecoSlot::Create(DECO_ITEM_TYPE eType)
{
	return new CPlayerDecoSlot(eType);
}

void CPlayerDecoSlot::Free()
{
	__super::Free();
}
