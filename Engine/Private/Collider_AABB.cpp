#include "Collider_AABB.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "DebugDraw.h"


CCollider_AABB::CCollider_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CColliderBase(pDevice, pContext)
{
	m_eType = (COLLIDER_TYPE::AABB);
}

CCollider_AABB::CCollider_AABB(const CCollider_AABB& Prototype)
	:CColliderBase(Prototype)
{
	m_eType = (COLLIDER_TYPE::AABB);
}

HRESULT CCollider_AABB::Initialize_Prototype()
{
	return __super::Initialize_Prototype();
}

HRESULT CCollider_AABB::Initialize(void* pArg)
{
	const AABB_COLLIDER_DESC* pDesc = static_cast<const AABB_COLLIDER_DESC*>(pArg);

	m_pOriginalBoundDesc = new BoundingBox(pDesc->vCenter, pDesc->vExtentes);
	m_pBoundDesc = new BoundingBox(*m_pOriginalBoundDesc);
	return S_OK;
}
void CCollider_AABB::Update(_fmatrix WorldMatrix)
{
	m_pOriginalBoundDesc->Transform(*m_pBoundDesc, WorldMatrix);
}


_bool CCollider_AABB::Intersects(CColliderBase* pOther)
{
	m_bCollide = false;
	CColliderBase::COLLIDER_TYPE eType = pOther->Get_ColliderType();
	switch (eType)
	{
		//TODO :AABB OBB MESH
	case COLLIDER_TYPE::SPHERE:
		m_bCollide = m_pBoundDesc->Intersects(*static_cast<CCollider_Sphere*>(pOther)->Get_Desc());
		break;
	case COLLIDER_TYPE::AABB:
		m_bCollide = m_pBoundDesc->Intersects(*static_cast<CCollider_AABB*>(pOther)->m_pBoundDesc);
		break;
	default:
		assert(false);
	}

	return m_bCollide;
}

_bool CCollider_AABB::Intersects(CColliderBase* pOther, _vector* vOutNormal, _float* fOutDepth)
{
	return _bool();
}

_bool CCollider_AABB::Contains(const FXMVECTOR& vPosition)
{
	return m_pBoundDesc->Contains(vPosition);

}

_bool CCollider_AABB::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	_float fDist = FLT_MAX;
	_bool bRet = m_pBoundDesc->Intersects(tRay.vOrigin, tRay.vDirection, fDist);
	if(bRet)
	{
		if (tRay.fDist < fDist)
			return false;
		pOut->fDist = fDist;
		pOut->vPoint = tRay.vOrigin + tRay.vDirection * pOut->fDist;
		pOut->vNormal = pOut->vPoint - XMLoadFloat3(& m_pBoundDesc->Center);
		float x = XMVectorGetX(pOut->vNormal);
		float y = XMVectorGetY(pOut->vNormal);
		float z = XMVectorGetZ(pOut->vNormal);
		float fMax = max(max(abs(x), abs(y)), abs(z));
		pOut->vNormal = XMVectorSet(_int(x / fMax), _int(y / fMax), _int(z / fMax), 0.f);
		pOut->vNormal = XMVector4Normalize(pOut->vNormal);
		pOut->pCollider = this;
	}

	return bRet;
}

HRESULT CCollider_AABB::Render()
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



CCollider_AABB* CCollider_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider_AABB* pInstance = new CCollider_AABB(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CCollider_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_AABB::Clone(void* pArg)
{
	CCollider_AABB* pInstance = new CCollider_AABB(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_AABB::Free()
{
	__super::Free();
	Safe_Delete(m_pOriginalBoundDesc);
	Safe_Delete(m_pBoundDesc);
}

