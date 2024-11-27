#pragma once
#include "Base.h"

BEGIN(Client)
class IEffControllable
{
public:
	virtual void Reset() abstract;
};

END