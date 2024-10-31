#pragma once
#include "Character.h"

BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CSkill;
class CBoneModelObject;
class CMimicBoneModelObject;
class CFace;
class CInventory;
class CItemObjet;

class CBayar :
    public CCharacter
{
	enum class BEHAVIOR_STATE
	{
		IDLE,
		CHASE,
		ATTACK,
		DEAD,
		LAST
	};
    enum class ANIM_STATE
    {
		IDLE
		,ATTACK_1_A
		,ATTACK_1_B
		,ATTACK_1_C
		,ATTACK_1_D
		,ATTACK_1_E
		,ATTACK_1_F
		,ATTACK_1_G
		,ATTACK_1_H
		,ATTACK_2_A
		,ATTACK_2_B
		,ATTACK_2_C
		,ATTACK_2_D
		,ATTACK_2_E
		,ATTACK_2_G
		,ATTACK_2_H
		,ATTACK_3_E
		,ATTACK_3_G
		,ATTACK_3_H
		,ATTACK_4_G
		,ATTACK_IDLE
		,BORE
		,DAMG_A
		,DAMG_B
		,DEAD_A
		,DEAD_B
		,DEAD_DAMG_A
		,DEAD_DAMG_B
		,DEAD_IDLE_A
		,JUMP
		,JUMP_B
		,JUMP_DOWN
		,JUMP_LAND
		,JUMP_READY
		,JUMP_UP
		,REGEN
		,RUN
		,STUN
		,WALK_A
        ,LAST
    };

protected:
	explicit CBayar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBayar(const CBayar& Prototype);
	virtual ~CBayar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	HRESULT Ready_AnimStateMachine();
	HRESULT Ready_FaceStateMachine();
	HRESULT Ready_Skill();

	void On_StateChange(_uint iState);
	void On_SubStateChange(_uint iSubState);
	void On_FaceStateChange(_uint iState);

	virtual void Use_Skill(CSkill* pSkill) override;

private:
	CCollider* m_pDetectionCollider = { nullptr };
public:
	static CBayar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END