#include "Collider_Frustum.h"
#include "Collider_Sphere.h"
#include "GameInstance.h"
#include "DebugDraw.h"

CCollider_Frustum::CCollider_Frustum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CColliderBase(pDevice, pContext)
{
	m_eType = (COLLIDER_TYPE::FRUSTUM);
}


CCollider_Frustum::CCollider_Frustum(const CCollider_Frustum& Prototype)
	: CColliderBase(Prototype)
{
	m_eType = (COLLIDER_TYPE::FRUSTUM);
}

HRESULT CCollider_Frustum::Initialize_Prototype()
{
	return __super::Initialize_Prototype();
}

HRESULT CCollider_Frustum::Initialize(void* pArg)
{
	const FRUSTUM_COLLIDER_DESC* pDesc = static_cast<const FRUSTUM_COLLIDER_DESC*>(pArg);
	_float fAspect = pDesc->fWidth / pDesc->fHeight;
	_float fFovY = 2*atan(tan(pDesc->fWidth/fAspect/pDesc->fDistance));
	
	//XMMatrixPerspectiveLH(pDesc->fWidth, pDesc->fHeight, 0,pDesc->fDistance);
	//XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar)
	auto matrix = XMMatrixPerspectiveFovLH(fFovY, fAspect, 1, pDesc->fDistance);
	m_pOriginalBoundDesc = new BoundingFrustum(matrix);
	m_pBoundDesc = new BoundingFrustum(*m_pOriginalBoundDesc);
	return S_OK;
}

void CCollider_Frustum::Update(_fmatrix WorldMatrix)
{
	m_pOriginalBoundDesc->Transform(*m_pBoundDesc, WorldMatrix);
}

HRESULT CCollider_Frustum::Render()
{
#ifdef _DEBUG
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pBoundDesc, XMVectorSet(1.f, 0.f, 0.f, 1.f));

	m_pBatch->End();

	return S_OK;
#endif
}

_bool CCollider_Frustum::Intersects(CColliderBase* pOther)
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

_bool CCollider_Frustum::Contains(const FXMVECTOR& vPos)
{
    return m_pBoundDesc->Contains(vPos);
}

_bool CCollider_Frustum::RayCast(const Ray& tRay, RaycastHit* pOut)
{
    return m_pBoundDesc->Intersects(tRay.vOrigin,tRay.vDirection, pOut->fDist);
}
CCollider_Frustum* CCollider_Frustum::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider_Frustum* pInstance = new CCollider_Frustum(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CCollider_Frustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Frustum::Clone(void* pArg)
{
	CCollider_Frustum* pInstance = new CCollider_Frustum(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_Frustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Frustum::Free()
{
	__super::Free();
	Safe_Delete(m_pOriginalBoundDesc);
	Safe_Delete(m_pBoundDesc);
}

