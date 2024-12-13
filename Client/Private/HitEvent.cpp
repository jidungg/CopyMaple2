#include "stdafx.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModelObject.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "EffectManager.h"
#include "UIDamgCount.h"

CHitEvent::CHitEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect)
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

CHitEvent* CHitEvent::Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamg, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect)
{
	return new CHitEvent(pAttacker, pVictim, iDamg, bCrit, bPlayer, eHitEffect);
}

void CHitEvent::Exec()
{
	CEffectManager* pEffMgr = EFFECT_MANAGER;
	_vector vPos = static_cast<CCharacter*>(m_pVictim)->Get_Hitpoint();
	m_pVictim->Hit(m_pAttacker,m_iDamage);
	pEffMgr->Play_EffectModel(m_eHitEffect,static_cast<CCharacter*>( m_pVictim)->Get_WorldPosition());
	pEffMgr->Play_DamgCount(m_bCrit,m_bPlayer, m_iDamage, vPos);
}

void CHitEvent::Free()
{
	Safe_Release(m_pAttacker);
	Safe_Release(m_pVictim);
}
