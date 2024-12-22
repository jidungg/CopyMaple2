#pragma once
#include "Base.h"

BEGIN(Client)
class ISlotItem
{
public:
	virtual const _char* Get_IconTag() abstract;
protected:
};

class IQuickItem : public ISlotItem
{
public:
	virtual void Use() abstract;
	virtual void Update_Cool(_float fTimeDelta)
	{
		m_fCoolTimeAcc += fTimeDelta;
	}
	virtual _float Get_CoolTimeRatio() abstract;
	virtual _int Get_StackCount()abstract;
protected:
	_float m_fCoolTimeAcc = { 0.f };
};
END