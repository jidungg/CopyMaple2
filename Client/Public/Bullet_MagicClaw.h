#pragma once
#include "Bullet.h"

BEGIN(Client)
class CBullet_MagicClaw :
    public CBullet
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_MagicClaw";
private:
	explicit CBullet_MagicClaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_MagicClaw(const CBullet& Prototype);
	virtual ~CBullet_MagicClaw() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Launch(_float fDamage, CGameObject* pTarget = nullptr) override;

	void On_EffectAnimEnd(CEffModel* pModel) {Set_Active(false);}

private:
	vector<_float> m_fAttackTime = {0, 0.25};
	_uint m_iAttackCount = { 0};
	_float m_fTimeAcc = { 0.f };

	CEffModelObject* m_pEffect = { nullptr };
public:
	static CBullet_MagicClaw* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();

};

END