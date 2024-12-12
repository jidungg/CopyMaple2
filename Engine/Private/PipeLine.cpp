#include "..\Public\PipeLine.h"

CPipeLine::CPipeLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBase(), m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


HRESULT CPipeLine::Initialize()
{
	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	_float	iViewportWidth = {};
	_float	iViewportHeight = {};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	/* ����Ƣ���� ���� ���, �����ร�� �������. */
	XMStoreFloat4x4(&m_TransformMatrices[D3DTS_UI_VIEW], XMMatrixIdentity());
	XMStoreFloat4x4(&m_TransformMatrices[D3DTS_UI_PROJ], XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	return S_OK;
}

void CPipeLine::Update()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformInverseMatrices[i], 
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrices[i])));
	}

	XMStoreFloat4(&m_vCamPosition, XMLoadFloat4((_float4*)&m_TransformInverseMatrices[D3DTS_VIEW].m[3]));	
}

CPipeLine * CPipeLine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPipeLine*		pInstance = new CPipeLine(pDevice,pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CPipeLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeLine::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}