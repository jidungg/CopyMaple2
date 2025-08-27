#pragma once
#include "UISlot.h"
#include "UIPlayerInfo.h"

BEGIN(Client)
class CPlayerEquipSlot;
class CPlayerDecoSlot;
class CPlayerInfoSlot;
class CUIPlayerInfoSlot :
    public CUISlot
{

public:
	typedef struct UIPlayerInfoSlotDesc : public CUISlot::UISLOT_DESC
	{
		CUIPlayerInfo* pPlayerInfo = { nullptr };
		CUIPlayerInfo::SLOT_ID eSlotId = { CUIPlayerInfo::SLOT_ID::SLOT_LAST };
	}UIPLAYERINFO_SLOT_DESC;

public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIPlayerInfoSlot";
private:
	explicit CUIPlayerInfoSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIPlayerInfoSlot(const CUIPlayerInfoSlot& Prototype);
	virtual ~CUIPlayerInfoSlot() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;

	virtual _bool Has_Item() override;
	virtual void On_MouseEnter()override;
	virtual void On_MouseRightClick()override;

	void Set_Item(const ITEM_DATA* pItem);

	CUIPlayerInfo::SLOT_ID Get_SlotID() { return m_eSlotId; }

private:
	_bool Is_DecoSlot() { return m_eSlotId == CUIPlayerInfo::SLOT_ID::FACE; }
private:
	CUIPlayerInfo::SLOT_ID m_eSlotId = { CUIPlayerInfo::SLOT_ID::SLOT_LAST };
	const ITEM_DATA* m_pItemData = { nullptr };
	CUIIcon* m_pBackPanel = { nullptr };

	CUIPlayerInfo* m_pIUPlayerInfo = { nullptr };
public:
	static CUIPlayerInfoSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END