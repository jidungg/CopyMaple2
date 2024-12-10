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
	m_pCubeColliderCom{ Prototype.m_pCubeColliderCom }
	, m_eTerrainDir{ Prototype.m_eTerrainDir }
	, m_iIndex{ Prototype.m_iIndex }
	, m_iBuildItemID{ Prototype.m_iBuildItemID }
{

	Safe_AddRef(m_pCubeColliderCom);
	Safe_AddRef(m_pMeshColliderCom);
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
	m_iBuildItemID = pDesc->iID;
	m_eBlockType = static_cast<BUILD_ITEM_DATA*>(ITEMDB->Get_Data(ITEM_TYPE::BUILD, (_uint)m_iBuildItemID))->eBlockType;
	m_eBuildItemType= static_cast<BUILD_ITEM_DATA*>( ITEMDB->Get_Data(ITEM_TYPE::BUILD,(_uint)m_iBuildItemID))->eBuildType;
	m_eTerrainDir = pDesc->direction;
	m_iIndex = pDesc->index;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->pos.x, pDesc->pos.y, pDesc->pos.z,1));
	m_pTransformCom->LookToward(Get_Direction_Vector(m_eTerrainDir));
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainObject::Ready_Components(TERRAINOBJ_DESC* pDesc)
{

	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::FLOOR:
	case Client::BUILD_ITEM_TYPE::WALL:
	case Client::BUILD_ITEM_TYPE::GROUND:
	{
		CCollider_AABB::AABB_COLLIDER_DESC tDesc{};
		tDesc.vCenter = { 0,0.5f,0 };
		tDesc.vExtentes = { 0.5f,0.5f,0.5f };

		if (FAILED(Add_Component(LEVEL_LOADING, CCollider_AABB::m_szProtoTag,
			TEXT("Cube_Collider_Com"), reinterpret_cast<CComponent**>(&m_pCubeColliderCom), &tDesc)))
			return E_FAIL;
		m_vecCollider.push_back(m_pCubeColliderCom);
		break;
	}
	case Client::BUILD_ITEM_TYPE::DEFORM:
	case Client::BUILD_ITEM_TYPE::CUBRIC:
	case Client::BUILD_ITEM_TYPE::STRUC:
	case Client::BUILD_ITEM_TYPE::PROP:
	case Client::BUILD_ITEM_TYPE::NATURE:
	case Client::BUILD_ITEM_TYPE::FUNCT:
	{
		CCollider_Mesh::MESH_COLLIDER_DESC tMeshDesc{};
		tMeshDesc.pMesh = m_pModelCom->Get_Mesh(0);

		if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Mesh::m_szProtoTag,
			TEXT("Mesh_Collider_Com"), reinterpret_cast<CComponent**>(&m_pMeshColliderCom), &tMeshDesc)))
			return E_FAIL;
		m_vecCollider.push_back(m_pMeshColliderCom);
		break;
	}
	default:
		m_pCubeColliderCom = nullptr;
		m_pMeshColliderCom = nullptr;
		break;
	}
	return S_OK;
}


void CTerrainObject::Update(_float fTimeDelta)
{
	if(m_bRotating)
		m_bRotating = false == m_pTransformCom->TurnToward(Get_Direction_Vector(m_eTerrainDir), fTimeDelta);

	__super::Update(fTimeDelta);
}


