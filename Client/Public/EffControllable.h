#pragma once
#include "Base.h"

BEGIN(Client)
class IEffControllable
{
public:
	virtual void Update_Value(KEYFRAME* pFrame) abstract;
};

END