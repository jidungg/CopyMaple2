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
	virtual _float Get_CoolTimeRatio() abstract;
protected:

};
END