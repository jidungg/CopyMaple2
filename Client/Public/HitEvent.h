#pragma once
#include "Event.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CHitEvent :
    public CEvent
{
private:
	explicit CHitEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, EFF_MODEL_ID eHitEffect);
	virtual ~CHitEvent() {}

public:
	static CHitEvent* Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, EFF_MODEL_ID eHitEffect);
	void Exec() override;

private:
	CGameObject* m_pAttacker = { nullptr };
	CGameObject* m_pVictim = { nullptr };
	_int m_iDamage = { 0 };
	EFF_MODEL_ID m_eHitEffect = { EFF_MODEL_ID::LAST };
private:
	void Free() override;

};

END