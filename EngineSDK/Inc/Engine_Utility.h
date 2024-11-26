#pragma once
#include "Base.h"

BEGIN(Engine)

class CEngineUtility :
    public CBase
{
public:
    static TRANSFORM_KEYFRAME Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio);
	static _float Get_RandomFloat(_float fMin, _float fMax);
};

END