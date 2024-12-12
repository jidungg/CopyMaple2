#pragma once
#include "Bullet.h"
#include "set"
BEGIN(Client)
class CBullet_BayarAttackD :
    public CBullet
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_BayarAttackD";
private:
	explicit CBullet_BayarAttackD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_BayarAttackD(const CBullet& Prototype);
	virtual ~CBullet_BayarAttackD() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Compute_Matrix() override;

	virtual void Launch(_float fDamage) override;

	void On_EffectAnimEnd(CEffModel* pModel) { Set_Active(false); }
private:
	_bool m_bAttack = { false };
	const _char* m_szAttachBoneName = { "Bip01 R Hand" };
	const _float4x4* m_pSocketMatrix = { nullptr };
	set<CGameObject*> m_setHitObject;
public:
	static CBullet_BayarAttackD* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END