#pragma once
#include "Character.h"
BEGIN(Client)
class CMonster :
    public CCharacter
{
protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:


};

END