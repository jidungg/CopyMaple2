#include "stdafx.h"
#include "WorldUIObject.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Texture.h"

CWorldUIObject::CWorldUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CWorldUIObject::CWorldUIObject(const CWorldUIObject& Prototype)
	:CUIObject(Prototype),
    m_pVIBufferCom{ Prototype.m_pVIBufferCom },
    m_pShaderCom{ Prototype.m_pShaderCom }
{
}


HRESULT CWorldUIObject::Initialize(void* pArg)
{
	WorldUIObjDesc* pDesc = static_cast<WorldUIObjDesc*>(pArg);
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}



HRESULT CWorldUIObject::Ready_Components(void* pArg)
{
    /* Com_VIBuffer */
    m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Proto_Component_Stock(CVIBuffer_Rect::m_szPrptotypeTag));
    if (FAILED(Add_Component(m_pVIBufferCom, TEXT("Com_VIBuffer"))))
        return E_FAIL;

  
    /* Com_Shader */
    if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWorldUIObject::Render()
{
    if (FAILED(Bind_ShaderResources(m_pShaderCom)))
        return E_FAIL;

    if (m_pShaderCom)
        m_pShaderCom->Begin(0);
    if (m_pVIBufferCom)
    {
        m_pVIBufferCom->Bind_BufferDesc();
        m_pVIBufferCom->Render();
    }

    for (auto& child : m_pChilds)
        if (child->Is_Active())
            child->Render();
    return S_OK;
}
void CWorldUIObject::Compute_Matrix()
{
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    if (m_pParentMatrix != nullptr)
    {
        vPos = XMVector4Transform( vPos, XMLoadFloat4x4(m_pParentMatrix));
    }

    _matrix vView= XMLoadFloat4x4(&m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
    _matrix vProj = XMLoadFloat4x4(&m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));
    vPos = XMVector4Transform( vPos, vView);
    vPos = XMVector4Transform( vPos, vProj);
    vPos /= vPos.m128_f32[3];
	vPos.m128_f32[0] = (vPos.m128_f32[0]) * g_iWinSizeX *0.5 ;
	vPos.m128_f32[1] = ( vPos.m128_f32[1]) * g_iWinSizeY * 0.5;

	XMStoreFloat4x4( &m_WorldMatrix , XMMatrixIdentity());
	m_WorldMatrix.m[3][0] = vPos.m128_f32[0];
	m_WorldMatrix.m[3][1] = vPos.m128_f32[1];
	m_WorldMatrix.m[3][2] = vPos.m128_f32[2];
	m_WorldMatrix.m[3][3] = vPos.m128_f32[3];

	m_WorldMatrix.m[0][0] = fSize.x;
	m_WorldMatrix.m[1][1] = fSize.y;
	m_WorldMatrix.m[2][2] = 1;
}
HRESULT CWorldUIObject::Bind_ShaderResources(CShader* pShader)
{
    if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    
    if (m_pShaderCom)
    {
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_VIEW))))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_PROJ))))
            return E_FAIL;
    }

    return S_OK;
}

CWorldUIObject* CWorldUIObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWorldUIObject* pInstance = new CWorldUIObject(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CWorldUIObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWorldUIObject::Clone(void* pArg)
{
    CWorldUIObject* pInstance = new CWorldUIObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CWorldUIObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWorldUIObject::Free()
{
    __super::Free();
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
}
