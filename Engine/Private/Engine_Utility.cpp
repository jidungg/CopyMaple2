#include "Engine_Utility.h"

TRANSFORM_KEYFRAME CEngineUtility::Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio)
{
	TRANSFORM_KEYFRAME KeyFrame = {};
	XMStoreFloat3(&KeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&Frame1.vScale), XMLoadFloat3(&Frame2.vScale), fRatio));
	XMStoreFloat4(&KeyFrame.vRotation, XMQuaternionSlerp(XMLoadFloat4(&Frame1.vRotation), XMLoadFloat4(&Frame2.vRotation), fRatio));
	XMStoreFloat3(&KeyFrame.vPosition, XMVectorLerp(XMLoadFloat3(&Frame1.vPosition), XMLoadFloat3(&Frame2.vPosition), fRatio));

	return KeyFrame;
}

TRANSFORM_KEYFRAME CEngineUtility::Lerp_NonQuaternionFrame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio)
{
	TRANSFORM_KEYFRAME KeyFrame = {};

	XMStoreFloat3(&KeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&Frame1.vScale), XMLoadFloat3(&Frame2.vScale), fRatio));
	_vector vRot1 = XMVectorLerp(XMLoadFloat4(&Frame1.vRotation), XMLoadFloat4(&Frame2.vRotation), fRatio);
	XMStoreFloat4(&KeyFrame.vRotation, vRot1);
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

void CEngineUtility::Scale_Matrix(_matrix& outmatSrc, _float fX, _float fY, _float fZ)
{
	outmatSrc.r[0] = XMVector3Normalize(outmatSrc.r[0]) * fX;
	outmatSrc.r[1] = XMVector3Normalize(outmatSrc.r[1]) * fY;
	outmatSrc.r[2] = XMVector3Normalize(outmatSrc.r[2]) * fZ;
}

_vector CEngineUtility::Parabolic_Interpolation(_fvector vStart, _fvector vEnd, _float fJumpHeight, _float fArrivalTime, _float fCurrentTime, _float fGravity)
{
	_float fMaxHeight = vStart.m128_f32[1] + fJumpHeight;
	_float fTimeRatio = fCurrentTime / fArrivalTime;
	_vector vResult = {};
	vResult.m128_f32[0]= fTimeRatio *(vEnd.m128_f32[0] - vStart.m128_f32[0]);
	vResult.m128_f32[2]= fTimeRatio * (vEnd.m128_f32[2] - vStart.m128_f32[2]) ;

	_float fInitialVelocityY = fMaxHeight * 2 / fArrivalTime;
	vResult.m128_f32[1] = vStart.m128_f32[1] + fInitialVelocityY * fTimeRatio  -fGravity*fTimeRatio*fTimeRatio * 0.5f;

	return vResult;
}

std::wstring CEngineUtility::ConvertStringToWString(const std::string& str)
{
	// 변환에 필요한 널 문자를 포함한 버퍼 크기 계산
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (bufferSize == 0) {
		throw std::runtime_error("Failed to calculate buffer size.");
	}

	// 변환 수행
	std::wstring wstr(bufferSize, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], bufferSize);

	// 문자열 끝의 널 문자를 제거
	wstr.resize(wcslen(wstr.c_str()));
	return wstr;
}

string CEngineUtility::ConvertWStringToString(const wstring& wstr)
{
	// 변환에 필요한 널 문자를 포함한 버퍼 크기 계산
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (bufferSize == 0) {
		throw std::runtime_error("Failed to calculate buffer size.");
	}
	// 변환 수행
	std::string str(bufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
	// 문자열 끝의 널 문자를 제거
	str.resize(strlen(str.c_str()));
	return str;
}