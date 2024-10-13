#pragma once
#include "Transform.h"
#include "Client_Defines.h"

BEGIN(Client)
class CMyTransform :
    public CTransform
{
protected:
	explicit CMyTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMyTransform(const CMyTransform& Prototype);
	virtual ~CMyTransform() = default;

public:
	virtual void Go_Direction(DIRECTION eDirection, _float fTimeDelta);
	virtual void Look_At(DIRECTION eDirection);

public:
	static CMyTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END