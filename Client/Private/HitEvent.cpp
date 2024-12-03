#include "stdafx.h"
#include "Character.h"
#include "HitEvent.h"

CHitEvent::CHitEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, const _char* szEffectTag)
	: m_pAttacker(pAttacker)
	, m_pVictim(pVictim)
	, m_iDamage(iDamage)
{
	Safe_AddRef(m_pAttacker);
	Safe_AddRef(m_pVictim);
	m_eEventID = (_uint)EVENT_ID::HIT;
}

CHitEvent* CHitEvent::Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, const _char* szEffectTag)
{
	return new CHitEvent(pAttacker, pVictim, iDamage,szEffectTag);
}

void CHitEvent::Exec()
{
	m_pVictim->Hit(m_pAttacker,m_iDamage);
}

void CHitEvent::Free()
{
	Safe_Release(m_pAttacker);
	Safe_Release(m_pVictim);
}
