#pragma once
#include "Component.h"

/* �ٸ� ������Ʈ�ʹ� �� �ٸ��� ��ü ���� �� ������ Ʈ������ ������Ʈ�� ������ �ְ� �Ұ��̴�. */
/* ��ü�� ���� ���¸� ǥ���Ѵ�.(m_WorldMatrix) */
/* ��ü�� ���� ���� ��ȯ�� ����� �����Ѵ�. ������ ����, ȸ���Ѵ�. �ٶ󺻴�.  */
/* ��������� �������� ������ �� �ֵ��� ���̴��� ��������� ������ ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct
	{
		_float		fSpeedPerSec{};
		_float		fRotationPerSec{};

	}TRANSFORM_DESC;
protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	void Set_Parent(CTransform* pParent) { m_pParentTransform = pParent;}
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState], vState);
	}

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}
	_float3 Compute_Scaled();


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Scaling(_float fX, _float fY, _float fZ);
	virtual void Go_Straight(_float fTimeDelta);
	virtual void Go_Backward(_float fTimeDelta);
	virtual void Go_Left(_float fTimeDelta);
	virtual void Go_Right(_float fTimeDelta);
	virtual void Go_Direction(_fvector vDirection, _float fTimeDelta);

	void LookAt(_fvector vAt);

	/* ���� ȸ���� �������� �߰��� ������ �ӵ��� ȸ���Ѵ�. */
	void Turn(_fvector vAxis, _float fTimeDelta);
	/* �׵���¸� �������� ������ ������ ȸ���Ѵ�. */
	void Rotation(_fvector vAxis, _float fRadian);

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);


protected:
	/* rowmajor*/
	_float4x4				m_WorldMatrix = {};

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

	CTransform*				m_pParentTransform = nullptr;
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END