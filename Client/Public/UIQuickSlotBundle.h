#pragma once
#include "UIContainer.h"

BEGIN(Client)
class IQuickItem;
class CUIQuickSlot;
class CUIQuickSlotBundle :
    public CUIContainer
{
public:
	typedef struct QuickSlotBundleDesc : public CUIObject::UIObjectDesc
	{
		_float fSlotSize = { 60.f };
		_float fSlotMargin = { 10.f };
		vector<KEY> vecHotKey;
		_uint iSlotRowCount = { 1 };
		_uint iSlotColCount = { 4 };

	}QUICKSLOTBUNDLE_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_QuickSlotBundle";
protected:
	explicit CUIQuickSlotBundle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIQuickSlotBundle(const CUIQuickSlotBundle& Prototype);
	virtual ~CUIQuickSlotBundle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTImeDelta) override;

	void Recevie_Input(KEY eKey);
	void Set_QuickItem(KEY eHotKey,  IQuickItem* pItem);

private:
	map<KEY, CUIQuickSlot*> m_mapQuickSlot;

public:
	static CUIQuickSlotBundle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END