#include "stdafx.h"
#include "BayarKnockOff.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bayar.h"
#include "Sound.h"

CBayarKnockOff::CBayarKnockOff()
	:CSkill()
{
}

HRESULT CBayarKnockOff::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//TargetSearcher
	CCollider_Sphere::SPHERE_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fRadius = 0.7f;
	tTargetSearcherDesc.vCenter = { 0.f, 1.f, 0.f };
	m_pTargetSearcher = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);


	//Jump Ready Effect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_jump_ready_a.effmodel");
	m_pJumpReadyEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pJumpReadyEffect->Set_Active(false);
	m_pUser->Add_Child(m_pJumpReadyEffect);

	//Gathering Effect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_01_d_b.effmodel");
	m_pGatheringEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pGatheringEffect->Set_Active(false);
	m_pUser->Add_Child(m_pGatheringEffect);

	return S_OK;
}

void CBayarKnockOff::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_fTimeAcc += fTimeDelta;
}

void CBayarKnockOff::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_pTargetSearcher->Is_Active())
	{
		if (m_fTimeAcc >= m_fDamgInterval)
		{
			m_fTimeAcc = 0;

			list<CGameObject*> listTarget;
			SearchTarget(&listTarget, LAYER_PLAYER);
			m_pTargetSearcher->Render();
			for (auto& pTarget : listTarget)
			{
				CPlayer* pTargetPlayer = static_cast<CPlayer*>(pTarget);
				if (static_cast<CBayar*>(m_pUser)->Is_Attached(pTargetPlayer))
					pTargetPlayer->Detach_From();
				_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
				_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
				fDmg = m_pUser->Get_Stat().iATK * fDmg *0.3* 0.01;
				_bool bCrit = m_pUser->Judge_Critical();
				fDmg *= bCrit ? 1.5 : 1.f;
				m_pGameInstance->Push_Event(CDamgEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit, false, EFF_MODEL_ID::HIT_A ));
			}
		}
	}

}



void CBayarKnockOff::On_SkillUsed()
{
	m_pJumpReadyEffect->Set_Active(true);
	m_pJumpReadyEffect->Start_Animation();
	m_pGatheringEffect->Set_Active(true);
	m_pGatheringEffect->Start_Animation(0, true);
	m_pGatheringEffect->Set_AnimSpeed(3.15f);

	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_Skill_G_0%d.wav"), 1, 2);
	pSouind->SetVolume(100);
}

void CBayarKnockOff::On_CastingEnd()
{
	m_pGatheringEffect->Set_Active(false);

	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_Skill_H_02.wav"));
	pSouind->SetVolume(100);
}

void CBayarKnockOff::Fire()
{
	m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
	m_pTargetSearcher->Set_Active(true);

}

void CBayarKnockOff::On_AttackEnd()
{
	m_pTargetSearcher->Set_Active(false);
}

CBayarKnockOff* CBayarKnockOff::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarKnockOff* pInstance = new CBayarKnockOff;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarKnockOff Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarKnockOff::Free()
{
	__super::Free();
}
