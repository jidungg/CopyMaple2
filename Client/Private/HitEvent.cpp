#include "stdafx.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModelObject.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "EffectManager.h"

CHitEvent::CHitEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, EFF_MODEL_ID eHitEffect)
	: m_pAttacker(pAttacker)
	, m_pVictim(pVictim)
	, m_iDamage(iDamage)
	, m_eHitEffect(eHitEffect)
{
	Safe_AddRef(m_pAttacker);
	Safe_AddRef(m_pVictim);
	m_eEventID = (_uint)EVENT_ID::HIT;
}

CHitEvent* CHitEvent::Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, EFF_MODEL_ID eHitEffect)
{
	return new CHitEvent(pAttacker, pVictim, iDamage, eHitEffect);
}

void CHitEvent::Exec()
{
	m_pVictim->Hit(m_pAttacker,m_iDamage);
	EFFECT_MANAGER->Play_EffectModel(m_eHitEffect,static_cast<CCharacter*>( m_pVictim)->Get_Hitpoint());
}

void CHitEvent::Free()
{
	Safe_Release(m_pAttacker);
	Safe_Release(m_pVictim);
}
