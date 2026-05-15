#pragma once
#include "Component.h"

BEGIN(Engine)
class CMaterial;
class CMesh;

class ENGINE_DLL CBaseModel abstract : public CComponent
{
protected:
    CBaseModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBaseModel(const CBaseModel& Prototype);
    virtual ~CBaseModel() = default;

public:
    // 비애니메이션 모델 파일(.model)을 읽어 메시·머티리얼 초기화.
    // CModel은 이 메서드를 완전 오버라이드함.
    // CInstancedStaticModel은 __super::Initialize_Prototype을 호출.
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual HRESULT Initialize(void* pArg) override;

    // 서브클래스가 구현: CModel은 DrawIndexed, CInstancedStaticModel은 DrawIndexedInstanced
    virtual HRESULT Render(_uint iMeshIndex) = 0;  // 순수 가상

public:
    HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex = 0);
    _uint   Get_NumMeshes() const { return m_iNumMeshes; }
    CMesh*  Get_Mesh(_uint iMeshIndex) const { return m_Meshes[iMeshIndex]; }
    bool    Is_MeshActive(_uint iIdx);
    void    Set_MeshActive(_uint iIdx, _bool bIsOn);

protected:
    _uint               m_iNumMeshes    = { 0 };
    vector<CMesh*>      m_Meshes;
    _uint               m_iNumMaterials = { 0 };
    vector<CMaterial*>  m_Materials;
    _float4x4           m_PreTransformMatrix = {};

protected:
    // 비애니메이션 메시 버퍼 생성 (CModel::TYPE_NONANIM 고정). CModel이 오버라이드함.
    virtual HRESULT Ready_Meshes(ifstream& inFile);
    HRESULT Ready_Materials(ifstream& inFile, const _char* pModelFilePath);
    // 파일 스트림에서 본 데이터를 읽고 즉시 버림 (비애니메이션 모델용)
    HRESULT Ready_Bones_Skip(ifstream& inFile, _int iParentBoneIndex);

public:
    virtual CComponent* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END
