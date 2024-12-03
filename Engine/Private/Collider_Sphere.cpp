#include "Collider_Sphere.h"
#include "GameInstance.h"
#include "DebugDraw.h"

CCollider_Sphere::CCollider_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CColliderBase(pDevice, pContext)
{
	m_eType = (COLLIDER_TYPE::SPHERE);
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& Prototype)
	:CColliderBase(Prototype)
{
	m_eType = (COLLIDER_TYPE::SPHERE);
}

HRESULT CCollider_Sphere::Initialize_Prototype()
{
    return __super::Initialize_Prototype();
}

HRESULT CCollider_Sphere::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	const SPHERE_COLLIDER_DESC* pDesc = static_cast<const SPHERE_COLLIDER_DESC*>(pArg);

	m_pOriginalBoundDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
	m_pBoundDesc = new BoundingSphere(*m_pOriginalBoundDesc);
    return S_OK;
}

void CCollider_Sphere::Update(_fmatrix WorldMatrix)
{
	m_pOriginalBoundDesc->Transform(*m_pBoundDesc, WorldMatrix);
}


_bool CCollider_Sphere::Intersects(CColliderBase* pOther)
{
	m_bCollide = false;
	CColliderBase::COLLIDER_TYPE eType = pOther->Get_ColliderType();
	switch (eType)
	{
		//TODO :AABB OBB MESH
	case COLLIDER_TYPE::SPHERE:
		m_bCollide = m_pBoundDesc->Intersects(*static_cast<CCollider_Sphere*>(pOther)->Get_Desc());
		break;
	default:
		assert(false);
	}

	return m_bCollide;
}

_bool CCollider_Sphere::Contains(const FXMVECTOR& vPosition)
{
	return m_pBoundDesc->Contains(vPosition);
}


_bool CCollider_Sphere::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	_bool bRet = m_pBoundDesc->Intersects(tRay.vOrigin, tRay.vDirection, pOut->fDist);
	if (bRet)
	{
		pOut->vPoint = tRay.vOrigin + tRay.vDirection * pOut->fDist;
		pOut->vNormal = XMVector3Normalize(pOut->vPoint - XMLoadFloat3( &m_pBoundDesc->Center));
		pOut->pCollider = this;
	}
	return bRet;
}

HRESULT CCollider_Sphere::Render()
{
#ifdef _DEBUG
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pBoundDesc, XMVectorSet(0.f, 1.f, 0.f, 1.f));

	m_pBatch->End();

	return S_OK;
#endif
}



CCollider_Sphere* CCollider_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider_Sphere* pInstance = new CCollider_Sphere(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCollider_Sphere::Clone(void* pArg)
{
	CCollider_Sphere* pInstance = new CCollider_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CCollider_Sphere::Free()
{
    __super::Free();
	Safe_Delete(m_pOriginalBoundDesc);
	Safe_Delete(m_pBoundDesc);

}

