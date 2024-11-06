#pragma once
#include "UISlot.h"
BEGIN(Client)
class IQuickItem;
class CUIQuickSlot :
    public CUISlot
{
public:
	typedef struct QuickSlotDesc : public CUISlot::UISLOT_DESC
	{
		KEY eHotKey = { KEY::LAST };
	}QUICKLSLOT_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_QuickSlot";

public:
	explicit CUIQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIQuickSlot(const CUIQuickSlot& Prototype);
	virtual ~CUIQuickSlot() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	// CUISlot을(를) 통해 상속됨
	bool Has_Item() override;

	virtual void On_MouseClick() override;

	void Set_QuickItem(IQuickItem* pItem);
	void Set_HotKey(KEY eKey) { m_eHotKey = eKey; }
private:
	IQuickItem* m_pQuickItem = { nullptr };

	KEY m_eHotKey = { KEY::LAST };
public:
	static CUIQuickSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END