# CInstancedStaticModel 구현 플랜

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `CBaseModel` 추상 기반 클래스를 신설하고, `CModel`과 `CInstancedStaticModel`이 이를 공유 상속함으로써 GPU 인스턴싱을 지원하는 구조를 Engine DLL에 구현한다.

**Architecture:** `CBaseModel`(abstract)이 메시·머티리얼 데이터와 로딩 헬퍼를 소유. `CModel`은 뼈·애니메이션을 추가하고, `CInstancedStaticModel`은 D3D11 Dynamic 인스턴스 버퍼와 Texture2DArray를 추가. 인스턴스 당 월드 행렬과 텍스처 인덱스를 `INSTANCE_DATA` 구조체로 GPU에 전달. `Render()`는 `DrawIndexedInstanced`로 한 번의 드로우콜에 N개를 그린다.

**Tech Stack:** DirectX 11, Effects11 (FX 셰이더), C++17, HLSL 5.0, msbuild

---

## 파일 변경 목록

| 작업 | 파일 |
|------|------|
| 신규 | `Engine/Public/BaseModel.h` |
| 신규 | `Engine/Private/BaseModel.cpp` |
| 신규 | `Engine/Public/InstancedStaticModel.h` |
| 신규 | `Engine/Private/InstancedStaticModel.cpp` |
| 신규 | `Client/Bin/ShaderFiles/Shader_VtxInstancedMesh.hlsl` |
| 수정 | `Engine/Public/Engine_Struct.h` — `INSTANCE_DATA` 구조체 추가 |
| 수정 | `Engine/Public/Engine_Struct.cpp` — `INSTANCE_DATA::Elements` 추가 |
| 수정 | `Engine/Public/Model.h` — `CBaseModel` 상속, 공유 멤버 제거 |
| 수정 | `Engine/Private/Model.cpp` — 이동된 멤버/메서드 제거 |
| 수정 | `Engine/Default/Engine.vcxproj` — 신규 파일 등록 |
| 수정 | `Client/Private/Loader.cpp` — 셰이더 프로토타입 등록 |
| 삭제 | `Client/Default/InstancedStaticModel.h` |
| 삭제 | `Client/Default/InstancedStaticModel.cpp` |
| 수정 | `Client/Default/Client.vcxproj` — 삭제된 파일 제거 |

---

## Task 1: INSTANCE_DATA 구조체 추가

**Files:**
- Modify: `Engine/Public/Engine_Struct.h`
- Modify: `Engine/Public/Engine_Struct.cpp`

- [ ] **Step 1: Engine_Struct.h에 INSTANCE_DATA 추가**

`VTXANIMMESH` 구조체 정의 아래(`}VTXANIMMESH;` 다음 줄)에 다음을 삽입:

```cpp
typedef struct InstanceData
{
    XMFLOAT4X4  WorldMatrix;      // 인스턴스 월드 변환 행렬 (64 bytes)
    _uint       iTextureIndex;    // Texture2DArray 슬롯 인덱스 (4 bytes)
    XMFLOAT3    vPadding;         // 16바이트 정렬 패딩 (12 bytes)

    static const unsigned int           iNumElements = { 5 };
    static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
} INSTANCE_DATA;
```

- [ ] **Step 2: Engine_Struct.cpp에 INSTANCE_DATA::Elements 추가**

`VTXANIMMESH::Elements[]` 정의 바로 다음에 삽입:

```cpp
const D3D11_INPUT_ELEMENT_DESC INSTANCE_DATA::Elements[] = {
    // WorldMatrix는 float4x4이므로 WORLD 시맨틱 0~3에 각 행을 매핑
    { "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    // TextureIndex: offset 64 (WorldMatrix 64bytes 이후)
    { "TEXCOORD", 1, DXGI_FORMAT_R32_UINT,         1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Public/Engine_Struct.h Engine/Public/Engine_Struct.cpp
git commit -m "feat: INSTANCE_DATA 구조체 추가 (인스턴싱용 per-instance 버퍼 레이아웃)"
```

---

## Task 2: CBaseModel 신설

**Files:**
- Create: `Engine/Public/BaseModel.h`
- Create: `Engine/Private/BaseModel.cpp`

- [ ] **Step 1: Engine/Public/BaseModel.h 생성**

```cpp
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
    virtual HRESULT Render(_uint iMeshIndex) = 0;

public:
    // 공유 인터페이스
    HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName,
                          _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex = 0);
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
```

