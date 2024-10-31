#include "CollisionMgr.h"
#include "Export_Utility.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

void CCollisionMgr::Free()
{ 
}
void CCollisionMgr::Set_CollisionMatrix(LAYER_ID eObjectLayer, LAYER_ID eSubjectLayer, bool bValue)
{
	UINT iRow = (UINT)eObjectLayer;
	UINT iCol = (UINT)eSubjectLayer;

	if (iCol < iRow)
	{
		iRow = (UINT)eSubjectLayer;
		iCol = (UINT)eObjectLayer;
	}

	if(bValue)
		m_arrCheck[iRow] |= (1 << iCol);
	else
		m_arrCheck[iRow] &= ~(1 << iCol);

}

void CCollisionMgr::Update_Collision()
{
	for (int row = 0; row < (int)LAYER_ID::LAST; row++)
	{
		for (int col = 0; col < (int)LAYER_ID::LAST; col++)
		{
			if(m_arrCheck[row] & (1 << col))
				Check_Collision_Layer((LAYER_ID)row, (LAYER_ID)col);
		}
	}
}

void CCollisionMgr::Check_Collision_Layer(LAYER_ID eObjectLayer, LAYER_ID eSubjectLayer)
{
	auto pLeftLayer = Get_GameObjectList(eObjectLayer);
	auto pRightLayer = Get_GameObjectList(eSubjectLayer);
	CCollider* pObject ;
	CCollider* pSubject;
	COLLISION_ID ID;

	for (auto& left : *pLeftLayer)
	{
		if (left.second->Is_Active() == false)continue;
		pObject = (CCollider*)left.second->Find_Component(COMPONENT_GROUP::ID_DYNAMIC, CCollider::To_String());
		if (pObject == nullptr || pObject->Is_Active() == false) continue;
		for (auto& right : *pRightLayer)
		{
			if (right.second->Is_Active() == false)continue;
			pSubject = (CCollider*)right.second->Find_Component(COMPONENT_GROUP::ID_DYNAMIC, CCollider::To_String());
			if (pSubject== nullptr || pObject->Is_Active() == false) continue;
			if (pObject == pSubject) continue;


			ID.Left_Id = pObject->Get_ID();
			ID.Right_Id = pSubject->Get_ID();

			auto iter = m_mapColBefore.find(ID.ID);
			if (iter == m_mapColBefore.end())
			{
				m_mapColBefore.insert({ ID.ID, false });
				iter = m_mapColBefore.find(ID.ID);
			}
			_vec3 vIntesect;
			if (Is_Collision(pObject, pSubject, vIntesect))
			{
				if (iter->second)
				{
					pObject->OnCollision(pSubject, vIntesect);
					pSubject->OnCollision(pObject, vIntesect);
				}
				else
				{
					pObject->OnCollisionEnter(pSubject, vIntesect);
					pSubject->OnCollisionEnter(pObject, vIntesect);
					iter->second = true;
				}
			}
			else
			{
				if (iter->second)
				{
					pObject->OnCollisionExit(pSubject, vIntesect);
					pSubject->OnCollisionExit(pObject, vIntesect);
					iter->second = false;
				}
			}

		}
	}
}

bool CCollisionMgr::Is_Collision(CCollider* pObject, CCollider* pSubject, _vec3& vOutIntersect)
{
	pObject->Update_Collider();
	pSubject->Update_Collider();
	if (pObject->Get_ColliderType() ==COLLIDER_TYPE::LINE)
	{
		return pObject->Is_Collision(pSubject, vOutIntersect);
	}
	else if (pSubject->Get_ColliderType() == COLLIDER_TYPE::LINE)
	{
		return pSubject->Is_Collision(pObject, vOutIntersect);
	}
	_vec3 vObjMin = pObject->Get_Min();
	_vec3 vObjMax = pObject->Get_Max();
	_vec3 vSbjMin = pSubject->Get_Min();
	_vec3 vSbjMax = pSubject->Get_Max();

	if (vObjMax.x < vSbjMin.x || vObjMin.x > vSbjMax.x)
		return false;	   									
	if (vObjMax.y < vSbjMin.y || vObjMin.y > vSbjMax.y)
		return false;	   									
	if (vObjMax.z < vSbjMin.z || vObjMin.z > vSbjMax.z)
		return false;


	return true;

}



void CCollisionMgr::Zero_CollisionMatrix()
{
	memset(m_arrCheck,0,sizeof(LAYER_ID::LAST)* sizeof(UINT32));
}

