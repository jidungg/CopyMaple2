#pragma once
#include "UIButton.h"
#include "Item.h"
#include "UIListItemEntry.h"

BEGIN(Client)
class CUIList;
class CUIIcon;
class CUIButtonItemIndicator :
	public CUIButton, public IUIListItemEntry
{
protected:
	CUIButtonItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIButtonItemIndicator(const CUIButtonItemIndicator& Prototype);
	virtual ~CUIButtonItemIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype(LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;

	HRESULT On_ListItemDataSet(const ITEM_DATA* data) override;
	virtual HRESULT Render_ListEntry() override;
	virtual void On_CreateListItemEntry(CUIList* pList, _uint iIndex) override;
	virtual void Set_ListItemEntryActive(_bool bActive) override;
	const ITEM_DATA* Get_ItemDesc() { return(m_pItemDesc); }
protected:
	virtual void Call_Callback(const ButtonCallback& fCallback) override;
	const ITEM_DATA* m_pItemDesc = { nullptr };
	CUIIcon* m_pIcon = { nullptr };
	_float4 m_vIconBorder = { 3,3,3,3 };
public:
	static CUIButtonItemIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

	// IUIListItemEntry을(를) 통해 상속됨
	void Set_Offset(_float iX, _float iY) override;
};

END