- [ ] **Step 2: Engine/Private/BaseModel.cpp 생성**

```cpp
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
    return m_Meshes[iIdx]->Is_Active();
}

void CBaseModel::Set_MeshActive(_uint iIdx, _bool bIsOn)
{
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
        if (FAILED(Ready_Bones_Skip(inFile, 0)))
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
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Public/BaseModel.h Engine/Private/BaseModel.cpp
git commit -m "feat: CBaseModel 추상 기반 클래스 신설 (메시·머티리얼 공유 로직)"
```

---

## Task 3: Engine.vcxproj에 CBaseModel 등록

**Files:**
- Modify: `Engine/Default/Engine.vcxproj`

- [ ] **Step 1: ClInclude 항목 추가**

`Engine/Default/Engine.vcxproj`에서 다음 줄을 찾는다:
```xml
    <ClInclude Include="..\Public\Model.h" />
```
그 바로 앞에 삽입:
```xml
    <ClInclude Include="..\Public\BaseModel.h" />
```

- [ ] **Step 2: ClCompile 항목 추가**

같은 파일에서 다음 줄을 찾는다:
```xml
    <ClCompile Include="..\Private\Model.cpp" />
```
그 바로 앞에 삽입:
```xml
    <ClCompile Include="..\Private\BaseModel.cpp" />
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Default/Engine.vcxproj
git commit -m "build: Engine.vcxproj에 BaseModel 파일 등록"
```

---

## Task 4: CModel 리팩토링 (CBaseModel 상속)

**Files:**
- Modify: `Engine/Public/Model.h`
- Modify: `Engine/Private/Model.cpp`

- [ ] **Step 1: Model.h — 상속 변경 및 공유 멤버 제거**

`Model.h` 전체를 아래로 교체한다:

```cpp
#pragma once

#include "BaseModel.h"

BEGIN(Engine)
class CEvent;

class ENGINE_DLL CModel : public CBaseModel
{
public:
    enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_MIMIC, TYPE_END };
    typedef struct ModelDesc
    {
        const CModel* pMimicTarget = { nullptr };
    }MODEL_DESC;
protected:
    CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CModel(const CModel& Prototype);
    virtual ~CModel() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix) override;
    virtual HRESULT Initialize_Prototype(TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render(_uint iMeshIndex) override;

public:
    HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

    _bool Play_Animation(_float fTimeDelta);

    _uint Get_MeshIndex(const _char* szName) const;
    _uint Get_BoneIndex(const _char* pBoneName) const;
    float Get_AnimTime();
    _uint Get_AnimIndex();
    _float Get_AnimationProgress(_uint iAnimIdx);
    TYPE Get_Type() { return m_eModelType; }
    const _float4x4* Get_BoneMatrix(const _char* pBoneName) const;
    class CBone* Get_Bone(const _char* pBoneName) const;
    bool Is_AnimChangeable();

    void Set_AnimationLoop(_uint iIdx, _bool bIsLoop);
    void Set_Animation(_uint iIdx);
    void Set_AnimPostDelayPercent(_uint iIdx, _float fPercent);
    void Switch_Animation(_uint iIdx);
    void Register_AnimEvent(_uint iAnimIdx, ANIM_EVENT tAnimEvent);

protected:
    TYPE                        m_eModelType = { TYPE_END };
    vector<class CBone*>        m_Bones;
    _uint                       m_iCurrentAnimIndex = {};
    _uint                       m_iPrevAnimIndex    = {};
    _uint                       m_iNumAnimations    = {};
    vector<class CAnimation*>   m_vecAnimation;
    map<_uint, TRANSFORM_KEYFRAME> m_mapAnimTransLeftFrame;

protected:
    // CBaseModel::Ready_Meshes를 오버라이드: m_eModelType과 this 전달
    HRESULT Ready_Meshes(ifstream& inFile) override;
    HRESULT Ready_Bones(ifstream& inFile, _uint iParentBoneIndex);
    HRESULT Ready_Animations(ifstream& inFile);

public:
    static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                          const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                          CModel::TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
```

> **주의:** `Get_NumMeshes`, `Get_Mesh`, `Is_MeshActive`, `Set_MeshActive`, `Bind_Material` 는 `CBaseModel`로 이동했으므로 `Model.h`에서 삭제.
> `m_iNumMeshes`, `m_Meshes`, `m_iNumMaterials`, `m_Materials`, `m_PreTransformMatrix` 도 `CBaseModel`로 이동.

