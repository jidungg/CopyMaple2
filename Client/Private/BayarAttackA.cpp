#include "stdafx.h"
#include "BayarAttackA.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Sound.h"

CBayarAttackA::CBayarAttackA()
    :CSkill()
{
}

HRESULT CBayarAttackA::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{

	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fRadius = 1.f;
	tTargetSearcherDesc.vCenter = { 0.f, 0.f, 1.f };
	m_pTargetSearcher = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);


	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonegiant_attack_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
	m_pUser->Add_Child(m_pCastEffect);
	m_pCastEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 0.1f, 0.f,1.f });
	m_pCastEffect->Get_Transform()->Scaling(1, 1, 1);

	return S_OK;
}

void CBayarAttackA::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	
}

void CBayarAttackA::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

}

void CBayarAttackA::On_SkillUsed()
{
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_01_A_0%d.wav"), 1, 3);
	pSouind->SetVolume(100);
}

void CBayarAttackA::On_CastingEnd()
{

}

void CBayarAttackA::Fire()
{
	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();
	//m_pCastEffect->Set_Transform(vPos);
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_NormalAttack_0%d.wav"), 1, 6);
	pSouind->SetVolume(100);

}

void CBayarAttackA::On_AttackEnd()
{
	m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	for (auto& pTarget : listTarget)
	{
		_float fDmg;
		_bool bCrit;
		fDmg = Calc_Damg(bCrit);
		m_pGameInstance->Push_Event(CDamgEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit,false,EFF_MODEL_ID::HIT_A));
	}

}



CBayarAttackA* CBayarAttackA::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarAttackA* pInstance = new CBayarAttackA;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBarayrAttackA Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarAttackA::Free()
{
	__super::Free();
	//Safe_Release(m_pCastEffect);
}
