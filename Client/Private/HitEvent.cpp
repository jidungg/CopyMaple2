#include "stdafx.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModelObject.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "EffectManager.h"
#include "UIDamgCount.h"
#include "Monster.h"
#include "UIBundle.h"
#include "Sound.h"
#include "GameInstance.h"

CDamgEvent::CDamgEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect, const wstring& strSFX)
	: m_pAttacker(pAttacker)
	, m_pVictim(pVictim)
	, m_iDamage(iDamage)
	, m_eHitEffect(eHitEffect)
	,m_bCrit(bCrit)
	,m_bPlayer(bPlayer)
	, m_strSFX(strSFX)
{
	Safe_AddRef(m_pAttacker);
	Safe_AddRef(m_pVictim);
	m_eEventID = (_uint)EVENT_ID::HIT;
}

CDamgEvent* CDamgEvent::Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamg, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect, const wstring& strSFX)
{
	return new CDamgEvent(pAttacker, pVictim, iDamg, bCrit, bPlayer, eHitEffect, strSFX);
}

void CDamgEvent::Exec()
{
	CEffectManager* pEffMgr = EFFECT_MANAGER;
	if (m_iDamage >= 0)
	{
		m_pVictim->Hit(m_pAttacker,m_iDamage);
		_vector vPos = static_cast<CCharacter*>(m_pVictim)->Get_Hitpoint();
		pEffMgr->Play_EffectModel(m_eHitEffect, vPos);
		vPos = static_cast<CCharacter*>(m_pVictim)->Get_OverHeadPoint();
		pEffMgr->Play_DamgCount(m_bCrit, m_bPlayer, m_iDamage, vPos);

		CMonster* pMonster = dynamic_cast<CMonster*>(m_pVictim);
		if (pMonster)
		{
			UIBUNDLE->Set_ShowingHPMonster(pMonster);
			UIBUNDLE->Set_MonsterHPBarVisible(true);
		}
	}
	else
	{
		_vector vPos = static_cast<CCharacter*>(m_pVictim)->Get_WorldPosition();
		vPos.m128_f32[1] += 0.01;
		pEffMgr->Play_EffectModel(m_eHitEffect, vPos);
		vPos = static_cast<CCharacter*>(m_pVictim)->Get_OverHeadPoint();
		pEffMgr->Play_RecoverCount(m_iDamage, vPos);
		static_cast<CCharacter*>(m_pVictim)->RestoreHP(-m_iDamage);
	}
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_LOADING, m_strSFX);
	pSouind->SetVolume(100);

}

void CDamgEvent::Free()
{
	Safe_Release(m_pAttacker);
	Safe_Release(m_pVictim);
}
