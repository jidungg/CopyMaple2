#include "stdafx.h"
#include "EffConcreteController.h"
#include "EffMaterialProperty.h"
#include "EffTexturingProperty.h"
#include "EffConcreteController.h"
#include "EffBone.h"
#include "EffModel.h"
#include "Engine_Utility.h"
#include "GameInstance.h"


#pragma region ALPHA

CEffAlphaController::CEffAlphaController()
	:CEffController()
{
}

HRESULT CEffAlphaController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = EFF_CTRL_TYPE::CT_ALPHA;
	if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
		return E_FAIL;
	_uint iNumFrame = 0;
	inFile.read(reinterpret_cast<char*>(&iNumFrame), sizeof(_uint));
	for (_uint i = 0; i < iNumFrame; i++)
	{
		FLOAT_KEYFRAME tNewFrame;
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fValue), sizeof(_float));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fTrackPosition), sizeof(_float));
		//cout << "fValue" << tNewFrame.fValue << ", TrackPos" << tNewFrame.fTrackPosition << endl;
		m_vecKeyFrame.push_back(tNewFrame);
	}
	return S_OK;
}



CEffAlphaController* CEffAlphaController::Create(ifstream& inFile, const CEffModel* pModel)
{
	CEffAlphaController* pInstance = new CEffAlphaController();
	if (FAILED(pInstance->Initialize_Prototype(inFile, pModel)))
	{
		MSG_BOX("Failed to Create : CEffAlphaController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffAlphaController* CEffAlphaController::Clone()
{
	return new CEffAlphaController(*this);
}

_bool CEffAlphaController::Update_InTime(_float fTrackPos)
{
	assert(m_pTarget);

	CEffMaterialProperty* pTarget = static_cast<CEffMaterialProperty*>(m_pTarget);
	//GetCurrentKeyFrame : fTrackPos를 넘지 않는 가장 가까운 KeyFrame
	_uint iCurrentKeyFrameIndex = 0;
	_uint iNumFrame = m_vecKeyFrame.size();
	if (iNumFrame == 1)
	{
		pTarget->Set_Alpha(m_vecKeyFrame[0].fValue);
		return true;
	}
	//마지막 프레임보다 많이 갔으면? 
	if (m_vecKeyFrame[iNumFrame - 1].fTrackPosition < fTrackPos)
	{
		pTarget->Set_Alpha(m_vecKeyFrame[iNumFrame - 1].fValue);
		return true;
	}
	while (m_vecKeyFrame[iCurrentKeyFrameIndex + 1].fTrackPosition < fTrackPos)
	{
		iCurrentKeyFrameIndex++;

	}

	FLOAT_KEYFRAME tLeftKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex];
	FLOAT_KEYFRAME tRightKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex + 1];


	_float fRatio = (fTrackPos - tLeftKeyFrame.fTrackPosition)
		/ (tRightKeyFrame.fTrackPosition - tLeftKeyFrame.fTrackPosition);

	_float fAlpha = (tRightKeyFrame.fValue - tLeftKeyFrame.fValue) * fRatio + tLeftKeyFrame.fValue;

	pTarget->Set_Alpha(fAlpha);
	return false;
}

void CEffAlphaController::Set_Target(vector<CEffMaterialProperty*>& vecMaterial)
{
	m_pTarget = vecMaterial[m_iTargetIndex];
}

#pragma endregion

#pragma region TEXTURETRANSFORM

CEffTextureTransfromController::CEffTextureTransfromController()
	: CEffController()
{
}

HRESULT CEffTextureTransfromController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = EFF_CTRL_TYPE::CT_TEXTURE_TRANSFORM;
	if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
		return E_FAIL;
	inFile.read(reinterpret_cast<char*>(&m_eTexSlot), sizeof(_uint));
	inFile.read(reinterpret_cast<char*>(&m_eTexOperation), sizeof(_uint));
	_uint iNumFrame = 0;
	inFile.read(reinterpret_cast<char*>(&iNumFrame), sizeof(_uint));
	for (_uint i = 0; i < iNumFrame; i++)
	{
		FLOAT_KEYFRAME tNewFrame;
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fValue), sizeof(_float));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fTrackPosition), sizeof(_float));
		m_vecKeyFrame.push_back(tNewFrame);
	}
	return S_OK;
}


