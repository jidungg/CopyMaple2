#pragma once
#include "Client_Defines.h"
#include "Pawn.h"

BEGIN(Engine)
class CCamera_Trace;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayer :
	public CPawn
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{

	}PLAYER_DESC;

protected:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void Receive_KeyInput(KEY eKey, KEY_STATE eKeyState, _float fTimeDelta) override;
private:


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END