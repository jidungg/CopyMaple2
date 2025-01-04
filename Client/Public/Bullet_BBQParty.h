#pragma once
#include "Bullet.h"
BEGIN(Engine)
class CCollider_Cylinder;
END
BEGIN(Client)
class CBullet_BBQParty :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_BBQParty";
private:
	explicit CBullet_BBQParty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_BBQParty(const CBullet& Prototype);
	virtual ~CBullet_BBQParty() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual _bool Check_Collision(CGameObject* pOther)override;
	virtual void Launch(CSkill* pSkill, _fvector vPosition = { 0,0,0,1 }) override;
	virtual void Launch(CSkill* pSkill, CGameObject* pTarget = nullptr) override;

	void On_KeepEffectAnimEnd(CEffModel* pModel);
	void On_SplashCastEffectAnimEnd(CEffModel* pModel);
	void On_SplashEndEffectAnimEnd(CEffModel* pModel);


private:

	CEffModelObject* m_pKeepEffect = { nullptr };
	CEffModelObject* m_pSplashCastEffect = { nullptr };
	CEffModelObject* m_pSplashInvokeEffect = { nullptr };
	CEffModelObject* m_pSplashEndEffect = { nullptr };

	_float m_fMaxDuration = { 10.f };
	_float m_fDurationAcc = { 0.f };
	_float m_fDamgInterval = {2.f };
	_float m_fDamgTimeAcc = { 0.f };
	_uint m_iMaxSplashCount = 3;
	_uint m_iSplashCount = 0;
public:
	static CBullet_BBQParty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END