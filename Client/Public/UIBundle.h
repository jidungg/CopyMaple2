#pragma once
#include "UIContainer.h"

BEGIN(Client)
class IQuickItem;
class CUIQuickSlotBundle;
class CUIInventory;
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

	CUIInventory* Get_Inventory() { return m_pInventory; }
	CUIQuickSlotBundle* Get_QuckSlotBundle() { return m_pQuickSlotBundle; }

	void Set_QuickItem(KEY eHotKey, IQuickItem* pItem);
	void Update_Slot(ITEM_TYPE eType, _uint iIndex);
private:
	CUIQuickSlotBundle* m_pQuickSlotBundle = nullptr;
	CUIInventory* m_pInventory = nullptr;
public:
	void Free() override;
};
#define UIBUNDLE CUIBundle::GetInstance()
END