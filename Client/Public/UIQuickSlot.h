#pragma once
#include "UIPanel.h"
BEGIN(Client)
class IQuickItem
{
public:
	virtual void Use() abstract;
	const _char* Get_IconTag() { return m_szIconTag; }
protected:
	const _char* m_szIconTag = { nullptr };
	_float m_fCoolTime = { 0.f };
	_float m_fCoolTimeLeft = { 0.f };
	_float m_fCoolTimeLeftRatio = { 0.f };
};
class CUIQuickSlot :
    public CUIPanel
{
public:
	typedef struct QuickSlotDesc : public CUIPanel::PANEL_DESC
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
	virtual HRESULT Render()override;

	virtual void On_MouseClick() override;

	void Set_QuickItem(IQuickItem* pItem);
	void UnSet_QuickItem();
private:
	IQuickItem* m_pQuickItem = { nullptr };
	CUIPanel* m_pItemIcon = { nullptr };
	KEY m_eHotKey = { KEY::LAST };
public:
	static CUIQuickSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END