json CTerrainObject::ToJson()
{
	json j;
	j["ItemId"] = m_iBuildItemID;
	list<_int> iData;
	j["IntData"] = iData;
	list<_float> fData;
	j["FloatData"] = fData;
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
	//if (nullptr == m_pCubeColliderCom)
	//	return vCharacterPosition + vMoveDirection * fMoveDistance;
	if(false == Is_BlockingType())
		return vCharacterPosition + vMoveDirection * fMoveDistance;

	_vector vNextPosition = vCharacterPosition + vMoveDirection * fMoveDistance;
	if (false == DirectX::Internal::XMVector3IsUnit(vMoveDirection))
		return vNextPosition;




	//충돌한 평면을 찾는다
	//평면의 방정식에  CharPosition과 NextPosition을 대입해서 평면까지의 거리를 구한다.
	//평면에서 각 점으로까지의 거리의 부호가 다른지 확인한다.


	CColliderBase* pCollider = Get_Collider(0);
	pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
	Ray ray(XMVectorSetY( vCharacterPosition, XMVectorGetY(vCharacterPosition) +fCollisionHeight), vMoveDirection, fMoveDistance);
	RaycastHit hitInfo;
	//평면의 Normal * (NextPosition에서 평면까지의 거리+ Radius) 만큼 이동
	if (pCollider->RayCast(ray, &hitInfo))
	{
		_float fPlaneD = -XMVector3Dot(hitInfo.vNormal, hitInfo.vPoint).m128_f32[0];
		_float fPlaneDistance = XMVector3Dot(hitInfo.vNormal, vNextPosition).m128_f32[0] + fPlaneD;
		vNextPosition += hitInfo.vNormal * (fCollisionRadius + fabsf(fPlaneDistance));	
		return vNextPosition;
	}
	else if(m_eBuildItemType == BUILD_ITEM_TYPE::GROUND)
	{
		BoundingBox* tBox = static_cast<CCollider_AABB*>( pCollider)->Get_Desc();
		_float3* pCorners = new _float3[8];
		tBox->GetCorners(pCorners);
		_vector vBlockCenter = XMLoadFloat3(&tBox->Center);
		_vector vMin = XMLoadFloat3(&pCorners[4]);
		_vector vMax = XMLoadFloat3(&pCorners[2]);
		_float fBlockX = vMax.m128_f32[0] - vMin.m128_f32[0];
		_float fBlockZ = vMax.m128_f32[2] - vMin.m128_f32[2];
		delete[] pCorners;

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
					vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(-1, 0, 0, 0) : XMVectorSet(0, 0, -1, 0);
				//LT
				else if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
					vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(0, 0, 1, 0) : XMVectorSet(-1, 0, 0, 0);
				//L
				else
					vCollisionNormal = XMVectorSet(-1, 0, 0, 0);
			}
			//오른쪽
			else if (vMax.m128_f32[0] <= vCharacterPosition.m128_f32[0])
			{
				//RB
				if (vMin.m128_f32[2] > vCharacterPosition.m128_f32[2])
					vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(0, 0, -1, 0) : XMVectorSet(1, 0, 0, 0);
				//RT
				if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
					vCollisionNormal = fMoveSlope > fCollisionSlope ? XMVectorSet(1, 0, 0, 0) : XMVectorSet(0, 0, 1, 0);
				//R
				else
					vCollisionNormal = XMVectorSet(1, 0, 0, 0);
			}
			//가운데
			else
			{
				//B
				if (vMin.m128_f32[2] > vCharacterPosition.m128_f32[2])
					vCollisionNormal = XMVectorSet(0, 0, -1, 0);
				//T
				else if (vMax.m128_f32[2] <= vCharacterPosition.m128_f32[2])
					vCollisionNormal = XMVectorSet(0, 0, 1, 0);
				//M
				else
					return  vCharacterPosition;
			}

			_vector vVertical = vCollisionNormal * fCollisionDepth;
			return  vNextPosition + vVertical;
		}
	}
	

	return vNextPosition;
}
_float CTerrainObject::Get_TopHeight(_vector Pos)
{
	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::GROUND:
	case Client::BUILD_ITEM_TYPE::FLOOR:
	case Client::BUILD_ITEM_TYPE::WALL:
		return m_WorldMatrix.m[3][1] + 1.0f;
		break;
	case Client::BUILD_ITEM_TYPE::DEFORM:
	case Client::BUILD_ITEM_TYPE::CUBRIC:
	case Client::BUILD_ITEM_TYPE::STRUC:
	case Client::BUILD_ITEM_TYPE::FUNCT:
	case Client::BUILD_ITEM_TYPE::PROP:
	{
		m_pMeshColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));
		RaycastHit hitInfo;
		Pos = XMVectorSetY(Pos, XMVectorGetY(Pos) + 0.5f);
		Ray ray(Pos, XMVECTOR{ 0,-1,0,0 });

		if (m_pMeshColliderCom->RayCast(ray, &hitInfo))
		{
			//cout << XMVectorGetY(hitInfo.vPoint) << endl;
			return XMVectorGetY(hitInfo.vPoint); 
		}
		break;
	}
	case Client::BUILD_ITEM_TYPE::NATURE:
	case Client::BUILD_ITEM_TYPE::LAST:
	default:
		return -1;
		break;
	}

	return -1;
}
//바닥높이
_float CTerrainObject::Get_BottomHeight(_vector Pos)
{
	switch (m_eBuildItemType)
	{
	case Client::BUILD_ITEM_TYPE::GROUND:
	case Client::BUILD_ITEM_TYPE::FLOOR:
	case Client::BUILD_ITEM_TYPE::WALL:
		return m_WorldMatrix.m[3][1];
		break;

	case Client::BUILD_ITEM_TYPE::DEFORM:
	case Client::BUILD_ITEM_TYPE::CUBRIC:
	case Client::BUILD_ITEM_TYPE::STRUC:
	case Client::BUILD_ITEM_TYPE::FUNCT:
	case Client::BUILD_ITEM_TYPE::PROP:
	{
		CColliderBase* pCollider = Get_Collider(0);
		pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
		RaycastHit hitInfo;
		Pos = XMVectorSetY(Pos, XMVectorGetY(Pos) -1.f);
		Ray ray(Pos, XMVECTOR{ 0,1,0,0 });

		if (pCollider->RayCast(ray, &hitInfo))
		{
			return XMVectorGetY(hitInfo.vPoint);
		}
		break;
	}
	default:
		return FLT_MAX;
		break;
	}

	return FLT_MAX;
}

_bool CTerrainObject::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	CColliderBase* pCollider = Get_Collider(0);
	if (nullptr == pCollider)
		return false;
	if (false == Is_BlockingType()) 
		return false;
	pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));

	return pCollider->RayCast(tRay, pOut);
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
	Safe_Release(m_pCubeColliderCom);
	Safe_Release(m_pMeshColliderCom);
}