- [ ] **Step 2: Model.cpp — 이동된 멤버 제거, Ready_Meshes 오버라이드 유지**

Model.cpp에서 아래 항목을 제거/수정한다:

**(a) 복사 생성자** — `m_Meshes`, `m_Materials`, `m_PreTransformMatrix` 관련 초기화를 제거. `__super` (CBaseModel) 복사 생성자가 처리:
```cpp
CModel::CModel(const CModel& Prototype)
    : CBaseModel{ Prototype }          // ← CComponent 에서 CBaseModel 로 변경
    , m_eModelType{ Prototype.m_eModelType }
    , m_iNumAnimations{ Prototype.m_iNumAnimations }
{
    for (auto& pPrototypeBone : Prototype.m_Bones)
        m_Bones.push_back(pPrototypeBone->Clone());
    for (auto& pPrototypeAnimation : Prototype.m_vecAnimation)
        m_vecAnimation.push_back(pPrototypeAnimation->Clone());
}
```

**(b) Initialize_Prototype (기존 동일)** — 변경 없음. `Ready_Meshes` 호출 시 가상 디스패치로 `CModel::Ready_Meshes` 가 호출됨.

**(c) CModel::Ready_Meshes 오버라이드 추가** — 기존 `Ready_Meshes` 이름은 유지, `eModelType`과 `this` 전달:
```cpp
HRESULT CModel::Ready_Meshes(ifstream& inFile)
{
    inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext,
                                     m_eModelType, this,
                                     inFile, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;
        m_Meshes.push_back(pMesh);
    }
    return S_OK;
}
```

**(d) Bind_Material, Is_MeshActive, Set_MeshActive 메서드 정의 삭제** — CBaseModel로 이동됨.

**(e) Free()** — `m_Materials`, `m_Meshes` 해제를 제거. `CBaseModel::Free()`가 처리. 뼈·애니메이션만 남김:
```cpp
void CModel::Free()
{
    __super::Free(); // CBaseModel::Free()가 Meshes/Materials 해제
    for (auto& pBone : m_Bones)
        Safe_Release(pBone);
    m_Bones.clear();
    for (auto& pAnim : m_vecAnimation)
        Safe_Release(pAnim);
    m_vecAnimation.clear();
}
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Public/Model.h Engine/Private/Model.cpp
git commit -m "refactor: CModel이 CBaseModel을 상속하도록 리팩토링"
```

---

## Task 5: Engine 빌드 검증 #1

**Files:** 없음 (빌드만)

- [ ] **Step 1: Engine 단독 빌드**

```bash
msbuild Engine/Default/Engine.vcxproj /p:Configuration=Debug /p:Platform=x64 /v:minimal
```

Expected: `Build succeeded.` (에러 0개)

만약 에러가 발생하면:
- `'CModel' is not a member of 'Engine'` 류 → Model.h의 `BEGIN(Engine)` 매크로 확인
- `'m_Meshes' undeclared` → Model.cpp에서 `m_Meshes`를 참조하는 코드가 있다면 `CBaseModel`에서 상속받으므로 그대로 사용 가능 (protected)
- `'Bind_Material' not a member of CModel` → Client 코드에서 `CModel*`로 호출 시 `CBaseModel*`로 업캐스트하거나 다형성으로 해결됨

- [ ] **Step 2: 커밋 (변경 없음이면 생략)**

---

## Task 6: CInstancedStaticModel Engine 구현

**Files:**
- Create: `Engine/Public/InstancedStaticModel.h`
- Create: `Engine/Private/InstancedStaticModel.cpp`

- [ ] **Step 1: Engine/Public/InstancedStaticModel.h 생성**

