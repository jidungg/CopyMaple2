#pragma once
#include "Base.h"

BEGIN(Engine)

class CEngineUtility :
    public CBase
{
public:
    static KEYFRAME Lerp_Frame(const KEYFRAME& Frame1, const KEYFRAME& Frame2, _float fRatio);
};

END