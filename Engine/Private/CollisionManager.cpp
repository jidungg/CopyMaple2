#include "CollisionManager.h"
#include "Collider.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"

CCollisionManager::CCollisionManager(CObject_Manager* pObjMgr, _uint iLayerCount)
	: m_arrCheck{}
	, m_pObjectMgr(pObjMgr)
	, m_iLayerCount(iLayerCount)
{
	Set_LayerCount(iLayerCount);
}

HRESULT CCollisionManager::Initialize()
{
	return S_OK;
}
void CCollisionManager::Set_CollisionMatrix(_uint eObjectLayer, _uint eSubjectLayer, bool bValue)
{
	_uint iRow = (_uint)eObjectLayer;
	_uint iCol = (_uint)eSubjectLayer;

	//양방향 체크
	//if (iCol < iRow)
	//{
	//	iRow = (_uint)eSubjectLayer;
	//	iCol = (_uint)eObjectLayer;
	//}

	if(bValue)
		m_arrCheck[iRow] |= (1 << iCol);
	else
		m_arrCheck[iRow] &= ~(1 << iCol);

}

void CCollisionManager::Update_Collision()
{
	for (_uint row = 0; row < m_iLayerCount; row++)
	{
		for (_uint col = 0; col < m_iLayerCount; col++)
		{
			if(m_arrCheck[row] & (1 << col))
				Check_Collision_Layer(row, col);
		}
	}
}

void CCollisionManager::Check_Collision_Layer(_uint eObjectLayer, _uint eSubjectLayer)
{
	vector<CGameObject*>* pLeftLayer = m_pObjectMgr->Get_GameObjectList(eObjectLayer);
	vector<CGameObject*>* pRightLayer = m_pObjectMgr->Get_GameObjectList(eSubjectLayer);

	if (pLeftLayer == nullptr || pRightLayer == nullptr)
		return;
	for (auto& left : *pLeftLayer)
	{
		if (left->Is_Active() == false)continue;
		if (left->Is_Dead()) continue;
		for (auto& right :* pRightLayer)
		{
			if (right->Is_Active() == false)continue;
			if (right->Is_Dead()) continue;
			if (left == right) continue;

			left->Check_Collision(right);

		}
	}
}






void CCollisionManager::Zero_CollisionMatrix()
{
	memset(m_arrCheck.data(), 0, m_iLayerCount * sizeof(_uint));
}

CCollisionManager* CCollisionManager::Create(CObject_Manager* pObjMgr, _uint iLayerCount)
{
	CCollisionManager* pInstance = new CCollisionManager(pObjMgr,  iLayerCount);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CCollisionMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollisionManager::Free()
{
	__super::Free();
}