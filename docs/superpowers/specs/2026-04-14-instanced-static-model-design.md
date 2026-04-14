# CInstancedStaticModel 설계 문서

**날짜:** 2026-04-14  
**작성자:** jiwan  

---

## 개요

DirectX 11 기반 커스텀 엔진에 GPU 인스턴싱 기능을 추가한다. 언리얼의 Instanced Static Mesh와 유사한 구조로, `CModel`과 공통 인터페이스를 공유하는 `CInstancedStaticModel`을 구현한다. 공통 추상 기반 클래스 `CBaseModel`을 신설하여 두 클래스가 이를 상속하는 구조를 취한다.

---

## 아키텍처

### 클래스 계층 구조

```
CComponent (Engine)
└── CBaseModel (Engine/Public/BaseModel.h) ← 신규
    ├── CModel (Engine/Public/Model.h) ← 기반 클래스 변경
    └── CInstancedStaticModel (Engine/Public/InstancedStaticModel.h) ← Engine으로 이동
```

기존 `Client/Default/InstancedStaticModel.h/.cpp`는 Engine으로 이동하며 Client 측 파일은 삭제한다.

---

## CBaseModel

**파일:** `Engine/Public/BaseModel.h`, `Engine/Private/BaseModel.cpp`

메시·머티리얼 데이터와 파일 로딩 로직을 소유하는 추상 기반 클래스.

### 공개 인터페이스

```cpp
class ENGINE_DLL CBaseModel : public CComponent
{
public:
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual HRESULT Render(_uint iMeshIndex) = 0;  // 순수 가상

public:
    HRESULT Bind_Material(CShader* pShader, const _char* pConstantName,
                          _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex = 0);
    _uint   Get_NumMeshes() const  { return m_iNumMeshes; }
    CMesh*  Get_Mesh(_uint iMeshIndex) const;
    bool    Is_MeshActive(_uint iIdx);
    void    Set_MeshActive(_uint iIdx, _bool bIsOn);

protected:
    _uint               m_iNumMeshes    = 0;
    vector<CMesh*>      m_Meshes;
    _uint               m_iNumMaterials = 0;
    vector<CMaterial*>  m_Materials;
    _float4x4           m_PreTransformMatrix = {};

protected:
    HRESULT Ready_Meshes(ifstream& inFile);
    HRESULT Ready_Materials(ifstream& inFile, const _char* pModelFilePath);
};
```

### CModel 변경 사항

- 상속 대상을 `CComponent` → `CBaseModel`로 변경
- `m_Meshes`, `m_Materials`, `m_PreTransformMatrix`, `Ready_Meshes()`, `Ready_Materials()`, `Bind_Material()`, `Get_NumMeshes()`, `Get_Mesh()`, `Is_MeshActive()`, `Set_MeshActive()` 를 `CBaseModel`로 이동
- 뼈·애니메이션 관련 멤버/메서드(`m_Bones`, `m_vecAnimation`, `Play_Animation()` 등)는 `CModel`에 유지

---

## CInstancedStaticModel

**파일:** `Engine/Public/InstancedStaticModel.h`, `Engine/Private/InstancedStaticModel.cpp`

### 인스턴스 데이터 구조체

```cpp
struct INSTANCE_DATA {
    _float4x4  WorldMatrix;    // 인스턴스 월드 변환 행렬
    _uint      iTextureIndex;  // Texture2DArray 슬롯 인덱스
    _float3    vPadding;       // 16바이트 정렬용 패딩
};
```

### 공개 인터페이스

```cpp
class ENGINE_DLL CInstancedStaticModel : public CBaseModel
{
public:
    // 파일로부터 메시·머티리얼 로드 (CBaseModel::Initialize_Prototype 호출)
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath,
                                         _fmatrix PreTransformMatrix) override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render(_uint iMeshIndex) override;  // DrawIndexedInstanced

public:
    // Texture2DArray SRV 등록 (소유권은 호출자가 관리, CInstancedStaticModel은 AddRef만 수행)
    HRESULT Set_TextureArray(ID3D11ShaderResourceView* pSRV);

    // 인스턴스 일괄 교체
    HRESULT Set_Instances(const vector<INSTANCE_DATA>& Instances);

    // 인스턴스 개별 조작
    _uint Add_Instance(const _float4x4& Transform, _uint iTextureIndex = 0);
    void  Update_Instance(_uint iIdx, const _float4x4& Transform, _uint iTextureIndex);
    void  Remove_Instance(_uint iIdx);
    _uint Get_InstanceCount() const;

private:
    vector<INSTANCE_DATA>       m_InstanceData;
    ID3D11Buffer*               m_pInstanceBuffer   = nullptr;
    ID3D11ShaderResourceView*   m_pTextureArraySRV  = nullptr;
    _uint                       m_iMaxInstanceCount = 0;
    bool                        m_bDirty            = false;

private:
    HRESULT Create_InstanceBuffer(_uint iCapacity);
    HRESULT Update_InstanceBuffer();  // m_bDirty == true일 때 GPU 업로드

public:
    static CInstancedStaticModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                                         const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};
```

