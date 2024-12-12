#pragma once
#include "EffModelObject.h"
BEGIN(Engine)
class CColliderBase;
END
BEGIN(Client)
class CCharacter;
class CBullet abstract :
    public CGameObject
{
public:
	typedef struct BulletDesc : CGameObject::GAMEOBJECT_DESC
	{
		CGameObject* pShooter = { nullptr };
		EFF_MODEL_ID eHitEffect = { EFF_MODEL_ID::LAST };
	}BULLET_DESC;
protected:
	explicit CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	void SearchTarget(list<CGameObject*>* pOutList, LAYERID eLayerID);
	virtual _bool Check_Collision(CGameObject* pOther)override;

	virtual void Launch(_float fDamage, CGameObject* pTarget);
	virtual void Launch(_float fDamage, _fvector vPosition);
	virtual void Launch(_float fDamage);

protected:
	TEAM m_eTeam = { TEAM::LAST };
	CColliderBase* m_pCollider = { nullptr };
	_float m_fDamage = { 0 };
	CGameObject* m_pShooter = { nullptr };
	EFF_MODEL_ID m_eHitEffect = { EFF_MODEL_ID::LAST };
public:

	virtual void Free();
};

END