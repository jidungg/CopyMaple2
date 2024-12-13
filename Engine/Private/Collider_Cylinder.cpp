#include "Collider_Cylinder.h"

CCollider_Cylinder::CCollider_Cylinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CColliderBase(pDevice, pContext)
{
	m_eType = (COLLIDER_TYPE::CYLINDER);
}

CCollider_Cylinder::CCollider_Cylinder(const CCollider_Cylinder& Prototype)
	:CColliderBase(Prototype)
	, m_fHeight{ Prototype.m_fHeight }
	, m_fRadius{ Prototype.m_fRadius }
	, m_vWorldCenter{ Prototype.m_vWorldCenter }
{
	m_eType = (COLLIDER_TYPE::CYLINDER);
}


HRESULT CCollider_Cylinder::Initialize_Prototype()
{
	return __super::Initialize_Prototype();
}

HRESULT CCollider_Cylinder::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	const CYLINDER_COLLIDER_DESC* pDesc = static_cast<const CYLINDER_COLLIDER_DESC*>(pArg);
	m_fHeight = pDesc->fHeight;
	m_fRadius = pDesc->fRadius;

	return S_OK;
}

void CCollider_Cylinder::Update(_fmatrix WorldMatrix)
{
	m_vWorldCenter = XMVector3Transform(XMLoadFloat3(&m_vCenterOffset), WorldMatrix);
}

_bool CCollider_Cylinder::Intersects(CColliderBase* pOther)
{
	_vector vPos = XMLoadFloat3( &pOther->Get_CenterOffset());
	return Contains(vPos);
}

_bool CCollider_Cylinder::Contains(const FXMVECTOR& vPosition)
{
	m_bCollide = false;
	_vector vXZPos = XMVectorSet(XMVectorGetX(vPosition), 0, XMVectorGetZ(vPosition), 1);
	_vector vXZCenter = XMVectorSet(XMVectorGetX(m_vWorldCenter), 0, XMVectorGetZ(m_vWorldCenter), 1);
	if (XMVector3Length(vXZPos - vXZCenter).m128_f32[0] < m_fRadius)
	{
		_float fY = XMVectorGetY(vPosition);
		_float fYCenter = XMVectorGetY(m_vWorldCenter);
		if (fY <= fYCenter + m_fHeight / 2 && fY >= fYCenter - m_fHeight / 2)
			m_bCollide = true;
	}
	return m_bCollide;
}

_bool CCollider_Cylinder::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	return false;
}

HRESULT CCollider_Cylinder::Render()
{
	return S_OK;
}

CCollider_Cylinder* CCollider_Cylinder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider_Cylinder* pInstance = new CCollider_Cylinder(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_Cylinder");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCollider_Cylinder::Clone(void* pArg)
{
	CCollider_Cylinder* pInstance = new CCollider_Cylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider_Cylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Cylinder::Free()
{
	__super::Free();
}
