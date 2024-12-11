#include "stdafx.h"
#include "OctoTree.h"
#include "GameInstance.h"

COctoTree::COctoTree()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}


HRESULT COctoTree::Initialize(XMUINT3 i3TotalSize, _uint iLBN, _uint iRTF)
{
	m_i3TotalSize = i3TotalSize;
	m_iCorners[LBN] = iLBN;
	m_iCorners[RTF] = iRTF;

	XMUINT3 i3LBN  = SplitIndex(i3TotalSize,m_iCorners[LBN]);
	XMUINT3 i3RTF = SplitIndex(i3TotalSize, m_iCorners[RTF]);
	m_vSize.x= i3RTF.x - i3LBN.x + 1;
	m_vSize.z = i3RTF.z - i3LBN.z + 1;
	m_vSize.y = i3RTF.y - i3LBN.y + 1;

	XMUINT3 i3LTF = { i3LBN.x, i3RTF.y, i3RTF.z };
	XMUINT3 i3RTN = { i3RTF.x, i3RTF.y, i3LBN.z };
	XMUINT3 i3LTN = { i3LBN.x, i3RTF.y, i3LBN.z };
	XMUINT3 i3LBF = { i3LTF.x, i3LBN.y, i3LTF.z };
	XMUINT3 i3RBF = { i3RTF.x, i3LBN.y, i3RTF.z };
	XMUINT3 i3RBN = { i3RTF.x, i3LBN.y, i3LBN.z };

	m_iCorners[LBF] = CombineIndex(i3TotalSize, i3LBF);
	m_iCorners[RBF] = CombineIndex(i3TotalSize, i3RBF);
	m_iCorners[RBN] = CombineIndex(i3TotalSize, i3RBN);
	m_iCorners[LTF] = CombineIndex(i3TotalSize, i3LTF);
	m_iCorners[RTN] = CombineIndex(i3TotalSize, i3RTN);
	m_iCorners[LTN] = CombineIndex(i3TotalSize, i3LTN);


	if (2 >= m_vSize.x && 2 >= m_vSize.y && 2 >= m_vSize.z)
	{
		m_bIsLeaf = true;
		return S_OK;
	}


	XMUINT3 i3Center = i3LBN;
	i3Center.x += m_vSize.x / 2 -1;
	i3Center.y += m_vSize.y / 2 -1;
	i3Center.z += m_vSize.z / 2 -1;
	m_iCenter = CombineIndex(i3TotalSize,i3Center);

	XMUINT3 i3TmpLBN, i3TmpRTF;


	//LBN
	i3TmpLBN = i3LBN;
	i3TmpRTF = i3Center;

	m_pChildren[LBN] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));


	//RBN

	i3TmpRTF.x = i3RTF.x;
	i3TmpRTF.y = i3Center.y;
	i3TmpRTF.z = i3Center.z;

	i3TmpLBN.x = i3Center.x +1;
	i3TmpLBN.y = i3LBN.y;
	i3TmpLBN.z = i3LBN.z;
	m_pChildren[RBN] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//LBF
	i3TmpLBN.x = i3LBN.x;
	i3TmpLBN.y = i3LBN.y;
	i3TmpLBN.z = i3Center.z+1;

	i3TmpRTF.x = i3Center.x;
	i3TmpRTF.y = i3Center.y;
	i3TmpRTF.z = i3RTF.z;
	m_pChildren[LBF] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//RBF	
	i3TmpRTF = i3RTF;
	i3TmpRTF.y = i3Center.y;
	i3TmpLBN.x = i3Center.x + 1;
	i3TmpLBN.z = i3Center.z + 1;
	i3TmpLBN.y = i3LBF.y;
	m_pChildren[RBF] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//LTF
	i3TmpRTF = i3RTF;
	i3TmpRTF.x = i3Center.x;
	i3TmpLBN.x = i3LBN.x;
	i3TmpLBN.y = i3Center.y +1;
	i3TmpLBN.z = i3Center.z +1;
	m_pChildren[LTF] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//RTF
	i3TmpRTF = i3RTF;
	i3TmpLBN = i3Center;
	i3TmpLBN.x += 1;
	i3TmpLBN.y += 1;
	i3TmpLBN.z += 1;
	m_pChildren[RTF] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//RTN

	i3TmpLBN.x = i3Center.x+1;
	i3TmpLBN.y = i3Center.y+1;
	i3TmpLBN.z  = i3LBN.z;

	i3TmpRTF = i3RTF;
	i3TmpRTF.z = i3Center.z;
	m_pChildren[RTN] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	//LTN
	i3TmpLBN = i3LBN;
	i3TmpLBN.y = i3Center.y +1;

	i3TmpRTF = i3Center;
	i3TmpRTF.y = i3RTF.y;
	m_pChildren[LTN] = COctoTree::Create(i3TotalSize, CombineIndex(i3TotalSize, i3TmpLBN), CombineIndex(i3TotalSize, i3TmpRTF));

	return S_OK;
}