```cpp
#pragma once
#include "BaseModel.h"

BEGIN(Engine)

class ENGINE_DLL CInstancedStaticModel final : public CBaseModel
{
protected:
    CInstancedStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInstancedStaticModel(const CInstancedStaticModel& Prototype);
    virtual ~CInstancedStaticModel() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath,
                                         _fmatrix PreTransformMatrix) override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render(_uint iMeshIndex) override; // DrawIndexedInstanced

public:
    // Texture2DArray SRV 등록 (소유권은 호출자, 내부에서 AddRef)
    HRESULT Set_TextureArray(ID3D11ShaderResourceView* pSRV);
    ID3D11ShaderResourceView* Get_TextureArraySRV() const { return m_pTextureArraySRV; }

    // 인스턴스 일괄 교체
    HRESULT Set_Instances(const vector<INSTANCE_DATA>& Instances);

    // 인스턴스 개별 조작
    _uint Add_Instance(const _float4x4& Transform, _uint iTextureIndex = 0);
    void  Update_Instance(_uint iIdx, const _float4x4& Transform, _uint iTextureIndex);
    void  Remove_Instance(_uint iIdx); // swap-and-pop, O(1), 순서 불보장
    _uint Get_InstanceCount() const { return (_uint)m_InstanceData.size(); }

private:
    vector<INSTANCE_DATA>       m_InstanceData;
    ID3D11Buffer*               m_pInstanceBuffer   = { nullptr };
    ID3D11ShaderResourceView*   m_pTextureArraySRV  = { nullptr };
    _uint                       m_iMaxInstanceCount = { 0 };
    bool                        m_bDirty            = { false };

private:
    HRESULT Create_InstanceBuffer(_uint iCapacity);
    HRESULT Update_InstanceBuffer();

public:
    static CInstancedStaticModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                                         const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
```

- [ ] **Step 2: Engine/Private/InstancedStaticModel.cpp 생성**

```cpp
#include "..\Public\InstancedStaticModel.h"
#include "Mesh.h"     // CMesh, Get_VertexBuffer, Get_IndexBuffer, Get_IndexCount
#include "Shader.h"

CInstancedStaticModel::CInstancedStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBaseModel{ pDevice, pContext }
{
}

CInstancedStaticModel::CInstancedStaticModel(const CInstancedStaticModel& Prototype)
    : CBaseModel{ Prototype }
    , m_iMaxInstanceCount{ Prototype.m_iMaxInstanceCount }
{
    // 인스턴스 데이터와 버퍼는 복사하지 않음 (Clone 후 Set_Instances로 채움)
    if (m_pTextureArraySRV)
        m_pTextureArraySRV->AddRef();
}

HRESULT CInstancedStaticModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    // CBaseModel::Initialize_Prototype: 뼈 스킵 후 메시·머티리얼 로드
    return __super::Initialize_Prototype(pModelFilePath, PreTransformMatrix);
}

HRESULT CInstancedStaticModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CInstancedStaticModel::Render(_uint iMeshIndex)
{
    if (m_InstanceData.empty())
        return S_OK;

    if (m_bDirty)
    {
        if (FAILED(Update_InstanceBuffer()))
            return E_FAIL;
    }

    CMesh* pMesh = m_Meshes[iMeshIndex];

    // 슬롯 0: 메시 버텍스 버퍼 (per-vertex VTXMESH)
    // 슬롯 1: 인스턴스 버퍼 (per-instance INSTANCE_DATA)
    ID3D11Buffer* pVBs[2]   = { pMesh->Get_VertexBuffer(), m_pInstanceBuffer };
    _uint         strides[2] = { sizeof(VTXMESH), sizeof(INSTANCE_DATA) };
    _uint         offsets[2] = { 0, 0 };

    m_pContext->IASetVertexBuffers(0, 2, pVBs, strides, offsets);
    m_pContext->IASetIndexBuffer(pMesh->Get_IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pContext->DrawIndexedInstanced(pMesh->Get_IndexCount(),
                                     (_uint)m_InstanceData.size(), 0, 0, 0);
    return S_OK;
}

HRESULT CInstancedStaticModel::Set_TextureArray(ID3D11ShaderResourceView* pSRV)
{
    Safe_Release(m_pTextureArraySRV);
    m_pTextureArraySRV = pSRV;
    if (m_pTextureArraySRV)
        m_pTextureArraySRV->AddRef();
    return S_OK;
}

HRESULT CInstancedStaticModel::Set_Instances(const vector<INSTANCE_DATA>& Instances)
{
    m_InstanceData = Instances;
    m_bDirty = true;
    return S_OK;
}

_uint CInstancedStaticModel::Add_Instance(const _float4x4& Transform, _uint iTextureIndex)
{
    INSTANCE_DATA data;
    data.WorldMatrix    = Transform;
    data.iTextureIndex  = iTextureIndex;
    data.vPadding       = { 0.f, 0.f, 0.f };
    m_InstanceData.push_back(data);
    m_bDirty = true;
    return (_uint)m_InstanceData.size() - 1;
}

void CInstancedStaticModel::Update_Instance(_uint iIdx, const _float4x4& Transform, _uint iTextureIndex)
{
    if (iIdx >= m_InstanceData.size()) return;
    m_InstanceData[iIdx].WorldMatrix   = Transform;
    m_InstanceData[iIdx].iTextureIndex = iTextureIndex;
    m_bDirty = true;
}

void CInstancedStaticModel::Remove_Instance(_uint iIdx)
{
    if (iIdx >= (_uint)m_InstanceData.size()) return;
    m_InstanceData[iIdx] = m_InstanceData.back(); // swap-and-pop
    m_InstanceData.pop_back();
    m_bDirty = true;
}

HRESULT CInstancedStaticModel::Create_InstanceBuffer(_uint iCapacity)
{
    Safe_Release(m_pInstanceBuffer);
    m_iMaxInstanceCount = iCapacity;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth      = sizeof(INSTANCE_DATA) * iCapacity;
    desc.Usage          = D3D11_USAGE_DYNAMIC;
    desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pInstanceBuffer)))
        return E_FAIL;
    return S_OK;
}

HRESULT CInstancedStaticModel::Update_InstanceBuffer()
{
    _uint iCount = (_uint)m_InstanceData.size();

    // 용량 초과 시 ×2 성장
    if (iCount > m_iMaxInstanceCount)
    {
        _uint iNewCap = max(iCount, m_iMaxInstanceCount * 2);
        if (iNewCap == 0) iNewCap = 16; // 최초 할당
        if (FAILED(Create_InstanceBuffer(iNewCap)))
            return E_FAIL;
    }

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (FAILED(m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        return E_FAIL;

    memcpy(mapped.pData, m_InstanceData.data(), sizeof(INSTANCE_DATA) * iCount);
    m_pContext->Unmap(m_pInstanceBuffer, 0);

    m_bDirty = false;
    return S_OK;
}

CInstancedStaticModel* CInstancedStaticModel::Create(ID3D11Device* pDevice,
                                                      ID3D11DeviceContext* pContext,
                                                      const _char* pModelFilePath,
                                                      _fmatrix PreTransformMatrix)
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
    __super::Free(); // CBaseModel::Free() → Meshes/Materials 해제
    Safe_Release(m_pInstanceBuffer);
    Safe_Release(m_pTextureArraySRV);
}
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Public/InstancedStaticModel.h Engine/Private/InstancedStaticModel.cpp
git commit -m "feat: CInstancedStaticModel Engine 구현 (DrawIndexedInstanced, DYNAMIC 인스턴스 버퍼)"
```

