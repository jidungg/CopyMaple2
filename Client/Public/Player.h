#pragma once
#include "Client_Defines.h"
#include "Pawn.h"

BEGIN(Engine)
class CCamera_Trace;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayer :
	public CPawn
{
public:
	enum class ANIM_STATE {
		attack_01
		,attack_idle
		,bbqparty_1
		,bbqparty_2
		,bore_a
		,bore_b
		,bore_c
		,climb_down_land
		,climb_down_take
		,climb_idle
		,climb_l_down
		,climb_l_up
		,climb_left
		,climb_r_down
		,climb_r_up
		,climb_right
		,climb_up_land
		,climb_up_take
		,concentration
		,damg_a
		,damg_b
		,damg_c
		,dash_jump
		,dead
		,dead2
		,down_idle_a
		,down_idle_b
		,down_idle_c
		,down_idle_d
		,equip_chanage_cap
		,equip_change_body
		,equip_change_glove
		,equip_change_hair
		,equip_change_head
		,equip_change_idle
		,equip_change_mantle
		,equip_change_shoes
		,fakemeteor_1
		,fakemeteor_2
		,firetornado
		,fishing_bore_a
		,fishing_bore_b
		,fishing_end
		,fishing_idle
		,fishing_reel
		,fishing_start
		,flalmeburst
		,flamewave
		,fly_idle
		,fly_run
		,functobj_chair
		,hold_idle
		,idle
		,jump_attack
		,jump_damg
		,jump_dash_down
		,jump_dash_fall
		,jump_dash_land
		,jump_down_a
		,jump_down_b
		,jump_fall_a
		,jump_fall_b
		,jump_flydamg
		,jump_land
		,jump_up_a
		,jump_up_b
		,kindling_01
		,kindling_02
		,kindling_03_a
		,kindling_03_b
		,knock_back
		,ladder_down_land
		,ladder_down_take
		,ladder_idle
		,ladder_l_down
		,ladder_l_up
		,ladder_left
		,ladder_r_down
		,ladder_r_up
		,ladder_right
		,ladder_up_land
		,ladder_up_take
		,magicclaw
		,object_react_a
		,object_react_b
		,object_react_c
		,object_react_d
		,object_react_e
		,prostrate
		,prostrate_crawl_a
		,prostrate_crawl_b
		,prostrate_idle
		,push_a
		,push_b
		,resurrection
		,run
		,sit_chair
		,sit_chair_bore_a
		,sit_chair_bore_b
		,sit_chair_idle
		,sit_chair_talk
		,sit_ground
		,sit_ground_bore_a
		,sit_ground_bore_b
		,sit_ground_idle
		,sit_ground_talk
		,staff_attack
		,staff_attack_idle
		,staff_damg_a
		,staff_damg_b
		,staff_dead
		,staff_jump_attack
		,staff_jump_down_a
		,staff_jump_down_b
		,staff_jump_land
		,staff_jump_up_a
		,staff_jump_up_b
		,staff_run
		,stand_a
		,stand_b
		,stand_c
		,stuck
		,stun
		,stun_frozen
		,talk_a
		,talk_b
		,teleport
		,trinityforce
		,vomit
		,walk
		,wildfire_02
		,wizard_bore_a
		,LAST
	};
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{

	}PLAYER_DESC;

protected:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void Receive_KeyInput(KEY eKey, KEY_STATE eKeyState, _float fTimeDelta) override;
private:


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END