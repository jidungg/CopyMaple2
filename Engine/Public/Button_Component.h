#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CButton_Component :
    public CComponent
{
protected:
	explicit CButton_Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CButton_Component(const CButton_Component& rhs);
	virtual ~CButton_Component() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CButton_Component* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END