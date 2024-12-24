#include "stdafx.h"
#include "UIModelPad.h"
#include "GameInstance.h"
#include "RenderTarget.h"


CUIModelPad::CUIModelPad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CUIModelPad::CUIModelPad(const CUIModelPad& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUIModelPad::Initialize(void* pArg)
{

	UIModelPadDesc* pDesc = static_cast<UIModelPadDesc*>(pArg);
	m_pModel = pDesc->pModel;
	Safe_AddRef(m_pModel);

	m_tModelViewport.TopLeftX = 0.0f;
	m_tModelViewport.TopLeftY = 0.0f;
	m_tModelViewport.Width = pDesc->fSizeX;
	m_tModelViewport.Height = pDesc->fSizeY;
	m_tModelViewport.MinDepth = 0.0f;
	m_tModelViewport.MaxDepth = 1.0f;

	m_pModelRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, pDesc->fSizeX, pDesc->fSizeY, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f));
	if (nullptr == m_pModelRenderTarget)
		return E_FAIL;
	if (FAILED(Ready_DepthStencilView(pDesc->fSizeX, pDesc->fSizeY)))
		return E_FAIL;

	_float fAspect = pDesc->fSizeX/ pDesc->fSizeY;
	_float fFovY = XMConvertToRadians(40.f);
	_float fNear = 0.1f;
	_float fFar = 1000.f;
	m_ModelProjMatrix = XMMatrixPerspectiveFovLH(fFovY, fAspect, fNear, fFar);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_LOADING, CVIBuffer_Rect::m_szPrptotypeTag,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIModelPad::Render()
{
	//VIEWPORT SET
	_uint iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &m_tOriginalViewport);
	m_pContext->RSSetViewports(1, &m_tModelViewport);

	//VIEW&PROJ MATRIX SET
	_matrix matWorld = XMLoadFloat4x4( m_pModel->Get_WorldMatrix());
	_vector vLook = XMVector3Normalize( matWorld.r[2]);
	_vector vModelPos = matWorld.r[3];
	_vector vAt = vModelPos + _vector{ 0,0.5,0 };
	_vector vPosition = vAt + vLook*1.5f;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_ModelViewMatrix = XMMatrixLookAtLH(vPosition, vAt, vUp);
	_matrix matViewOrigin =  m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProjOrigin = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_ModelViewMatrix);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, m_ModelProjMatrix);


	//RENDER TARGET & DSSSET
	ID3D11RenderTargetView* pBackRTV = { nullptr };
	ID3D11DepthStencilView* pOriginalDSV = { nullptr };
	m_pContext->OMGetRenderTargets(1, &pBackRTV, &pOriginalDSV);

	ID3D11RenderTargetView* pRenderTargets[1] = { nullptr};

	_uint			iNumRenderTargets =1;
	m_pModelRenderTarget->Clear();
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	pRenderTargets[0] = m_pModelRenderTarget->Get_RTV();

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, m_pDepthStencilView);

	m_pModel->Render();

	//RENDER TARGET & DSS UNSET
	m_pContext->OMSetRenderTargets(1, &pBackRTV, pOriginalDSV);

	//VIWPORT UNSET
	Safe_Release(pOriginalDSV);
	Safe_Release(pBackRTV);
	m_pContext->RSSetViewports(1, &m_tOriginalViewport);

	//MATRIX UNSET
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, matViewOrigin);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, matProjOrigin);

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(5);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}

	return S_OK;
}
 

HRESULT CUIModelPad::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_PROJ))))
			return E_FAIL;
	}
	if (m_pModelRenderTarget)
		if (FAILED( m_pShaderCom->Bind_SRV("g_Texture", m_pModelRenderTarget->Get_SRV())))
			return E_FAIL;
	return S_OK;
}

HRESULT CUIModelPad::Ready_DepthStencilView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* ���� ������ �ȼ��� ��������� �ȼ��� ������ �����ؾ߸� ���� �ؽ�Ʈ�� ����������. */
	/* �ȼ��� ���� �ٸ��� �ƿ� �������� ����. */
	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	/* ����? ����?  */
	TextureDesc.Usage = D3D11_USAGE_DEFAULT /* ���� */;
	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTargetView */
	/* ShaderResourceView */
	/* DepthStencilView */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

CUIModelPad* CUIModelPad::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	CUIModelPad* pInstance = new CUIModelPad(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIModelPad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIModelPad::Clone(void* pArg)
{
	CUIModelPad* pInstance = new CUIModelPad(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIModelPad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIModelPad::Free()
{
	__super::Free();
	Safe_Release(m_pModel);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pModelRenderTarget);
	Safe_Release(m_pDepthStencilView);
}
