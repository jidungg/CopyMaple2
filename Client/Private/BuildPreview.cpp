#include "stdafx.h"
#include "BuildPreview.h"
#include <Client_Utility.h>

CBuildPreview::CBuildPreview(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTerrainObject(pDevice, pContext)
{
}

CBuildPreview::CBuildPreview(const CBuildPreview& Prototype)
	: CTerrainObject(Prototype)
{
}


void CBuildPreview::Compute_Matrix()
{
	_matrix pParentMatrix = XMMatrixTranslationFromVector(XMLoadFloat4x4(m_pParentMatrix).r[CTransform::STATE_POSITION]);
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * pParentMatrix);

}

CBuildPreview* CBuildPreview::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuildPreview* pInstance = new CBuildPreview(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildPreview");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CGameObject* CBuildPreview::Clone(void* pArg)
{
	CBuildPreview* pInstance = new CBuildPreview(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBuildPreview");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBuildPreview::Free()
{
	__super::Free();
}
