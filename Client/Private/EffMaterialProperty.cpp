#include "stdafx.h"
#include "EffMaterialProperty.h"

CEffMaterialProperty::CEffMaterialProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

HRESULT CEffMaterialProperty::Initialize_Prototype( ifstream& inFile)
{
	inFile.read(reinterpret_cast<char*>(&m_vAmbient), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_vDiffuse), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_vSpecular), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_vEmissive), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_fGlossiness), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fAlpha), sizeof(_float));
	return S_OK;
}

void CEffMaterialProperty::Update_Value(vector<class CEffController*>* pControllerList)
{
}

CEffMaterialProperty* CEffMaterialProperty::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& inFile)
{
	CEffMaterialProperty* pInstance = new CEffMaterialProperty(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype( inFile)))
	{
		MSG_BOX("Failed to Created : CEffMaterialProperty");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CEffMaterialProperty::Clone(void* pArg)
{
	return  new CEffMaterialProperty(*this);
}

void CEffMaterialProperty::Free()
{
	__super::Free();
}

void CEffMaterialProperty::Update_Value(KEYFRAME* pFrame)
{
}
