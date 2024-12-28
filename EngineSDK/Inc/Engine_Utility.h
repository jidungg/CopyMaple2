#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEngineUtility :
    public CBase
{
public:
    static TRANSFORM_KEYFRAME Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio);
    static _float Hermit_Interpolation(_float fV1, _float fV2, _float fT1, _float fT2, _float fX);
	static _float Get_RandomFloat(_float fMin, _float fMax);
    static void Scale_Matrix(_matrix& outmatSrc, _float fX =1, _float fY =1, _float fZ =1);
	static _vector Parabolic_Interpolation(_fvector vStart, _fvector vEnd, _float fJumpHeight, _float fArrivalTime, _float fCurrentTime,  _float fGravity = 9.8f);
    static std::wstring ConvertStringToWString(const std::string& str);
};

END