_uint COctoTree::Get_MaxSize()
{
	return max(m_vSize.x, max(m_vSize.y, m_vSize.z));
}

_float COctoTree::Get_Radius(_float fInterval)
{
	
	return XMVectorGetX(XMVector3Length(XMVectorSet((_float)m_vSize.x * fInterval, (_float)m_vSize.y * fInterval, (_float)m_vSize.z * fInterval, 0)));
}

_uint COctoTree::Get_CellCount()
{
	return m_vSize.x* m_vSize.y * m_vSize.z;
}

void COctoTree::To_String(string& outStr, _uint iIndent )
{
	XMUINT3 i3LBN = SplitIndex(m_i3TotalSize, m_iCorners[LBN]);
	XMUINT3 i3RTF = SplitIndex(m_i3TotalSize, m_iCorners[RTF]);
	outStr += string(iIndent, '\t') + "MIN : " +to_string( i3LBN .x) + ","+ to_string(i3LBN.z) + ","+ to_string(i3LBN.y) +"\t";
	outStr += string(iIndent, '\t') + "MAX : " + to_string(i3RTF.x) + "," + to_string(i3RTF.z) + "," + to_string(i3RTF.y) + "\n";
	outStr += string(iIndent, '\t') + "size: " + to_string(Get_MaxSize()) + "/" + to_string(m_vSize.x) + "," + to_string(m_vSize.z) + "," + to_string(m_vSize.y) + "\n";

	if (m_bIsLeaf)
	{
		return;
	}
	for (_uint i = 0; i < COctoTree::CORNER_END; i++)
	{
		COctoTree* pChild = Get_Child((CORNER)i);
		if (nullptr != pChild)
			pChild->To_String(outStr, iIndent+1);
	}
}


_uint COctoTree::CombineIndex(XMUINT3 i3Size, XMUINT3 i3Index)
{
	return i3Index.x + i3Size.x * i3Index.z + i3Size.x * i3Size.z * i3Index.y;
}

XMUINT3 COctoTree::SplitIndex(XMUINT3 i3Size, _uint iIndex)
{
	XMUINT3		vResult = {};

	vResult.x = iIndex % i3Size.x;
	vResult.y = iIndex / (i3Size.x * i3Size.z);
	vResult.z = iIndex / i3Size.x % i3Size.z;

	return vResult;
}


COctoTree* COctoTree::Create(XMUINT3 i3TotalSize, _uint iLBN, _uint iRTF)
{
	COctoTree* pInstance = new COctoTree();

	if (FAILED(pInstance->Initialize(i3TotalSize, iLBN, iRTF)))
	{
		MSG_BOX("Failed to Created : COctoTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COctoTree::Free()
{
	__super::Free();

	for (auto& pChild : m_pChildren)
		Safe_Release(pChild);

	Safe_Release(m_pGameInstance);
}

