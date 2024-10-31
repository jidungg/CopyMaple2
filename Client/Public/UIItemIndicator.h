#pragma once
#include "UIButton.h"
#include "Item.h"
#include "UIListItemEntry.h"

BEGIN(Client)
class CUIItemIndicator :
	public CUIButton, public IUIListItemEntry
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
	const ITEM_DESC* Get_ItemDesc() { return(m_pItemDesc); }
protected:
	virtual void Call_Callback(const ButtonCallback& fCallback) override;
	const ITEM_DESC* m_pItemDesc = nullptr;
	CTexture* m_pIconTexure = nullptr;
	CRect_Transform* m_pIconTransform = nullptr;

public:
	static CUIItemIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END