#pragma once

#include "Base.h"
BEGIN(Engine)
class CGameInstance;

END
BEGIN(Client)

class COctoTree final : public CBase
{
public:
	//X Y Z 순
	enum CORNER {
		LBN, RBN,RBF , LBF,
		LTF, RTF, RTN, LTN, CORNER_END };
private:
	COctoTree();
	virtual ~COctoTree() = default;

public:
	HRESULT Initialize(XMUINT3 i3TotalSize,_uint iLBN,_uint iRTF);

	COctoTree* Get_Child(CORNER iIndex) { return m_pChildren[iIndex]; }
	_uint* Get_Corners() { return m_iCorners; }
	_uint Get_Center() { return m_iCenter; }
	_bool Is_Leaf() { return m_bIsLeaf; }
	_uint Get_MaxSize();
	_float Get_Radius(_float fInterval);
	_uint Get_CellCount();

	void To_String(string& outStr,_uint iIndent =0 );
private:
	_uint CombineIndex(XMUINT3 i3TotalSize, XMUINT3 i3Index);
	XMUINT3 SplitIndex(XMUINT3 i3TotalSize, _uint iIndex);

private:
	_uint							m_iCorners[CORNER_END] = {UINT_MAX,};
	_uint							m_iCenter = {};
	COctoTree*				m_pChildren[CORNER_END] = { nullptr };
	CGameInstance*		m_pGameInstance = { nullptr };
	XMUINT3					m_vSize = {};
	_bool						m_bIsLeaf = { false };
	XMUINT3 				m_i3TotalSize = {};

public:
	static COctoTree* Create(XMUINT3 i3TotalSize, _uint iLBN, _uint iRTF);
	virtual void Free() override;
};

END