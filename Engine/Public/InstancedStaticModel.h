#pragma once
#include "BaseModel.h"

BEGIN(Engine)

// GPU 인스턴싱 전용 정적 메시 컴포넌트.
// CBaseModel에서 메시/머티리얼을 상속하고, DrawIndexedInstanced로 한 번에 렌더링.
class ENGINE_DLL CInstancedStaticModel final : public CBaseModel
{
public:
    typedef struct
    {
        _uint iMaxInstances; // 인스턴스 버퍼 최대 슬롯 수 (Clone/Initialize 시 전달)
    } INSTANCED_MODEL_DESC;

protected:
    CInstancedStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInstancedStaticModel(const CInstancedStaticModel& Prototype);
    virtual ~CInstancedStaticModel() = default;

public:
    // 비애니메이션 모델 파일 로드 (CBaseModel::Initialize_Prototype 위임)
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix) override;
    // pArg: INSTANCED_MODEL_DESC* (nullptr이면 iMaxInstances=1000)
    virtual HRESULT Initialize(void* pArg) override;
    // 슬롯 0 메시 VB + 슬롯 1 인스턴스 VB → DrawIndexedInstanced
    virtual HRESULT Render(_uint iMeshIndex) override;

public:
    // 매 프레임 렌더 직전에 호출. 인스턴스 데이터를 GPU 버퍼에 업로드.
    HRESULT Set_Instances(const vector<INSTANCE_DATA>& Instances);
    _uint   Get_NumInstances() const { return m_iNumInstances; }

private:
    ID3D11Buffer*   m_pInstanceBuffer = { nullptr };
    _uint           m_iMaxInstances   = { 0 };
    _uint           m_iNumInstances   = { 0 };

    HRESULT Create_InstanceBuffer(_uint iMaxInstances);

public:
    static CInstancedStaticModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                                         const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