---

## Task 7: Engine.vcxproj에 CInstancedStaticModel 등록

**Files:**
- Modify: `Engine/Default/Engine.vcxproj`

- [ ] **Step 1: ClInclude 항목 추가**

파일에서 다음 줄을 찾는다:
```xml
    <ClInclude Include="..\Public\BaseModel.h" />
```
그 바로 다음에 삽입:
```xml
    <ClInclude Include="..\Public\InstancedStaticModel.h" />
```

- [ ] **Step 2: ClCompile 항목 추가**

파일에서 다음 줄을 찾는다:
```xml
    <ClCompile Include="..\Private\BaseModel.cpp" />
```
그 바로 다음에 삽입:
```xml
    <ClCompile Include="..\Private\InstancedStaticModel.cpp" />
```

- [ ] **Step 3: 커밋**

```bash
git add Engine/Default/Engine.vcxproj
git commit -m "build: Engine.vcxproj에 InstancedStaticModel 파일 등록"
```

---

## Task 8: Engine 빌드 검증 #2

- [ ] **Step 1: Engine 전체 빌드**

```bash
msbuild Engine/Default/Engine.vcxproj /p:Configuration=Debug /p:Platform=x64 /v:minimal
```

Expected: `Build succeeded.` (에러 0개)

자주 발생하는 에러:
- `'INSTANCE_DATA': undeclared identifier` → InstancedStaticModel.cpp에서 `Engine_Struct.h` 포함 경로 확인. `BaseModel.h` → `Component.h` → `Engine_Defines.h`를 통해 포함됐는지 확인.
- `'VTXMESH': undeclared identifier` → 동일 경로 확인.
- `DrawIndexedInstanced` 미정의 → `m_pContext`가 `ID3D11DeviceContext*`인지 확인.

