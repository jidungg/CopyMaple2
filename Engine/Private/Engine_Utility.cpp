#include "Engine_Utility.h"

TRANSFORM_KEYFRAME CEngineUtility::Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio)
{
	TRANSFORM_KEYFRAME KeyFrame = {};
	XMStoreFloat3(&KeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&Frame1.vScale), XMLoadFloat3(&Frame2.vScale), fRatio));
	XMStoreFloat4(&KeyFrame.vRotation, XMQuaternionSlerp(XMLoadFloat4(&Frame1.vRotation), XMLoadFloat4(&Frame2.vRotation), fRatio));
	XMStoreFloat3(&KeyFrame.vPosition, XMVectorLerp(XMLoadFloat3(&Frame1.vPosition), XMLoadFloat3(&Frame2.vPosition), fRatio));

	return KeyFrame;
}

_float CEngineUtility::Hermit_Interpolation(_float fV1, _float fV2, _float fT1, _float fT2 , _float fX)
{
	_float fX2 = fX * fX;
	_float fX3 = fX2 * fX;

	return  fV1 * (2.0f * fX3 - 3.0f * fX2 + 1.0f) + fV2 * (-2.0f * fX3 + 3.0f * fX2) + fT1 * (fX3 - 2.0f * fX2 + fX) + fT2 * (fX3 - fX2);
}

_float CEngineUtility::Get_RandomFloat(_float fMin, _float fMax)
{
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 
	return fMin + random * (fMax - fMin);
}
