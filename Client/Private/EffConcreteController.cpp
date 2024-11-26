#include "stdafx.h"
#include "EffConcreteController.h"

CEffAlphaController::CEffAlphaController()
	:CEffController()
{
}

HRESULT CEffAlphaController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = CTRL_TYPE::CT_ALPHA;
    if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
        return E_FAIL;
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

_bool CEffAlphaController::Update_Controller(_float fCurrentTrackPos)
{
	return true;
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


CEffTextureTransfromController::CEffTextureTransfromController()
	: CEffController()
{
}

HRESULT CEffTextureTransfromController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = CTRL_TYPE::CT_TEXTURE_TRANSFORM;
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

_bool CEffTextureTransfromController::Update_Controller(_float fCurrentTrackPos)
{
	return true;
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


CEffTransformController::CEffTransformController()
	: CEffController()
{
}

HRESULT CEffTransformController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = CTRL_TYPE::CT_TRANSFORM;
    if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
        return E_FAIL;
	_uint iNumFrame = 0;
	inFile.read(reinterpret_cast<char*>(&iNumFrame), sizeof(_uint));
	for (_uint i = 0; i < iNumFrame; i++)
	{
		TRANSFORM_KEYFRAME tNewFrame;
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vScale), sizeof(_float3));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vRotation), sizeof(_float4));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.vPosition), sizeof(_float3));
		inFile.read(reinterpret_cast<char*>(&tNewFrame.fTrackPosition), sizeof(_float));
		m_vecKeyFrame.push_back(tNewFrame);
	}
    return S_OK;
}

_bool CEffTransformController::Update_Controller(_float fCurrentTrackPos)
{


	return true;
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



CEffMaterialColorController::CEffMaterialColorController()
	: CEffController()
{
}

HRESULT CEffMaterialColorController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	m_eCtrlType = CTRL_TYPE::CT_MATERIAL_COLOR;
    if (FAILED(__super::Initialize_Prototype(inFile, pModel)))
        return E_FAIL;
    return S_OK;
}

_bool CEffMaterialColorController::Update_Controller(_float fCurrentTrackPos)
{
	return true;
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


