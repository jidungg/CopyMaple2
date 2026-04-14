#include "..\Public\InstancedStaticModel.h"
#include "Mesh.h"

CInstancedStaticModel::CInstancedStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBaseModel{ pDevice, pContext }
{
}

CInstancedStaticModel::CInstancedStaticModel(const CInstancedStaticModel& Prototype)
    : CBaseModel{ Prototype }               // 메시/머티리얼 AddRef 포함
    , m_iMaxInstances{ Prototype.m_iMaxInstances }
{
    // m_pInstanceBuffer은 Clone 후 Initialize(pArg)에서 새로 생성
}

HRESULT CInstancedStaticModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    // 비애니메이션 모델 데이터(메시/머티리얼) 로드
    return __super::Initialize_Prototype(pModelFilePath, PreTransformMatrix);
}

HRESULT CInstancedStaticModel::Initialize(void* pArg)
{
    _uint iMax = 1000; // 기본 최대 인스턴스 수
    if (pArg)
        iMax = static_cast<INSTANCED_MODEL_DESC*>(pArg)->iMaxInstances;

    m_iMaxInstances = iMax;
    return Create_InstanceBuffer(m_iMaxInstances);
}

HRESULT CInstancedStaticModel::Create_InstanceBuffer(_uint iMaxInstances)
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth      = sizeof(INSTANCE_DATA) * iMaxInstances;
    desc.Usage          = D3D11_USAGE_DYNAMIC;
    desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return m_pDevice->CreateBuffer(&desc, nullptr, &m_pInstanceBuffer);
}

HRESULT CInstancedStaticModel::Set_Instances(const vector<INSTANCE_DATA>& Instances)
{
    if (Instances.empty())
    {
        m_iNumInstances = 0;
        return S_OK;
    }

    _uint iCount = min((_uint)Instances.size(), m_iMaxInstances);

    D3D11_MAPPED_SUBRESOURCE sub{};
    if (FAILED(m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub)))
        return E_FAIL;

    memcpy(sub.pData, Instances.data(), sizeof(INSTANCE_DATA) * iCount);
    m_pContext->Unmap(m_pInstanceBuffer, 0);

    m_iNumInstances = iCount;
    return S_OK;
}

HRESULT CInstancedStaticModel::Render(_uint iMeshIndex)
{
    if (m_iNumInstances == 0)
        return S_OK;
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    CMesh* pMesh = m_Meshes[iMeshIndex];

    // 슬롯 0: 메시 정점 버퍼 + 인덱스 버퍼 + 토폴로지
    pMesh->Bind_BufferDesc();

    // 슬롯 1: 인스턴스 버퍼 (WORLD 행렬 + 텍스처 인덱스)
    _uint iStride = sizeof(INSTANCE_DATA);
    _uint iOffset = 0;
    m_pContext->IASetVertexBuffers(1, 1, &m_pInstanceBuffer, &iStride, &iOffset);

    // 인스턴스드 드로우콜
    m_pContext->DrawIndexedInstanced(pMesh->Get_IndexCount(), m_iNumInstances, 0, 0, 0);

    return S_OK;
}

CInstancedStaticModel* CInstancedStaticModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                                                      const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    CInstancedStaticModel* pInstance = new CInstancedStaticModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : CInstancedStaticModel");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CInstancedStaticModel::Clone(void* pArg)
{
    CInstancedStaticModel* pInstance = new CInstancedStaticModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CInstancedStaticModel");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CInstancedStaticModel::Free()
{
    __super::Free();
    Safe_Release(m_pInstanceBuffer);
}
