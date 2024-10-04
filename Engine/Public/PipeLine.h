#pragma once

#include "Base.h"

/* �������� ���Ǵ� ���������� ���� ��, ��������� �����Ѵ�. */
/* ������ɷ����������þƤӤ��� �������� �ʴ´�.(��ġ�� ����� �����Ѵ� �������.) */
/* ī�޶� ��, ������ �����ϱ������� ������� �׻� ���������ο� �������ٰŴ�. */
/* ������������ ���ؼ� ���� ������ ��, �������� ���� �� �ֵ��� �ϰڴ�. */
/* Update�Լ��� ���ؼ� ��, ��������� ������� �������Ӹ��� �ѹ��� ���س��´�. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrices[eState], TransformMatrix);
	}

	_matrix Get_TransformMatrix(D3DTRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformMatrices[eState]);
	}

	_float4x4 Get_TransformFloat4x4(D3DTRANSFORMSTATE eState) {
		return m_TransformMatrices[eState];
	}

public:
	HRESULT Initialize();
	void Update();

private:
	_float4x4				m_TransformMatrices[D3DTS_END];
	_float4x4				m_TransformInverseMatrices[D3DTS_END];

	_float4					m_vCamPosition = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END