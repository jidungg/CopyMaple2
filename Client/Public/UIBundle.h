#pragma once
#include "UIContainer.h"
#include "Item.h"

BEGIN(Client)
class IQuickItem;
class CUIQuickSlotBundle;
class CUIInventory;
class CUIBar;
class CInvenSlot;
class CPlayer;
class CUIMainHUDGuage;
class CUIPlayerInfo;
class CPlayerEquipSlot;
class CPlayerDecoSlot;
class CPlayerInfoSlot;
class CUIBundle :
    public CUIContainer
{

	DECLARE_SINGLETON(CUIBundle)

public:
	typedef struct UIBUNDLE_DESC : public CUIObject::UIOBJECT_DESC
	{
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
	}BUNDLE_DESC;
protected:
	explicit CUIBundle();
	~CUIBundle() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;


	void Initialize_PlayerInfo(CPlayer* pPalyer);
	void Update_Inven_Slot(_uint iIndex, CInvenSlot* pSlot);
	void Update_CastingRatio(_float fRatio);
	void Update_PlayerInfo_Slot(_uint iSlotId, CPlayerInfoSlot* pItemSlot);
	void Set_CastingBarVisible(_bool bVisible);

	CUIInventory* Get_Inventory() { return m_pInventory; }
	CUIQuickSlotBundle* Get_QuckSlotBundle() { return m_pQuickSlotBundle1; }
	void Set_QuickItem(KEY eHotKey, IQuickItem* pItem);
private:
	CUIQuickSlotBundle* m_pQuickSlotBundle1 = { nullptr };
	CUIQuickSlotBundle* m_pQuickSlotBundle2 = { nullptr };
	CUIInventory* m_pInventory = { nullptr };
	CUIBar* m_pCastingBar = { nullptr };
	CUIMainHUDGuage* m_pMainHPBar = { nullptr };
	CUIPlayerInfo* m_pPlayerInfoUI = { nullptr };
public:
	void Free() override;
};
#define UIBUNDLE CUIBundle::GetInstance()
END