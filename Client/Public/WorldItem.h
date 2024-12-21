#pragma once
#include "EffModelObject.h"
#include "Item.h"

BEGIN(Client)
class CCubeTerrain;
class CWorldItem :
    public CEffModelObject
{
public:
	typedef struct WorldItemDesc : public CEffModelObject::EffectObjDesc
	{
		const ITEM_DATA* pItemData = { nullptr };
		_uint iStackCount = { 1 };
		CCubeTerrain* pTerrain = { nullptr };
	}WORLDITEM_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CWorldItem";
protected:
	explicit CWorldItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWorldItem(const CEffModelObject& Prototype);
	virtual ~CWorldItem() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual _bool Check_Collision(CGameObject* pOther) override;

	void PopUp();
	void Set_Item(const ITEM_DATA* pItemData, _uint iStackCount = 1);
private:
	const ITEM_DATA* m_pItemData = { nullptr };
	_uint m_iStackCount = { 1 };

	_bool m_bPopUp = { false };
	CCubeTerrain* m_pTerrain = { nullptr };
	_float m_fMagnetRange = { 1.f };
	_float m_fAcquireRange = { 0.1f };
	_float m_fUpForce = { 0.0f };
	_float m_fJumpForce = { 7.5f };
	_vector m_vPopMoveDirection = { 0,0,0,0 };
	_float m_fMoveSpeed = { 3.f };
	_float m_fUpforceAcc = { 1.5f  };
	_float m_fUpforceMax = { 1.f };
	
public:
	static CWorldItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END