#pragma once
#include "UIButton.h"

BEGIN(Client)
class CUIInventory;

class CUIInvenTabButton :
    public CUIButton
{
public:
	typedef struct UIInvenTabButtonDesc : public CUIButton::BUTTON_DESC
	{
		CUIInventory* pUIInventory = { nullptr };
		ITEM_TYPE  eItemType = { ITEM_TYPE::LAST };
	}UIINVENTABBUTTON_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIInvenTabButton";

public:
	explicit CUIInvenTabButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIInvenTabButton(const CUIInvenTabButton& Prototype);
	virtual ~CUIInvenTabButton() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void On_MouseClick()override;
private:
    ITEM_TYPE m_eItemType = { ITEM_TYPE::LAST };
    CUIInventory* m_pUIInventory = { nullptr };

public:
	static CUIInvenTabButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END