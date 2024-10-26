#include "Engine_Utility.h"

KEYFRAME CEngineUtility::Lerp_Frame(const KEYFRAME& Frame1, const KEYFRAME& Frame2, _float fRatio)
{
	KEYFRAME KeyFrame = {};
	XMStoreFloat3(&KeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&Frame1.vScale), XMLoadFloat3(&Frame2.vScale), fRatio));
	XMStoreFloat4(&KeyFrame.vRotation, XMQuaternionSlerp(XMLoadFloat4(&Frame1.vRotation), XMLoadFloat4(&Frame2.vRotation), fRatio));
	XMStoreFloat3(&KeyFrame.vPosition, XMVectorLerp(XMLoadFloat3(&Frame1.vPosition), XMLoadFloat3(&Frame2.vPosition), fRatio));

	return KeyFrame;
}