- [ ] **Step 2: UpdateLib.bat 실행 (EngineSDK 동기화)**

```bash
cd D:/Workbench/Projects/CopyMaple2
./UpdateLib.bat
```

이 명령은 `Engine/Public/*.h` → `EngineSDK/Inc/`로 복사. `BaseModel.h`, `InstancedStaticModel.h`가 `EngineSDK/Inc/`에 생겼는지 확인:
```bash
ls EngineSDK/Inc/BaseModel.h EngineSDK/Inc/InstancedStaticModel.h
```

- [ ] **Step 3: 커밋**

```bash
git add EngineSDK/
git commit -m "build: UpdateLib.bat 실행 — EngineSDK에 BaseModel, InstancedStaticModel 헤더 동기화"
```

---

## Task 9: Client에서 구 CInstancedStaticModel 제거

**Files:**
- Delete: `Client/Default/InstancedStaticModel.h`
- Delete: `Client/Default/InstancedStaticModel.cpp`
- Modify: `Client/Default/Client.vcxproj`

- [ ] **Step 1: 파일 삭제**

```bash
rm Client/Default/InstancedStaticModel.h
rm Client/Default/InstancedStaticModel.cpp
```

- [ ] **Step 2: Client.vcxproj에서 항목 제거**

`Client/Default/Client.vcxproj`에서 다음 두 줄을 찾아 삭제:
```xml
    <ClInclude Include="InstancedStaticModel.h" />
```
```xml
    <ClCompile Include="InstancedStaticModel.cpp" />
```

- [ ] **Step 3: 커밋**

```bash
git add Client/Default/InstancedStaticModel.h Client/Default/InstancedStaticModel.cpp Client/Default/Client.vcxproj
git commit -m "chore: Client에서 구 CInstancedStaticModel 제거 (Engine으로 이동)"
```

---

## Task 10: Shader_VtxInstancedMesh.hlsl 작성

**Files:**
- Create: `Client/Bin/ShaderFiles/Shader_VtxInstancedMesh.hlsl`

- [ ] **Step 1: 셰이더 파일 생성**

```hlsl
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

// cbuffer: 뷰/투영 행렬 (월드는 per-instance로 전달)
float4x4 g_ViewMatrix, g_ProjMatrix;

// Texture2DArray: iTextureIndex 로 슬라이스 선택
Texture2DArray g_TextureArray;

// ─────────────────────────────────────────────────────
// Vertex Shader
// ─────────────────────────────────────────────────────
struct VS_IN
{
    // Slot 0: per-vertex (VTXMESH)
    float3             vPosition    : POSITION;
    float3             vNormal      : NORMAL;
    float2             vTexcoord    : TEXCOORD0;
    float3             vTangent     : TANGENT;
    // Slot 1: per-instance (INSTANCE_DATA)
    row_major float4x4 vWorldMatrix : WORLD;      // WORLD0~3
    uint               iTexIndex    : TEXCOORD1;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos  : TEXCOORD1;
    uint   iTexIndex : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    matrix matWV  = mul(In.vWorldMatrix, g_ViewMatrix);
    matrix matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal   = normalize(mul(float4(In.vNormal, 0.f), In.vWorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos  = Out.vPosition;
    Out.iTexIndex = In.iTexIndex;
    return Out;
}

// ─────────────────────────────────────────────────────
// Pixel Shader — Deferred GBuffer 출력
// ─────────────────────────────────────────────────────
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos  : TEXCOORD1;
    uint   iTexIndex : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal  : SV_TARGET1;
    float4 vDepth   : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vDiffuse = g_TextureArray.Sample(LinearSampler, float3(In.vTexcoord, (float)In.iTexIndex));
    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth  = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    return Out;
}

// ─────────────────────────────────────────────────────
technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader   = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN();
    }
}
```

- [ ] **Step 2: Client.vcxproj에 FxCompile 항목 추가 (선택)**

기존 셰이더 항목 패턴:
```xml
    <FxCompile Include="..\Bin\ShaderFiles\Shader_VtxMesh.hlsl" />
```
동일 패턴으로 삽입:
```xml
    <FxCompile Include="..\Bin\ShaderFiles\Shader_VtxInstancedMesh.hlsl" />
```

- [ ] **Step 3: 커밋**

