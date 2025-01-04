#pragma once
#include "Bullet.h"
#include "set"

BEGIN(Client)
class CEffModelObject;
class CBullet_BayarAttackB :
    public CBullet
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Bullet_BayarAttackB";
private:
	explicit CBullet_BayarAttackB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_BayarAttackB(const CBullet& Prototype);
	virtual ~CBullet_BayarAttackB() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	virtual void Compute_Matrix() override;

	virtual void Launch(CSkill* pSkill) override;

	void On_EffectAnimEnd(CEffModel* pModel) { Set_Active(false); }
private:
	_bool m_bAttack = { false };
	const _char* m_szAttachBoneName = {  "Bip01 R Hand" };
	const _float4x4* m_pSocketMatrix = { nullptr };
	set<CGameObject*> m_setHitObject;
public:
	static CBullet_BayarAttackB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END