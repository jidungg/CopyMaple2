#include "stdafx.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModelObject.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "EffectManager.h"
#include "UIDamgCount.h"

CDamgEvent::CDamgEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect)
	: m_pAttacker(pAttacker)
	, m_pVictim(pVictim)
	, m_iDamage(iDamage)
	, m_eHitEffect(eHitEffect)
	,m_bCrit(bCrit)
	,m_bPlayer(bPlayer)
{
	Safe_AddRef(m_pAttacker);
	Safe_AddRef(m_pVictim);
	m_eEventID = (_uint)EVENT_ID::HIT;
}

CDamgEvent* CDamgEvent::Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamg, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect)
{
	return new CDamgEvent(pAttacker, pVictim, iDamg, bCrit, bPlayer, eHitEffect);
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
	}
	else
	{
		_vector vPos = static_cast<CCharacter*>(m_pVictim)->Get_WorldPosition();
		vPos.m128_f32[1] += 0.01;
		pEffMgr->Play_EffectModel(m_eHitEffect, vPos);
		vPos = static_cast<CCharacter*>(m_pVictim)->Get_OverHeadPoint();
		pEffMgr->Play_RecoverCount(m_iDamage, vPos);
		static_cast<CCharacter*>(m_pVictim)->RestoreHP(m_iDamage);
	}

}

void CDamgEvent::Free()
{
	Safe_Release(m_pAttacker);
	Safe_Release(m_pVictim);
}