CEffTextureTransfromController* CEffTextureTransfromController::Create(ifstream& inFile, const CEffModel* pModel)
  {
	CEffTextureTransfromController* pInstance = new CEffTextureTransfromController();
	if (FAILED(pInstance->Initialize_Prototype(inFile, pModel)))
	{
		MSG_BOX("Failed to Create : CEffTextureTransfromController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffTextureTransfromController* CEffTextureTransfromController::Clone()
{
	return new CEffTextureTransfromController(*this);
}

_bool CEffTextureTransfromController::Update_InTime(_float fTrackPos)
{
	assert(m_pTarget);
	//if (m_eTexSlot != EFF_TEX_TYPE::TT_BASE)
	//	return;
	CEffTexturingProperty* pTarget = static_cast<CEffTexturingProperty*>(m_pTarget);
	//GetCurrentKeyFrame : fTrackPos를 넘지 않는 가장 가까운 KeyFrame
	_uint iCurrentKeyFrameIndex = 0;
	_uint iNumFrame = m_vecKeyFrame.size();
	if (iNumFrame == 1)
	{
		pTarget->Set_TextureTransformData(m_eTexSlot,m_eTexOperation, m_vecKeyFrame[0].fValue);
		return true;
	}
	if (m_vecKeyFrame[iNumFrame - 1].fTrackPosition < fTrackPos)
	{
		pTarget->Set_TextureTransformData(m_eTexSlot, m_eTexOperation, m_vecKeyFrame[iNumFrame - 1].fValue);
		return true;
	}
	while (m_vecKeyFrame[iCurrentKeyFrameIndex + 1].fTrackPosition < fTrackPos)
	{
		iCurrentKeyFrameIndex++;
		//마지막 프레임보다 많이 갔으면? 뭔가 이상함

	}

	FLOAT_KEYFRAME tLeftKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex];
	FLOAT_KEYFRAME tRightKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex + 1];


	_float fRatio = (fTrackPos - tLeftKeyFrame.fTrackPosition)
		/ (tRightKeyFrame.fTrackPosition - tLeftKeyFrame.fTrackPosition);

	_float fValue = (tRightKeyFrame.fValue - tLeftKeyFrame.fValue) * fRatio + tLeftKeyFrame.fValue;
	pTarget->Set_TextureTransformData(m_eTexSlot, m_eTexOperation, fValue);
	return false;
}

void CEffTextureTransfromController::Set_Target(vector<CEffTexturingProperty*>& vecTexturing)
{
	m_pTarget = vecTexturing[m_iTargetIndex];
}


#pragma endregion

#pragma region TRANSFORM

CEffTransformController::CEffTransformController()
	: CEffController()
	, m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CEffTransformController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = EFF_CTRL_TYPE::CT_TRANSFORM;
	if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
		return E_FAIL;
	_uint iNumFrame = 0;
	inFile.read(reinterpret_cast<char*>(&iNumFrame), sizeof(_uint));
	for (_uint i = 0; i < iNumFrame; i++)
	{
		TRANSFORM_KEYFRAME tNewFrame;
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vScale), sizeof(_float3));
		tNewFrame.vScale.x = -tNewFrame.vScale.x;
		//_float3 vRotation;
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vRotation), sizeof(_float3));
		tNewFrame.vRotation.x = tNewFrame.vRotation.x ;
		tNewFrame.vRotation.y = tNewFrame.vRotation.y;
		tNewFrame.vRotation.z =-tNewFrame.vRotation.z;
		//XMStoreFloat4( &tNewFrame.vRotation , XMQuaternionRotationRollPitchYaw(vRotation.x, vRotation.z, vRotation.y));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vPosition), sizeof(_float3));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fTrackPosition), sizeof(_float));
		m_vecKeyFrame.push_back(tNewFrame);
	}
	inFile.read(reinterpret_cast<char*>(&m_sFlags), sizeof(_ushort));

	return S_OK;
}


