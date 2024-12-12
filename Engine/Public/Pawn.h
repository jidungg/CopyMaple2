#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPawn :
    public CGameObject
{
protected:
	explicit CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual void Receive_KeyInput(_float fTimeDelta) {};
	void Set_Position(const _vector& vPos);
public:

};

END