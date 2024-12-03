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
	explicit CHitEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, const _char* szEffectTag);
	virtual ~CHitEvent() {}

public:
	static CHitEvent* Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, const _char* szEffectTag);
	void Exec() override;

private:
	CGameObject* m_pAttacker = { nullptr };
	CGameObject* m_pVictim = { nullptr };
	_int m_iDamage = { 0 };
	const _char* m_szEffectTag = { "" } ;
private:
	void Free() override;

};

END