_bool CEffTransformController::Update_InTime(_float fTrackPos)
{
	assert(m_pTarget);

	CEffBone* pTarget = static_cast<CEffBone*>(m_pTarget);
	//GetCurrentKeyFrame : fTrackPos를 넘지 않는 가장 가까운 KeyFrame
	_uint iCurrentKeyFrameIndex = 0;
	_uint iNumFrame = m_vecKeyFrame.size();
	if (iNumFrame == 0)
		return true;
	if (iNumFrame == 1)
	{
		TRANSFORM_KEYFRAME tKeyFrame = m_vecKeyFrame[0];
		_vector vQuaternion = XMQuaternionRotationRollPitchYaw(tKeyFrame.vRotation.x, tKeyFrame.vRotation.y, tKeyFrame.vRotation.z);
		vQuaternion.m128_f32[2] = -vQuaternion.m128_f32[2];
		vQuaternion.m128_f32[3] = -vQuaternion.m128_f32[3];
		(pTarget)->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&tKeyFrame.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), vQuaternion, XMVectorSetW(XMLoadFloat3(&tKeyFrame.vPosition), 1.f)));
		return true;
	}
	if (m_vecKeyFrame[iNumFrame - 1].fTrackPosition < fTrackPos)
	{
		TRANSFORM_KEYFRAME tKeyFrame = m_vecKeyFrame[iNumFrame - 1];
		_vector vQuaternion = XMQuaternionRotationRollPitchYaw(tKeyFrame.vRotation.x, tKeyFrame.vRotation.y, tKeyFrame.vRotation.z);
		vQuaternion.m128_f32[2] = -vQuaternion.m128_f32[2];
		vQuaternion.m128_f32[3] = -vQuaternion.m128_f32[3];
		pTarget->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&tKeyFrame.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), vQuaternion, XMVectorSetW(XMLoadFloat3(&tKeyFrame.vPosition), 1.f)));
		return true;
	}
	while (m_vecKeyFrame[iCurrentKeyFrameIndex + 1].fTrackPosition < fTrackPos)
	{
		iCurrentKeyFrameIndex++;
	}

	TRANSFORM_KEYFRAME tLeftKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex];
	TRANSFORM_KEYFRAME tRightKeyFrame = m_vecKeyFrame[iCurrentKeyFrameIndex + 1];


	_float fRatio = (fTrackPos - tLeftKeyFrame.fTrackPosition)
		/ (tRightKeyFrame.fTrackPosition - tLeftKeyFrame.fTrackPosition);
	TRANSFORM_KEYFRAME tKeyFrame = CEngineUtility::Lerp_NonQuaternionFrame(tLeftKeyFrame, tRightKeyFrame, fRatio);

	_vector vQuaternion = XMQuaternionRotationRollPitchYaw(tKeyFrame.vRotation.x, tKeyFrame.vRotation.y, tKeyFrame.vRotation.z);

	pTarget->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&tKeyFrame.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), vQuaternion, XMVectorSetW(XMLoadFloat3(&tKeyFrame.vPosition), 1.f)));
	return false;
}

void CEffTransformController::Set_Target(vector<CEffBone*>& vecBone)
{
	m_pTarget = vecBone[m_iTargetIndex];
}

CEffTransformController* CEffTransformController::Create(ifstream& inFile, const CEffModel* pModel)
{
	CEffTransformController* pInstance = new CEffTransformController();
	if (FAILED(pInstance->Initialize_Prototype(inFile, pModel)))
	{
		MSG_BOX("Failed to Create : CEffTransformController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffTransformController* CEffTransformController::Clone()
{
	return new CEffTransformController(*this);
}

#pragma endregion

#pragma region MATERIALCOLOR

CEffMaterialColorController::CEffMaterialColorController()
	: CEffController()
{
}

HRESULT CEffMaterialColorController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = EFF_CTRL_TYPE::CT_MATERIAL_COLOR;
	if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
		return E_FAIL;
	return S_OK;
}
_bool CEffMaterialColorController::Update_InTime(_float fTrackPos)
{
	assert(m_pTarget);
	return true;
}

void CEffMaterialColorController::Set_Target(vector<CEffMaterialProperty*>& vecMaterial)
{
	m_pTarget = vecMaterial[m_iTargetIndex];
}


CEffMaterialColorController* CEffMaterialColorController::Create(ifstream& inFile, const CEffModel* pModel)
{
	CEffMaterialColorController* pInstance = new CEffMaterialColorController();
	if (FAILED(pInstance->Initialize_Prototype(inFile, pModel)))
	{
		MSG_BOX("Failed to Create : CEffMaterialColorController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffMaterialColorController* CEffMaterialColorController::Clone()
{
	return new CEffMaterialColorController(*this);
}



#pragma endregion
