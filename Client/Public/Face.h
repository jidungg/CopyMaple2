#pragma once
#include "Component.h"
#include "Texture.h"

BEGIN(Client)
class CFace :
    public CComponent
{
public:
	enum class FACE_STATE
	{
		IDLE = 0,
		TALK,
		ATTACK,
		FOCUS,
		PAIN,
		LAST = 5
	};
	enum class FACE_SUBSTATE
	{
		NORMAL,
		BLINK1,
		BLINK2,
		BLINK3,
		LAST
	};
private:
	explicit CFace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFace(const CFace& Prototype);
	virtual ~CFace() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pTexDirPath);

	void Update(_float fTimeDelta)override;

	void Set_State(FACE_STATE eState) { m_eState = eState; }
	HRESULT Bind_FaceShaderResource(class CShader* pShader, const _char* pConstantName);

private:
	HRESULT Ready_Face(const _tchar* pTexDirPath, const _tchar* pSubDirPath, FACE_STATE eState);
private:
	CTexture* m_pTexture[(_uint)FACE_STATE::LAST];
	CTexture* m_pTexture_C[(_uint)FACE_STATE::LAST];

	FACE_STATE m_eState = FACE_STATE::IDLE;
	FACE_SUBSTATE m_eSubState = FACE_SUBSTATE::NORMAL;
	_float m_fBlinkTimeAcc = 0.f;
	_float m_fBlinkTime = 0.2f;
	_float m_fBlinkInterval = 5.f;
public:
	static CFace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTexDirPath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END