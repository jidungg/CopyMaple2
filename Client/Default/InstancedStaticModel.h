#pragma once
#include "D:\Workbench\Projects\CopyMaple2\EngineSDK\Inc\Component.h"
class CInstancedStaticModel :
    public CComponent
{
    protected:
    CInstancedStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInstancedStaticModel(const CInstancedStaticModel& Prototype);
	virtual ~CInstancedStaticModel() = default;
public:
    virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
    public:
    virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

