#pragma once
#include "Bullet.h"

BEGIN(Client)
class CBullet_FireTornado :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_FireTornado";

private:
	explicit CBullet_FireTornado(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_FireTornado(const CBullet& Prototype);
	virtual ~CBullet_FireTornado() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Launch(_float fDamage, _vector vPosition) override;

	void On_BallEffectAnimEnd(CEffModel* pModel);
	void On_RemainEffectAnimEnd(CEffModel* pModel);


private:
	CEffModelObject* m_pBallEffect = { nullptr };
	CEffModelObject* m_pSplashCastEffect = { nullptr };
	CEffModelObject* m_pSplashEndEffect = { nullptr };

	_float m_fTimeAcc = { 0.f };
	_float m_fDamgInterval = { 0.5f };
public:
	static CBullet_FireTornado* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END