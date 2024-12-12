#pragma once

#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ,D3DTS_UI_VIEW, D3DTS_UI_PROJ, D3DTS_END };
private:
	CPipeLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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

	_float4x4 Get_TransformFloat4x4_Inverse(D3DTRANSFORMSTATE eState) {
		return m_TransformInverseMatrices[eState];
	}

	_matrix Get_TransformMatrix_Inverse(D3DTRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformInverseMatrices[eState]);
	}
	const _float4* Get_CamPosition() {return &m_vCamPosition;}
public:
	HRESULT Initialize();
	void Update();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	_float4x4				m_TransformMatrices[D3DTS_END];
	_float4x4				m_TransformInverseMatrices[D3DTS_END];
	_float4					m_vCamPosition = {};

public:
	static CPipeLine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END