```bash
git add Client/Bin/ShaderFiles/Shader_VtxInstancedMesh.hlsl Client/Default/Client.vcxproj
git commit -m "feat: Shader_VtxInstancedMesh.hlsl 추가 (per-instance WorldMatrix + Texture2DArray)"
```

---

## Task 11: Loader.cpp에 셰이더 프로토타입 등록

**Files:**
- Modify: `Client/Private/Loader.cpp`

- [ ] **Step 1: 인스턴싱 셰이더 입력 레이아웃 결합 및 등록**

`Client/Private/Loader.cpp`에서 다음 블록을 찾는다:
```cpp
/* For.Prototype_Component_Shader_VtxMesh*/
if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxMesh"),
    CShader::Create(..., VTXMESH::Elements, VTXMESH::iNumElements))))
    return E_FAIL;
```

그 바로 다음에 삽입:

```cpp
/* For.Prototype_Component_Shader_VtxInstancedMesh */
// VTXMESH(slot 0, 4 elements) + INSTANCE_DATA(slot 1, 5 elements) 결합
{
    const _uint iTotal = VTXMESH::iNumElements + INSTANCE_DATA::iNumElements;
    D3D11_INPUT_ELEMENT_DESC InstancedMeshElements[iTotal];
    memcpy(InstancedMeshElements,
           VTXMESH::Elements,
           sizeof(D3D11_INPUT_ELEMENT_DESC) * VTXMESH::iNumElements);
    memcpy(InstancedMeshElements + VTXMESH::iNumElements,
           INSTANCE_DATA::Elements,
           sizeof(D3D11_INPUT_ELEMENT_DESC) * INSTANCE_DATA::iNumElements);

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING,
        TEXT("Prototype_Component_Shader_VtxInstancedMesh"),
        CShader::Create(m_pDevice, m_pContext,
                        TEXT("../Bin/shaderFiles/Shader_VtxInstancedMesh.hlsl"),
                        InstancedMeshElements, iTotal))))
        return E_FAIL;
}
```

> `INSTANCE_DATA`를 Loader.cpp에서 사용하려면 `EngineSDK/Inc/Engine_Struct.h`가 포함된 헤더(stdafx.h 또는 GameInstance.h 경유)를 통해 이미 접근 가능한지 확인. 필요 시 `#include "Engine_Struct.h"` 추가.

- [ ] **Step 2: 커밋**

```bash
git add Client/Private/Loader.cpp
git commit -m "feat: Loader에 Prototype_Component_Shader_VtxInstancedMesh 등록"
```

---

## Task 12: Client 빌드 검증 (최종)

- [ ] **Step 1: 솔루션 전체 빌드**

```bash
msbuild CopyMaple2.sln /p:Configuration=Debug /p:Platform=x64 /v:minimal
```

Expected: `Build succeeded.` (에러 0개)

자주 발생하는 에러:
- `'INSTANCE_DATA': undeclared identifier` in Loader.cpp → `Engine_Struct.h` 포함 경로 확인
- `'CInstancedStaticModel': is not a class or namespace` → `EngineSDK/Inc/InstancedStaticModel.h`가 없으면 UpdateLib.bat 재실행
- `error X3000` (HLSL 셰이더 컴파일) → 셰이더 파일의 `#include` 경로가 `../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli`인지 확인

- [ ] **Step 2: 런타임 스모크 테스트**

다음 코드로 `CInstancedStaticModel`을 간단히 검증:
```cpp
// 임의의 레벨 Initialize()에서
auto* pModel = CInstancedStaticModel::Create(
    m_pDevice, m_pContext,
    "../Bin/Resources/FBXs/NonAnim/EmptyModel.model",
    XMMatrixIdentity());

if (pModel)
{
    _float4x4 world;
    XMStoreFloat4x4(&world, XMMatrixTranslation(0.f, 0.f, 5.f));
    pModel->Add_Instance(world, 0);  // 인스턴스 1개 추가

    // 프로토타입으로 등록
    m_pGameInstance->Add_Prototype(LEVEL_LOADING,
        TEXT("Prototype_Component_InstancedModel_Test"), pModel);
}
```
실행 후 크래시 없이 시작되면 통과.

- [ ] **Step 3: 최종 커밋**

```bash
git add -A
git commit -m "feat: CInstancedStaticModel 구현 완료 — GPU 인스턴싱, CBaseModel 공유 인터페이스"
```
