#pragma once
#include "Transform.h"

BEGIN(Engine)
class ENGINE_DLL CRect_Transform :
    public CTransform
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		_float		fXOffset, fYOffset, fSizeX, fSizeY;
		CORNOR_TYPE ePivotType = CORNOR_TYPE::CENTER;
		CORNOR_TYPE eAnchorType = CORNOR_TYPE::CENTER;
	}RECTTRANSFORM_DESC;
protected:
	CRect_Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRect_Transform(const CRect_Transform& Prototype);
	virtual ~CRect_Transform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;


	void Compute_Matrix();

	virtual void Go_Straight(_float fTimeDelta);
	virtual void Go_Backward(_float fTimeDelta);
	virtual void Go_Left(_float fTimeDelta);
	virtual void Go_Right(_float fTimeDelta);
	void Move(_float fX, _float fY);
public:
	
	void Set_Offset(_float fX, _float fY);
	void Set_Size(_float fSizeX, _float fSizeY);
	void Set_PivotType(CORNOR_TYPE ePivotType) { m_ePivotType = ePivotType; }
	void Set_AnchorType(CORNOR_TYPE eAnchorType) { m_eAnchorType = eAnchorType; }
	_float2 Get_Position() { return _float2(m_fXPosition, m_fYPosition); }
	_float2 Get_Offset() { return _float2(m_fXOffset, m_fYOffset); }
	_float2 Get_ScreenSize() { return _float2(m_fSizeX, m_fSizeY); }
	_float2 Get_PivotOffset(CORNOR_TYPE ePivotType);
	_float2 Get_PivotPosition(CORNOR_TYPE ePivotType);
	_float2 Get_AnchorPosition(CORNOR_TYPE eAnchorType);
	bool Is_InRect(_float2 vPosition);
public:
	static _float2 Get_CornorRatio(CORNOR_TYPE ePivotType);
protected:

	_float					m_fSizeX, m_fSizeY;
	//부모 Anchor 기준 Pivot의 위치
	_float					m_fXOffset, m_fYOffset;
	//Screen 기준 중앙 위치
	_float					m_fXPosition{ 0 }, m_fYPosition{ 0 };

	_float					m_iViewportWidth = {};
	_float					m_iViewportHeight = {};

	CORNOR_TYPE m_ePivotType = CORNOR_TYPE::CENTER;
	CORNOR_TYPE m_eAnchorType = CORNOR_TYPE::CENTER;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END