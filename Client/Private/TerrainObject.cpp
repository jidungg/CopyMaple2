#include "stdafx.h"
#include "TerrainObject.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshCollider.h"
#include "Client_Utility.h"
#include "Character.h"
#include "Collider_AABB.h"
#include "Collider_Sphere.h"
#include "ItemDataBase.h"

CTerrainObject::CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CTerrainObject::CTerrainObject(const CTerrainObject& Prototype)
	: CModelObject(Prototype),
	m_pColliderCom{ Prototype.m_pColliderCom }
	, m_eTerrainDir{ Prototype.m_eTerrainDir }
	, m_iIndex{ Prototype.m_iIndex }
	, m_eBuildItemID{ Prototype.m_eBuildItemID }
{

	Safe_AddRef(m_pColliderCom);
}

HRESULT CTerrainObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrainObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	TERRAINOBJ_DESC* pDesc = (TERRAINOBJ_DESC*)pArg;

	pDesc->fRotationPerSec = 5.f;
	pDesc->fSpeedPerSec = 1.f;
	m_eBuildItemID = pDesc->eID;
	m_eBuildItemType= static_cast<BUILD_ITEM_DESC*>( ITEMDB->Get_Data(ITEM_TYPE::BUILD,(_uint)m_eBuildItemID))->eBuildType;
	m_eTerrainDir = pDesc->direction;
	m_iIndex = pDesc->index;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->pos.x, pDesc->pos.y, pDesc->pos.z,1));
	m_pTransformCom->LookToward(Get_Direction_Vector(m_eTerrainDir));
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CTerrainObject::Update(_float fTimeDelta)
{
	if(m_bRotating)
		m_bRotating = false == m_pTransformCom->TurnToward(Get_Direction_Vector(m_eTerrainDir), fTimeDelta);

	__super::Update(fTimeDelta);
}

HRESULT CTerrainObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	m_pColliderCom->Render();
	return S_OK;
}




json CTerrainObject::ToJson()
{
	json j;
	j["ItemId"] = m_eBuildItemID;
	j["Data"] = 0;
	j["Iteration"] = 1;
	j["Direction"] = m_eTerrainDir;
	j["Index"] = m_iIndex;
	return j;
}

void CTerrainObject::Rotate()
{
	m_eTerrainDir = (DIRECTION)(((_uint)m_eTerrainDir + 1) % (_uint)DIRECTION::XMZP);
	m_bRotating = true;
}


_vector CTerrainObject::BolckXZ(_vector vCharacterPosition, _vector vMoveDirection, _float fMoveDistance, _float fCollisionRadius, _float fCollisionHeight)
{
	m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));

	CCollider_AABB* pCollider = static_cast<CCollider_AABB*>(m_pColliderCom);
	_float3* pCorners = new _float3[8];
	pCollider->Get_Desc()->GetCorners(pCorners);
	_vector vMin = XMLoadFloat3(&pCorners[4]);
	_vector vMax = XMLoadFloat3(&pCorners[2]);
	delete[] pCorners;

	_vector vNextPosition = vCharacterPosition + vMoveDirection * fMoveDistance;
	
	_vector vClosestPoint = XMVectorClamp(vNextPosition, vMin, vMax);

	_vector vCollisionDir = vNextPosition - vClosestPoint;
	_float fDistance = XMVectorGetX(XMVector3Length(vCollisionDir));
	_float fCollisionDepth = fCollisionRadius - fDistance;


	if (fCollisionDepth > 0)
	{
		_float fMoveSlope = vMoveDirection.m128_f32[2] / vMoveDirection.m128_f32[0];
		_float fCollisionSlope = vCollisionDir.m128_f32[2] / vCollisionDir.m128_f32[0];
		_vector vCollisionNormal = XMVector3Normalize(vCollisionDir);
		//왼쪽
		if (vMin.m128_f32[0] > vCharacterPosition.m128_f32[0])
		{
			//LB : 원 중심에서 충돌점 까지의 벡터보다 진행방향 기울기가 작으면 윗면충돌
			if (vMin.m128_f32[2] > vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(-1, 0, 0, 0) : XMVectorSet(0, 0, -1, 0);
			}
			//LT
			else if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(0, 0, 1, 0) : XMVectorSet(-1, 0, 0, 0);
			}
			//L
			else
			{
				vCollisionNormal = XMVectorSet(-1, 0, 0, 0);
			}
		}
		//오른쪽
		else if (vMax.m128_f32[0] <= vCharacterPosition.m128_f32[0])
		{
			//RB
			if (vMin.m128_f32[2] > vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(0, 0, -1, 0) : XMVectorSet(1, 0, 0, 0);
			}
			//RT
			if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(1, 0, 0, 0) : XMVectorSet(0, 0, 1, 0);
			}
			//R
			else
			{
				vCollisionNormal = XMVectorSet(1, 0, 0, 0);
			}
		}
		//가운데
		else
		{
			//B
			if (vMin.m128_f32[2] > vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = XMVectorSet(0, 0, -1, 0);
			}
			//T
			else if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
			{
				vCollisionNormal = XMVectorSet(0, 0, 1, 0);

			}
			//M
			else
			{
				return  vCharacterPosition;
			}
		}

		//비빌 때 너무 빠름

		_vector vVertical = vCollisionNormal * fCollisionDepth;
		//_vector v11 = XMVector4Dot(vCollisionNormal, -fMoveDistance * vMoveDirection);
		//_vector v1 = vCollisionNormal * XMVectorGetX(v11);
		//_vector v2 = fMoveDistance * vMoveDirection ;
		//_vector vHorizontal = XMVectorSetX(  v2,.0);//v1+v2
		//_vector v2 = fMoveDistance * vMoveDirection;
		//_vector vHorizontal = v1 + v2;
		return  vNextPosition + vVertical;
	}

	return vCharacterPosition + vMoveDirection * fMoveDistance;
}

