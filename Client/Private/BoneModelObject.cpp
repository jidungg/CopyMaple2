#include "stdafx.h"
#include "BoneModelObject.h"

CBoneModelObject::CBoneModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CBoneModelObject::CBoneModelObject(const CBoneModelObject& Prototype)
	: CModelObject(Prototype)
	, m_pSocketMatrix(Prototype.m_pSocketMatrix)
{
}

HRESULT CBoneModelObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoneModelObject::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		BONEMODELOBJ_DESC* pDesc = static_cast<BONEMODELOBJ_DESC*>(pArg);
		m_pSocketMatrix = pDesc->pSocketMatrix;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CBoneModelObject::Compute_Matrix()
{
	assert(nullptr != m_pParentMatrix);
	assert(nullptr != m_pSocketMatrix);
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

}

HRESULT CBoneModelObject::Render()
{
	return __super::Render();
}

CBoneModelObject* CBoneModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoneModelObject* pInstance = new CBoneModelObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoneModelObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoneModelObject::Clone(void* pArg)
{
	CBoneModelObject* pInstance = new CBoneModelObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoneModelObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoneModelObject::Free()
{
	__super::Free();
}
