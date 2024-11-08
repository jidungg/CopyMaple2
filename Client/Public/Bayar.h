#pragma once
#include "Monster.h"

BEGIN(Engine)
class CCollider_Sphere;
END
BEGIN(Client)
class CSkill;
class CBoneModelObject;
class CMimicBoneModelObject;
class CFace;
class CInventory;
class CItemObjet;

class CBayar :
    public CMonster
{
	enum PART_ID
	{
		BODY,
		DETECTION,
		LEFT_ARM,
		RIGHT_ARM,
		PART_LAST
	};

	enum ANIM_CONDITION
	{
		AC_ANIMEND
		, AC_DETECTED
		, AC_ATTACKTRIGGER
		,AC_ATTACKIDX
		, AC_ISATTACK
		,AC_MOVE
		,AC_WALK
		, AC_LAST
	};
    enum STATE
    {
		AS_IDLE = 0
		,AS_ATTACK_1_A
		,AS_ATTACK_1_B
		,AS_ATTACK_1_C
		,AS_ATTACK_1_D
		,AS_ATTACK_1_E
		,AS_ATTACK_1_F
		,AS_ATTACK_1_G
		,AS_ATTACK_1_H
		,AS_ATTACK_2_A 
		,AS_ATTACK_2_B = 10
		,AS_ATTACK_2_C
		,AS_ATTACK_2_D
		,AS_ATTACK_2_E
		,AS_ATTACK_2_G
		,AS_ATTACK_2_H
		,AS_ATTACK_3_E
		,AS_ATTACK_3_G
		,AS_ATTACK_3_H
		,AS_ATTACK_4_G
		,AS_ATTACK_IDLE = 20
		,AS_BORE
		,AS_DAMG_A
		,AS_DAMG_B
		,AS_DEAD_A
		,AS_DEAD_B
		,AS_DEAD_DAMG_A
		,AS_DEAD_DAMG_B
		,AS_DEAD_IDLE_A
		,AS_JUMP
		,AS_JUMP_B = 30
		,AS_JUMP_DOWN
		,AS_JUMP_LAND
		,AS_JUMP_READY
		,AS_JUMP_UP
		,AS_REGEN
		,AS_RUN = 36
		,AS_STUN
		,AS_WALK_A
        ,AS_LAST
		
		,BS_BORN = 40
		,BS_IDLE
		,BS_DETECTED 
		,BS_ATTACK1 
		,BS_MOVE
		,BS_JUMP
		,BS_DAMAGED 
		,BS_DEAD 
		,BS_STUN
		,BS_LAST 

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
	virtual void Update_Collider()override;
	virtual _bool Check_Collision(CGameObject* pOther) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components(CHARACTER_DESC* pDesc);
	virtual HRESULT Ready_AnimStateMachine() override;

private:

	vector<const XMFLOAT4X4*> m_vecPartsMatrix;


public:
	static CBayar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END