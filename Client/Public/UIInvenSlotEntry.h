#pragma once
#include "UIButton.h"
#include "Item.h"
#include "UIListItemEntry.h"


BEGIN(Client)
//UIList에 등록돼서 사용됨.
//우클릭 하면 아이템 장착 등
class CUIIcon;
class CInvenSlot;
class CUIFont;
class CUIInvenSlotEntry :
    public CUIButton, public IUIListItemEntry
{
public:
	typedef struct UIInvenSlotEntryDesc : public CUIButton::BUTTON_DESC
	{

	} UIInvenSlotEntryDesc;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIInvenSlotEntry";
protected:
	CUIInvenSlotEntry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIInvenSlotEntry(const CUIInvenSlotEntry& Prototype);
	virtual ~CUIInvenSlotEntry() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;

	HRESULT On_ListItemDataSet(const ITEM_DATA* data) override;
	HRESULT On_InvenListItemDataSet(pair<const ITEM_DATA* , _uint> Data) ;
	virtual HRESULT Render_ListEntry() override;
	virtual void On_CreateListItemEntry(CUIList* pList, _uint iIndex) override;
	virtual void On_MouseEnter()override;
	virtual void On_MouseRightClick()override;

	virtual void Set_ListItemEntryActive(_bool bActive) override;
	virtual void Set_Offset(_float iX, _float iY) override;
	void Set_StackCount(_uint iCount);

	const ITEM_DATA* Get_ItemDesc() { return(m_pItemDesc); }

protected:

	const ITEM_DATA* m_pItemDesc = { nullptr };
	CUIIcon* m_pIcon = { nullptr };
	_float4 m_vIconBorder = { 3,3,3,3 };
	CUIList* m_pList = { nullptr };

	CUIFont* m_pStackCountFont = { nullptr };
public:
	static CUIInvenSlotEntry* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END