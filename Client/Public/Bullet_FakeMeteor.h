#pragma once
#include "Bullet.h"

BEGIN(Client)
class CBullet_FakeMeteor :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_FakeMeteor";

private:
	explicit CBullet_FakeMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_FakeMeteor(const CBullet& Prototype);
	virtual ~CBullet_FakeMeteor() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual _bool Check_Collision(CGameObject* pOther)override;
	virtual void Launch(CSkill* pSkill, CGameObject* pTarget = nullptr) override;

	void On_SplashCastEffectAnimEnd(CEffModel* pModel);
	void On_SplashInvokeBEnd(CEffModel* pModel);
	void On_MeteorImpact();

private:

	CEffModelObject* m_pSplashCastEffect = { nullptr };
	CEffModelObject* m_pSplashInvokeEffectA = { nullptr };
	CEffModelObject* m_pSplashInvokeEffectB = { nullptr };
	CEffModelObject* m_pSplashEndEffect = { nullptr };

	_bool m_bInvoke = { false };
	_float m_fInvokeDelay = 5.f;
	_float m_fTimeAcc = { 0.f };
public:
	static CBullet_FakeMeteor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END