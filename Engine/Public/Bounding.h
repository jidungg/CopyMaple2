#pragma once

#include "Base.h"
#include "DebugDraw.h"
#include "Collider.h"

/* 1. �� �浹ü Ŭ�������� �θ� Ŭ������. */
/* 2. ���� ����ü�� ����Ǵ� ����ü�� �θ� ����ü�� ������. */

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct
	{
		_float3		vCenter;
	}BOUND_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) = 0;
	virtual _bool Intersect(CCollider::COLLIDER_TYPE eType, CBounding* pTargetBounding) = 0;

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_bool					m_isColl = { false };

public:
	virtual void Free() override;
};

END