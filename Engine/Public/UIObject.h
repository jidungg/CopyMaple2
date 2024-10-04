#pragma once

#include "GameObject.h"

/* 크랄이언트에서 만든 유아이용 객체드르이 부모가 되는 클래스다.*/
/* 직교투ㅡ여9ㅇ을 위한 데이터와 기능을 가지고 있을 꺼야. */

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct : CGameObject::GAMEOBJECT_DESC
	{
		_float		fXOffset, fYOffset, fSizeX, fSizeY;
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

	void Add_ChildUI(CUIObject* pChildUI);
	void On_MouseEnter(const DIMOUSESTATE& tMouseState);
	void On_MouseOver(const DIMOUSESTATE& tMouseState);
	void On_MouseExit(const DIMOUSESTATE& tMouseState);
	bool Check_MouseOver(LONG lX, LONG lY);

protected:
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

	_float					m_fXPosition, m_fYPosition, m_fSizeX, m_fSizeY;
	_float					m_fXOffset, m_fYOffset;

	_float					m_iViewportWidth = {};
	_float					m_iViewportHeight = {};

	_uint					m_iDepth = {};

	list<CUIObject*>		m_pChildUIs;
	CUIObject*				m_pParentUI = nullptr;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END