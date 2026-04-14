#include "..\Public\BaseModel.h"
#include "Mesh.h"       // CMesh::Create, CModel::TYPE_NONANIM (Mesh.h includes Model.h)
#include "Material.h"
#include "Bone.h"       // CBone::Create (본 스킵용)
#include "Shader.h"

CBaseModel::CBaseModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CBaseModel::CBaseModel(const CBaseModel& Prototype)
    : CComponent{ Prototype }
    , m_iNumMeshes{ Prototype.m_iNumMeshes }
    , m_Meshes{ Prototype.m_Meshes }
    , m_iNumMaterials{ Prototype.m_iNumMaterials }
    , m_Materials{ Prototype.m_Materials }
    , m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);
    for (auto& pMat : m_Materials)
        Safe_AddRef(pMat);
}

// 비애니메이션 전용. 본/애니메이션 섹션을 읽고 버림.
// CModel은 이 메서드를 완전 오버라이드하므로 호출되지 않음.
HRESULT CBaseModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    ifstream inFile(pModelFilePath, ios::binary);
    if (!inFile)
    {
        string str = "파일을 열 수 없습니다.: ";
        str += pModelFilePath;
        MessageBoxA(NULL, str.c_str(), "에러", MB_OK);
        return E_FAIL;
    }

    bool bAnim;
    inFile.read(reinterpret_cast<char*>(&bAnim), sizeof(bool));
    (void)bAnim; // CBaseModel은 비애니메이션 전용이므로 플래그 사용 안 함

    // 본 데이터 스킵 (비애니메이션 모델에도 루트 본이 포함됨)
    if (FAILED(Ready_Bones_Skip(inFile, -1)))
        return E_FAIL;

    if (FAILED(Ready_Meshes(inFile)))
        return E_FAIL;

    if (FAILED(Ready_Materials(inFile, pModelFilePath)))
        return E_FAIL;

    // 애니메이션 개수 읽기 (비애니메이션 모델은 0)
    _uint iNumAnimations = 0;
    inFile.read(reinterpret_cast<char*>(&iNumAnimations), sizeof(_uint));
    // iNumAnimations == 0 이므로 루프 없음

    inFile.close();
    return S_OK;
}

HRESULT CBaseModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CBaseModel::Bind_Material(CShader* pShader, const _char* pConstantName,
                                   _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex)
{
    if (iMeshIndex >= (_uint)m_Meshes.size()) return E_FAIL;
    auto pMesh = m_Meshes[iMeshIndex];
    if (!pMesh) return E_FAIL;
    _uint matIndex = pMesh->Get_MaterialIndex();
    if (matIndex >= m_Materials.size()) return E_FAIL;
    auto pMat = m_Materials[matIndex];
    if (!pMat) return E_FAIL;
    return pMat->Bind_Texture(pShader, pConstantName, eType, iTextureIndex);
}

bool CBaseModel::Is_MeshActive(_uint iIdx)
{
    if (iIdx >= (_uint)m_Meshes.size()) return false;
    return m_Meshes[iIdx]->Is_Active();
}

void CBaseModel::Set_MeshActive(_uint iIdx, _bool bIsOn)
{
    if (iIdx >= (_uint)m_Meshes.size()) return;
    m_Meshes[iIdx]->Set_Active(bIsOn);
}

HRESULT CBaseModel::Ready_Meshes(ifstream& inFile)
{
    inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        // CModel::TYPE_NONANIM 고정, pModel=nullptr (비애니메이션은 pModel 불필요)
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext,
                                     CModel::TYPE_NONANIM, nullptr,
                                     inFile, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;
        m_Meshes.push_back(pMesh);
    }
    return S_OK;
}

HRESULT CBaseModel::Ready_Materials(ifstream& inFile, const _char* pModelFilePath)
{
    inFile.read(reinterpret_cast<char*>(&m_iNumMaterials), sizeof(_uint));
    m_Materials.resize(m_iNumMaterials);

    _char szDrive[MAX_PATH] = "";
    _char szDirectory[MAX_PATH] = "";
    _splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
    strcat_s(szDrive, szDirectory);

    for (_uint i = 0; i < m_iNumMaterials; i++)
    {
        CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, szDrive, inFile);
        if (nullptr == pMaterial)
            return E_FAIL;
        m_Materials[i] = pMaterial;
    }
    return S_OK;
}

// CBone::Create로 파일 스트림을 올바르게 전진시키고 즉시 해제
HRESULT CBaseModel::Ready_Bones_Skip(ifstream& inFile, _int iParentBoneIndex)
{
    CBone* pBone = CBone::Create(inFile, iParentBoneIndex);
    if (nullptr == pBone)
        return E_FAIL;
    Safe_Release(pBone); // 즉시 버림

    _uint iNumChildren = 0;
    inFile.read(reinterpret_cast<char*>(&iNumChildren), sizeof(_uint));
    for (_uint i = 0; i < iNumChildren; ++i)
    {
        if (FAILED(Ready_Bones_Skip(inFile, -1)))  // 버리므로 -1로 통일
            return E_FAIL;
    }
    return S_OK;
}

void CBaseModel::Free()
{
    __super::Free();
    for (auto& pMat : m_Materials)
        Safe_Release(pMat);
    m_Materials.clear();
    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);
    m_Meshes.clear();
}