### 인스턴스 버퍼 관리 전략

- `Add_Instance()` / `Set_Instances()` / `Remove_Instance()` 호출 시 `m_bDirty = true` 설정
- `Render()` 호출 시 `m_bDirty`이면 `Update_InstanceBuffer()` 실행 후 GPU 업로드
- 인스턴스 수가 `m_iMaxInstanceCount` 초과 시 버퍼를 ×2 크기로 재할당 (`D3D11_USAGE_DYNAMIC`)
- `Remove_Instance()`는 swap-and-pop 방식으로 O(1) 삭제 (순서 보장 불필요)

---

## 셰이더

**파일:** `Client/Bin/ShaderFiles/Shader_VtxInstancedMesh.hlsl`  
**프로토타입 이름:** `Prototype_Component_Shader_VtxInstancedMesh`

### 입력 레이아웃

| 슬롯 | 데이터 | 스텝 |
|------|--------|------|
| 0 | `POSITION`, `NORMAL`, `TEXCOORD0`, `TANGENT` | Per-Vertex |
| 1 | `WORLD`(float4x4), `TEXCOORD1`(uint TextureIndex) | Per-Instance |

```hlsl
// Vertex Input
struct VS_IN {
    float3             vPosition    : POSITION;
    float3             vNormal      : NORMAL;
    float2             vTexcoord    : TEXCOORD0;
    float3             vTangent     : TANGENT;
    // Instance data (slot 1)
    row_major float4x4 vWorldMatrix : WORLD;
    uint               iTexIndex    : TEXCOORD1;
};

Texture2DArray g_TextureArray;

float4 color = g_TextureArray.Sample(sampler0, float3(In.vTexcoord, (float)In.iTexIndex));
```

### D3D11_INPUT_ELEMENT_DESC

슬롯 1의 요소들은 `InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA`, `InstanceDataStepRate = 1`로 설정.

---

## 렌더링 흐름

```
GameObject::Render()
  └─ pShader->Bind_Matrix("g_ViewMatrix", ...)
  └─ pShader->Bind_Matrix("g_ProjMatrix", ...)
  └─ pShader->Bind_SRV("g_TextureArray", m_pInstancedModel->Get_TextureArraySRV())
  └─ for each mesh:
       pInstancedModel->Bind_Material(...)   // 머티리얼 바인딩 (선택적)
       pShader->Begin(0)
       pInstancedModel->Render(iMeshIndex)   // DrawIndexedInstanced 호출
```

`Render(_uint iMeshIndex)` 내부:
1. `m_bDirty`이면 `Update_InstanceBuffer()` 호출
2. `IASetVertexBuffers`로 VB(슬롯0) + InstanceBuffer(슬롯1) 동시 바인딩
3. `DrawIndexedInstanced(iIndexCount, iInstanceCount, 0, 0, 0)`

---

## 파일 변경 목록

| 작업 | 파일 |
|------|------|
| 신규 생성 | `Engine/Public/BaseModel.h` |
| 신규 생성 | `Engine/Private/BaseModel.cpp` |
| 신규 생성 | `Engine/Public/InstancedStaticModel.h` |
| 신규 생성 | `Engine/Private/InstancedStaticModel.cpp` |
| 신규 생성 | `Client/Bin/ShaderFiles/Shader_VtxInstancedMesh.hlsl` |
| 수정 | `Engine/Public/Model.h` — `CBaseModel` 상속으로 변경, 공유 멤버 제거 |
| 수정 | `Engine/Private/Model.cpp` — 이동된 메서드 제거 |
| 수정 | `Engine/CopyMaple2Engine.vcxproj` — 신규 파일 추가 |
| 삭제 | `Client/Default/InstancedStaticModel.h` |
| 삭제 | `Client/Default/InstancedStaticModel.cpp` |
| 수정 | `Client/CopyMaple2Client.vcxproj` — 삭제된 파일 제거 |
