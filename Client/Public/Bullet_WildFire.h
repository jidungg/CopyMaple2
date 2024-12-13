#pragma once
#include "Bullet.h"
#include "set"
BEGIN(Engine)
class CCollider_Frustum;
class CGameObject;
END

BEGIN(Client)   
class CBullet_WildFire :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_WildFire";

private:
	explicit CBullet_WildFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_WildFire(const CBullet_WildFire& Prototype);
	virtual ~CBullet_WildFire() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual _bool Check_Collision(CGameObject* pOther)override;
	virtual void Launch(_float fDamage, _fvector vPosition = { 0,0,0,1 });
private:
	CEffModelObject* m_pCastEffect2 = { nullptr };
	_bool m_bExpanding = { false };
	_float m_fMaxRange = {8.f };
	_float m_fMinRange = { 0.f };
	_float m_fCurrentRange = { 0.f };
	_float m_fExpandSpeed = { 10.f };
	set<CGameObject*> m_setHitObject;
public:
	static CBullet_WildFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};

END