#include "stdafx.h"
#include "WayFinder.h"
#include "GameInstance.h"
#include "CubeTerrain.h"
#include "Client_Utility.h"
#include "TerrainObject.h"

CWayFinder::CWayFinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CWayFinder::CWayFinder(const CWayFinder& Prototype)
	:CComponent(Prototype)
	, m_pCubeTerrain(Prototype.m_pCubeTerrain)
{
}

HRESULT CWayFinder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWayFinder::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	WAYFINDER_DESC* pDesc = (WAYFINDER_DESC*)pArg;
	m_pCubeTerrain = pDesc->pCubeTerrain;
	if (nullptr == m_pCubeTerrain)
		return E_FAIL;
	return S_OK;
}

void CWayFinder::Update(_float fTimeDelta)
{
}

void CWayFinder::Late_Update(_float fTimeDelta)
{
}

_bool CWayFinder::FindWay(_vector& vStart, _vector& vGoal, _uint iSearchRange)
{
	_uint iStartIdx = m_pCubeTerrain->PosToIndex(vStart);
	if( -1 ==iStartIdx)
		return false;
	vGoal.m128_f32[1] = m_pCubeTerrain->Get_FloorHeight(vGoal);
	_uint iGoalIdx = m_pCubeTerrain->PosToIndex(vGoal);
	if(-1 == iGoalIdx)
		return false;

	return MakeAStarRoute(iStartIdx, iGoalIdx);
}

_bool CWayFinder::MakeAStarRoute(_uint iStartIdx, _uint iDestIdx)
{
	//AStart 알고리즘
	// 주변의 갈 수 있는 노드를 찾음.
	// 갈 수 있는 노드들 중에서 가장 가까운 노드를 선택


	//1. 시작 노드를 OpenList에 넣음
	//2. OpenList에서 f값이 가장 작은 노드를 선택
		//g : 시작점에서 현재 노드까지의 비용
		//h : 현재 노드에서 목적지까지의 비용
		//f = g + h
		//f가 가장 작은 노드를 선택
	//3. 선택된 노드를 CloseList에 넣음
	//4. 선택된 노드의 주변 노드들을 OpenList에 넣음
	_bool bFind = false;
	m_mapAStarCell.clear();
	m_listOpenList.clear();
	m_listCloseList.clear();
	m_listRoute.clear();
	m_iterRoute = m_listRoute.end();
	ASTARCELL iCurrentCell = {};
	iCurrentCell.iIdx = iStartIdx;
	iCurrentCell.iPIdx = -1;
	iCurrentCell.fG = 0;
	iCurrentCell.fH = m_pCubeTerrain->Get_Distance(iStartIdx, iDestIdx);
	iCurrentCell.fF = iCurrentCell.fG + iCurrentCell.fH;
	m_listOpenList.push_back(iStartIdx);
	m_mapAStarCell[iStartIdx] = iCurrentCell;
	_int iCurrentIdx = -1;
	vector<_uint> vecAdjCells;
	ASTARCELL iAdjCell = {};
	while (false == m_listOpenList.empty())
	{

		//OpenList에서 f값이 가장 작은 노드를 선택, CloseList에 넣음, CurrentIndex로 설정
		m_listOpenList.sort([&](const _int& iLhs, const _int& iRhs)
			{
				return m_mapAStarCell[iLhs].fF < m_mapAStarCell[iRhs].fF;
			});
		iCurrentIdx = m_listOpenList.front();
		m_listOpenList.pop_front();
		m_listCloseList.push_back(iCurrentIdx);
		iCurrentCell = m_mapAStarCell[iCurrentIdx];
		vecAdjCells.clear();
		m_pCubeTerrain->Get_AdjCells(iCurrentIdx, vecAdjCells);

		//인접 노드들 OpenList에 넣음
		for (auto& iAdjIdx : vecAdjCells)
		{

			if (iAdjIdx == iDestIdx)
			{
				iAdjCell.iIdx = iAdjIdx;
				iAdjCell.iPIdx = iCurrentIdx;
				m_listCloseList.push_back(iAdjIdx);
				m_mapAStarCell[iAdjIdx] = iAdjCell;
				bFind =  true;
				break;
			}
			if (m_listCloseList.end() != find(m_listCloseList.begin(), m_listCloseList.end(), iAdjIdx))
				continue;
			//이미 OpenList에 있는 노드인 경우
			if (m_listOpenList.end() != find(m_listOpenList.begin(), m_listOpenList.end(), iAdjIdx))
			{
				//현재 노드에서의 비용이 더 작으면 갱신
				m_mapAStarCell[iAdjIdx].iPIdx = iCurrentIdx;
				_float fNewG = iCurrentCell.fG + m_pCubeTerrain->Get_AdjDistance(iCurrentIdx, iAdjIdx);
				if(m_mapAStarCell[iAdjIdx].fG > fNewG)
				{
					m_mapAStarCell[iAdjIdx].fG = fNewG;
					m_mapAStarCell[iAdjIdx].fF = m_mapAStarCell[iAdjIdx].fG + m_mapAStarCell[iAdjIdx].fH;
				}
				continue;
			}
			else
			{
				iAdjCell.iIdx = iAdjIdx;
				iAdjCell.iPIdx = iCurrentIdx;
				iAdjCell.fG = iCurrentCell.fG + m_pCubeTerrain->Get_AdjDistance(iCurrentIdx, iAdjIdx);
				iAdjCell.fH = m_pCubeTerrain->Get_Distance(iAdjIdx, iDestIdx);
				iAdjCell.fF = iAdjCell.fG + iAdjCell.fH;
				m_listOpenList.push_back(iAdjIdx);
				m_mapAStarCell[iAdjIdx] = iAdjCell;
			}

		}
		if (bFind) break;
		if (m_listOpenList.empty()) return false;

	}
	//목적지에서 시작지점까지 경로를 찾음
	if (bFind)
	{
		iCurrentIdx = iDestIdx;
		while (iCurrentIdx != -1)
		{
			m_listRoute.push_front(m_pCubeTerrain->IndexToPos(iCurrentIdx));
			iCurrentIdx = m_mapAStarCell[iCurrentIdx].iPIdx;
		}
		m_iterRoute = m_listRoute.begin();
	}
	return bFind;
}
_vector CWayFinder::Get_NextStation()
{
	if (m_iterRoute != m_listRoute.end())
	{
		++m_iterRoute;
		return (*m_iterRoute);
	}
}





CWayFinder* CWayFinder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return  new CWayFinder(pDevice, pContext);;
}

CComponent* CWayFinder::Clone(void* pArg)
{
	return new CWayFinder(*this);
}
