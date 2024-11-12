#pragma once
#include "ModelObject.h"
BEGIN(Engine)
class CColliderBase;
END
BEGIN(Client)
class CCharacter;
class CBullet abstract :
    public CModelObject
{
protected:
	explicit CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual _bool Check_Collision(CGameObject* pOther)override;

	virtual void Invoke(_float fDamg, _vector vPosition);
	virtual void Invoke(_float fDamg, CGameObject* pTarget);
protected:
	TEAM m_eTeam = { TEAM::LAST };
	CColliderBase* m_pCollider = { nullptr };
	_float m_iDamage = { 0 };
	_bool m_bInvoke = { false };
	_float m_fTimeAcc = { 0.f };

public:

	virtual void Free();
};

END