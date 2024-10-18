#pragma once
#include "UIPanel.h"
#include "Item.h"
#include "UIListItemEntry.h"

BEGIN(Client)
class CUIItemIndicator :
	public CUIPanel, public IUIListItemEntry
{
protected:
	CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIItemIndicator(const CUIItemIndicator& Prototype);
	virtual ~CUIItemIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype(LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;

	HRESULT On_ListItemDataSet(const UIListItemData* data) override;


protected:
	const ITEM_DESC* m_pItemDesc = nullptr;
	CUIPanel* m_pItemIcon = nullptr;

public:
	static CUIItemIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END