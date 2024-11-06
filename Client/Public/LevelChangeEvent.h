#pragma once
#include "Event.h"
BEGIN(Engine)
class CLevel;
class CGameObject;
END
BEGIN(Client)

class CLevelChangeEvent : public CEvent
{
private:
	explicit CLevelChangeEvent(CLevel* pLevel);
	virtual ~CLevelChangeEvent() {}

public:
	static CLevelChangeEvent* Create(CLevel* pLevel);
	void Exec() override;

private:
	CLevel* m_pLevel = { nullptr };
private:
	void Free() override;

};
END