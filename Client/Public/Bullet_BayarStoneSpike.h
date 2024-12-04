#pragma once
#include "Bullet.h"
#include "set"

BEGIN(Client)
class CEffModelObject;
class CBullet_BayarStoneSpike :
    public CBullet
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_StoneSpike";

private:
	explicit CBullet_BayarStoneSpike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_BayarStoneSpike(const CBullet& Prototype);
	virtual ~CBullet_BayarStoneSpike() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Launch(_float fDamage, _vector vPosition) override;

private:

	_bool m_bSpiked = { false };
	CEffModelObject* m_pPrecursorEffect = { nullptr };
	CEffModelObject* m_pRockCubeEffect = { nullptr };
	set<CGameObject*> m_setHitObject;
	_float m_fTimeAcc = { 0.f };
	_float m_fWaitTime = { 1.f };
	_float m_fTmp = 0.f;
public:
	static CBullet_BayarStoneSpike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END