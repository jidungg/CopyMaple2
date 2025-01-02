#pragma once
#include "Event.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CDamgEvent :
    public CEvent
{
private:
	explicit CDamgEvent(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffect, const wstring& strSFX);
	virtual ~CDamgEvent() {}

public:
	static CDamgEvent* Create(CGameObject* pAttacker, CGameObject* pVictim, _int iDamage, _bool bCrit, _bool bPlayer, EFF_MODEL_ID eHitEffectconst, const wstring& strSFX = TEXT("Hit_DefaultDefault_01.wav"));
	void Exec() override;

private:
	CGameObject* m_pAttacker = { nullptr };
	CGameObject* m_pVictim = { nullptr };
	_int m_iDamage = { 0 };
	EFF_MODEL_ID m_eHitEffect = { EFF_MODEL_ID::LAST };
	_bool m_bCrit, m_bPlayer;
	wstring m_strSFX;
private:
	void Free() override;

};

END