#pragma once
#include "UIPanel.h"
#include "UISlot.h"
BEGIN(Client)

class CInvenSlot;
class CUIInvenSlot :
    public CUISlot
{
public:
	typedef struct UIInvenSlotDesc : public CUISlot::UISLOT_DESC
	{
		CInvenSlot* pInvenSlot = { nullptr };
	}UIINVENSLOT_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIInvenSlot";
private:
	explicit CUIInvenSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIInvenSlot(const CUIInvenSlot& Prototype);
	virtual ~CUIInvenSlot() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	// CUISlot을(를) 통해 상속됨
	bool Has_Item() override;
	virtual void On_MouseClick()override;
	virtual void On_MouseRightClick()override;

	void Update_Slot();


private:
	CInvenSlot* m_pSlot = { nullptr };
public:
	static CUIInvenSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END