#pragma once

#include "GameObject.h"

/* 크랄이언트에서 만든 유아이용 객체드르이 부모가 되는 클래스다.*/
/* 직교투ㅡ여9ㅇ을 위한 데이터와 기능을 가지고 있을 꺼야. */

BEGIN(Engine)

END

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct : CGameObject::GAMEOBJECT_DESC
	{
		_float		fXOffset, fYOffset, fSizeX, fSizeY;
		CORNOR_TYPE ePivotType = CORNOR_TYPE::CENTER;
		CORNOR_TYPE eAnchorType = CORNOR_TYPE::CENTER;
	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	HRESULT Render() final;
	virtual HRESULT Render_Self()abstract;

public:
	void Add_ChildUI(CUIObject* pChildUI);
	void On_MouseEnter(const DIMOUSESTATE& tMouseState);
	void On_MouseOver(const DIMOUSESTATE& tMouseState);
	void On_MouseExit(const DIMOUSESTATE& tMouseState);
	bool Check_MouseOver(LONG lX, LONG lY);

	void Set_Offset(_float fX, _float fY);
	void Set_Size(_float fSizeX, _float fSizeY);
	void Set_PivotType(CORNOR_TYPE ePivotType) { m_ePivotType = ePivotType; }
	void Set_AnchorType(CORNOR_TYPE eAnchorType) { m_eAnchorType = eAnchorType; }
	_float2 Get_Position() { return _float2(m_fXPosition, m_fYPosition); }
	_float2 Get_Offset() { return _float2(m_fXOffset, m_fYOffset); }
	_float2 Get_ScreenSize() { return _float2(m_fSizeX, m_fSizeY); }
	_float2 Get_PivotOffset(CORNOR_TYPE ePivotType);
	_float2 Get_PivotPoint(CORNOR_TYPE ePivotType);
	_float2 Get_AnchorPoint(CORNOR_TYPE eAnchorType);

	static _float2 Get_CornorRatio(CORNOR_TYPE ePivotType);
protected:
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

	_float					m_fSizeX, m_fSizeY;
	//부모 Anchor 기준 Pivot의 위치
	_float					m_fXOffset, m_fYOffset;
	//Screen 기준 중앙 위치
	_float					m_fXPosition{ 0 }, m_fYPosition{ 0 };

	_float					m_iViewportWidth = {};
	_float					m_iViewportHeight = {};

	_uint					m_iDepth = {};

	list<CUIObject*>		m_pChildUIs;
	CUIObject*				m_pParentUI = nullptr;

	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	CORNOR_TYPE m_ePivotType = CORNOR_TYPE::CENTER;
	CORNOR_TYPE m_eAnchorType = CORNOR_TYPE::CENTER;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END