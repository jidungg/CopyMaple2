#pragma once
#include "UIPanel.h"
#include "UIInvenSlot.h"

BEGIN(Client)
class CInventory;
class CUIInventory :
    public CUIPanel
{
public:
	typedef struct UIInventoryDesc : public CUIPanel::PANEL_DESC
	{
		CInventory* pInventory = { nullptr };
	}UIINVENTORY_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIInventory";
private:
	explicit CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIInventory(const CUIInventory& Prototype);
	virtual ~CUIInventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	HRESULT Ready_Slots();

	void Set_InventoryTab(ITEM_TYPE eType);
	void Update_Slot(ITEM_TYPE eType, _uint iIndex);
private:
	CInventory* m_pInventory = { nullptr };
	_float2 m_fSlotSize = {};
	_float2 m_fSlotMargin = {};
	_float2 m_fSlotOffset = { 50,50 };
	vector<CUIInvenSlot*> m_vecSlot[(_uint)ITEM_TYPE::LAST];
	ITEM_TYPE m_eCurrentTab = ITEM_TYPE::EQUIP;
public:
	static CUIInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END