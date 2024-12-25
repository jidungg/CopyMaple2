#pragma once
#include "UICommonWindow.h"
#include "Item.h"

BEGIN(Client)
class CPlayerInfo;
class CUIPanel;
class CUIPlayerInfoSlot;
class CPlayerEquipSlot;
class CPlayerInfoSlot;
class CUIModelPad;
//아이템 장착 상황, 공격력, 방어력, 아이템 점수, 스탯
class CUIPlayerInfo :
    public CUICommonWindow
{
public:
	enum TAB_ID
	{
		EQUIP,
		DECO,
		TAB_LAST
	};
	enum SLOT_ID
	{
		HAT,
		TOP,
		BOTTOM,
		GLOVES,
		SHOES,
		CAPE,
		WEAPON,
		EAR,
		FACE,
		SLOT_LAST
	};
public:
	typedef struct UIPlayerInfoDesc : public CUICommonWindow::UICOMMONWINDOW_DESC
	{
		CPlayerInfo* pPlayerInfo = { nullptr };
	}UIPLAYERINFO_DESC;

public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIPlayerInfo";
private:
	explicit CUIPlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIPlayerInfo(const CUIPlayerInfo& Prototype);
	virtual ~CUIPlayerInfo() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;

	void Update_Slot(SLOT_ID eSlotID, CPlayerInfoSlot* pItemSlot);
	void Switch_Tab(TAB_ID eTab);
	void Select_Slot(SLOT_ID eSlotID);

public:
    static std::list<SLOT_ID> Item_To_SlotID(const ITEM_DATA* pItemData);
private:
	HRESULT Ready_Slots();
private:
	CPlayerInfo* m_pPlayerInfo = { nullptr };

	_float2 m_fSlotSize = { 68,68 };
	_float2 m_fCommonMargin = { 6,6};
	_float m_fHeaderHeight = { 55 };
	_uint m_iVisibleRowCount = { 8 };
	_uint m_iVisibleColCount = { 6 };
	_float2 m_fBackSize = { 422,451 };
	_float2 m_fBackBorderSize = { 7,7};
	_float2 m_fInnerBackBorderSize = { 6,6};
	_float2 m_fDashBoardSize = { 239,457 };

	CUIPanel* m_pBackBorder = { nullptr };
	CUIPanel* m_pBackPanel = { nullptr };
	CUIPanel* m_pDashBoardBack = { nullptr };
	TAB_ID m_eCurrentTab = { TAB_ID::EQUIP };
	CUIPlayerInfoSlot* m_arrSlot[SLOT_ID::SLOT_LAST] = { nullptr, };
	CUIModelPad* m_pModelPad = { nullptr };
public:
	static CUIPlayerInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END