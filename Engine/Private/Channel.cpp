
#include "..\Public\Channel.h"
#include "Model.h"
#include "Bone.h"


CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(ifstream& inFile, const CModel* pModel)
{
	_uint iNameLength = 0;
	inFile.read(reinterpret_cast<char*>(&iNameLength), sizeof(_uint));
	inFile.read(m_szName, iNameLength);
	m_szName[iNameLength] = '\0';
	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);

	inFile.read(reinterpret_cast<char*>(&m_iNumKeyFrames), sizeof(_uint));

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME		KeyFrame = {};

		inFile.read(reinterpret_cast<char*>(&KeyFrame), sizeof(KEYFRAME));

		m_KeyFrames.push_back(KeyFrame);
	}
	return S_OK;
}

void CChannel::Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones)
{
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vPosition = LastKeyFrame.vPosition;
	}

	else
	{
		if (fCurrentTrackPosition == 0)
			*pKeyFrameIndex = 0;
		else if (fCurrentTrackPosition >= m_KeyFrames[*pKeyFrameIndex + 1].fTrackPosition)
			++*pKeyFrameIndex;
		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[*pKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pKeyFrameIndex].fTrackPosition);


		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = m_KeyFrames[*pKeyFrameIndex].vScale;
		vLeftRotation = m_KeyFrames[*pKeyFrameIndex].vRotation;
		vLeftPosition = m_KeyFrames[*pKeyFrameIndex].vPosition;

		vRightScale = m_KeyFrames[*pKeyFrameIndex + 1].vScale;
		vRightRotation = m_KeyFrames[*pKeyFrameIndex + 1].vRotation;
		vRightPosition = m_KeyFrames[*pKeyFrameIndex + 1].vPosition;
		 
		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
		
	}

	Bones[m_iBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)));
}

pair<_uint, KEYFRAME> CChannel::Get_KeyFrame(_float fTrackPos) const
{
	pair<_uint,KEYFRAME >	KeyFrame = {};
	for (auto& key : m_KeyFrames)
	{
		if (fTrackPos > key.fTrackPosition)
			break;
		KeyFrame = make_pair( m_iBoneIndex, key );

	}
	return KeyFrame;
}

CChannel* CChannel::Create(ifstream& inFile, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(inFile, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CChannel::Free()
{
	__super::Free();
}