_float CTerrainObject::Get_TopHeight(_vector Pos)
{
	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::BLOCK:
		return m_WorldMatrix.m[3][1] + 1.0f;
		break;

	case Client::BUILD_ITEM_TYPE::FILED:
	{
		m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));
		RaycastHit hitInfo;
		Pos = XMVectorSetY(Pos, XMVectorGetY(Pos) + 1.f);
		Ray ray(Pos, XMVECTOR{ 0,-1,0,0 });

		if (m_pColliderCom->RayCast(ray, &hitInfo))
		{
			return XMVectorGetY(hitInfo.vPoint);
		}
		break;
	}
	case Client::BUILD_ITEM_TYPE::PORTAL:
	case Client::BUILD_ITEM_TYPE::SPAWN:
	case Client::BUILD_ITEM_TYPE::LAST:
	default:
		return m_WorldMatrix.m[3][1];
		break;
	}

	return -1;
}

_float CTerrainObject::Get_BottomHeight(_vector Pos)
{
	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::BLOCK:
		return m_WorldMatrix.m[3][1];
		break;

	case Client::BUILD_ITEM_TYPE::FILED:
	{
		m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));
		RaycastHit hitInfo;
		Pos = XMVectorSetY(Pos, XMVectorGetY(Pos) -1.f);
		Ray ray(Pos, XMVECTOR{ 0,1,0,0 });

		if (m_pColliderCom->RayCast(ray, &hitInfo))
		{
			return XMVectorGetY(hitInfo.vPoint);
		}
		break;
	}
	case Client::BUILD_ITEM_TYPE::PORTAL:
	case Client::BUILD_ITEM_TYPE::SPAWN:
	case Client::BUILD_ITEM_TYPE::LAST:
	default:
		return m_WorldMatrix.m[3][1];
		break;
	}

	return FLT_MAX;
}

_bool CTerrainObject::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));

	return m_pColliderCom->RayCast(tRay, pOut);
}



HRESULT CTerrainObject::Ready_Components(TERRAINOBJ_DESC* pDesc)
{

	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::PORTAL:
	case Client::BUILD_ITEM_TYPE::BLOCK:
	{
		CCollider_AABB::AABB_COLLIDER_DESC desc{};
		desc.vCenter = { 0,0.5f,0 };
		desc.vExtentes = { 0.5f,0.5f,0.5f };

		if (FAILED(Add_Component(LEVEL_LOADING, CCollider_AABB::m_szProtoTag,
			CCollider::m_szCompTag, reinterpret_cast<CComponent**>(&m_pColliderCom), &desc)))
			return E_FAIL;
		break;
	}
	case Client::BUILD_ITEM_TYPE::FILED:
	{
		CCollider_Mesh::MESH_COLLIDER_DESC desc{};
		desc.pMesh = m_pModelCom->Get_Mesh(0);

		if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Mesh::m_szProtoTag,
			CCollider::m_szCompTag, reinterpret_cast<CComponent**>(&m_pColliderCom), &desc)))
			return E_FAIL;
		break;
	}
	case Client::BUILD_ITEM_TYPE::SPAWN:
	case Client::BUILD_ITEM_TYPE::LAST:
	default:
		m_pColliderCom = nullptr;
		break;
	}


	return S_OK; 
}


CTerrainObject* CTerrainObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrainObject* pInstance = new CTerrainObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrainObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrainObject::Clone(void* pArg)
{
	CTerrainObject* pInstance = new CTerrainObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrainObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainObject::Free()
{
 	__super::Free();
	Safe_Release(m_pColliderCom);
}
