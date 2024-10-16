#include "MeshCollider.h"
#include "Mesh.h"
#include "GameObject.h"
#include "DirectXCollision.h"
CMeshCollider::CMeshCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCollider(pDevice, pContext)
{
}

HRESULT CMeshCollider::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CMeshCollider::Initialize(void* pArg)
{
	MESH_COLLIDER_DESC* pDesc = static_cast<MESH_COLLIDER_DESC*>(pArg);
	if (nullptr == pDesc)
		return E_FAIL;

	auto pVB = pDesc->pMesh->Get_VertexBuffer();
	auto pIB = pDesc->pMesh->Get_IndexBuffer();
	m_iNumVertices = pDesc->pMesh->Get_VertexCount();
	m_iNumIndexes = pDesc->pMesh->Get_IndexCount();
	_uint iVertexStride = sizeof(VTXMESH);

	D3D11_BUFFER_DESC stagingBufferDesc = {};
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.ByteWidth = iVertexStride * m_iNumVertices;
	stagingBufferDesc.BindFlags = 0;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingBufferDesc.MiscFlags = 0;
	m_pDevice->CreateBuffer(&stagingBufferDesc, nullptr, &m_pStagingVB);
	m_pContext->CopyResource(m_pStagingVB, pVB);

	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.ByteWidth = sizeof(_uint) * m_iNumIndexes;
	stagingBufferDesc.BindFlags = 0;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingBufferDesc.MiscFlags = 0;
	m_pDevice->CreateBuffer(&stagingBufferDesc, nullptr, &m_pStagingIB);
	m_pContext->CopyResource(m_pStagingIB, pIB);

	return S_OK;
}

bool CMeshCollider::Check_Collision(CCollider* pOther, RaycastHit* pOut)
{
	//TODO: ������ ��ƴ�..
	return false;
}

bool CMeshCollider::Check_Collision(const Ray& tRay, RaycastHit* pOut)
{
	
	D3D11_MAPPED_SUBRESOURCE VB ;
	HRESULT hr = m_pContext->Map(m_pStagingVB, 0, D3D11_MAP_READ, 0, &VB);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map vertex buffer." << std::endl;
		return false;
	}
	D3D11_MAPPED_SUBRESOURCE IB;
	hr = m_pContext->Map(m_pStagingIB, 0, D3D11_MAP_READ, 0, &IB);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map Index buffer." << std::endl;
		return false;
	}
	VTXMESH* vertices = static_cast<VTXMESH*>(VB.pData);
	_uint* indices = static_cast<_uint*>(IB.pData);

	float fMinDist = 9999;
	bool bIsHit = false;
	XMMATRIX matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
	XMMATRIX matInverseWorld = XMMatrixInverse(nullptr, matWorld);
	XMVECTOR vLocalOrigin = XMVector4Transform(XMLoadFloat4(&tRay.vOrigin), matInverseWorld);
	XMVECTOR vLocalDirection = XMVector4Transform(XMLoadFloat4(&tRay.vDirection), matInverseWorld);
	for (size_t indexCount = 0; indexCount < m_iNumIndexes; )
	{
		XMVECTOR v0 =XMLoadFloat3(&vertices[indices[indexCount++]].vPosition);
		XMVECTOR v1 = XMLoadFloat3(&vertices[indices[indexCount++]].vPosition);
		XMVECTOR v2 =XMLoadFloat3(&vertices[indices[indexCount++]].vPosition);
		float fDist;
		if(TriangleTests::Intersects(vLocalOrigin, vLocalDirection,v0, v1, v2, fDist))
		{

			if (fDist < (tRay.fDist < 0 ? 9999 : tRay.fDist))
			{
				bIsHit = true;
				if (fDist < fMinDist)
				{
					fMinDist = fDist;

				}
			}
		}
	}

	if (bIsHit)
	{
		pOut->pCollider = this;
		pOut->fDist = fMinDist;
		XMStoreFloat4(&(pOut->vPoint), XMLoadFloat4(&(tRay.vOrigin)) + XMVector3Normalize(XMLoadFloat4(&(tRay.vDirection))) * fMinDist);
	}
	// ���� ����
	m_pContext->Unmap(m_pStagingVB, 0);
	m_pContext->Unmap(m_pStagingIB, 0);

	return bIsHit;
}
CMeshCollider* CMeshCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMeshCollider* pInstance = new CMeshCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


CComponent* CMeshCollider::Clone(void* pArg)
{
	CMeshCollider* pInstance = new CMeshCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMeshCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CMeshCollider::Free()
{
	__super::Free();
	Safe_Release(m_pStagingVB);
	Safe_Release(m_pStagingIB);
}