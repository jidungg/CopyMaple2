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
class CUIPlayerInfo;
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

	CUIInventory* Get_Inventory() { return m_pInventory; }
	CUIQuickSlotBundle* Get_QuckSlotBundle() { return m_pQuickSlotBundle; }

	void Set_QuickItem(KEY eHotKey, IQuickItem* pItem);
	void Initialize_PlayerInfo(CPlayer* pPalyer);
	void Update_Inven_Slot(_uint iIndex, CInvenSlot* pSlot);
	void Update_CastingRatio(_float fRatio);
	void Set_CastingBarVisible(_bool bVisible);
private:
	CUIQuickSlotBundle* m_pQuickSlotBundle = { nullptr };
	CUIInventory* m_pInventory = { nullptr };
	CUIBar* m_pCastingBar = { nullptr };
	CUIPlayerInfo* m_pMainHPBar = { nullptr };
public:
	void Free() override;
};
#define UIBUNDLE CUIBundle::GetInstance()
END