#pragma once
#include "Bullet.h"

BEGIN(Client)
class CEffModelObject;
class CBullet_Kindling :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_Kindling";
private:
	explicit CBullet_Kindling(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_Kindling(const CBullet& Prototype);
	virtual ~CBullet_Kindling() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Launch(CSkill* pSkill, CGameObject* pTarget = nullptr) override;

	virtual void On_Collision(CGameObject* pOther);
	void On_EffectAnimEnd(CEffModel* pModel) { Set_Active(false); }

private:
	_float m_fSpeed = { 5.f };
	CEffModelObject* m_pEffect = { nullptr };
public:
	static CBullet_Kindling* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END