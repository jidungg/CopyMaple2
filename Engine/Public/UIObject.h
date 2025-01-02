#pragma once

#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
class ENGINE_DLL CUIObject  : public CGameObject
{
public:
	typedef struct UIObjectDesc: public CRect_Transform::RECTTRANSFORM_DESC
	{
		_bool bDraggableX = { false };
		_bool bDraggableY = { false };
		_bool bRegisterUIManager = { true };
	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta) override;

	virtual void Add_Child(CGameObject* pChild) override;
	virtual void Remove_Child(CGameObject* pChild);
	void Increase_Priority();
	void Compute_Matrix_Recursive();
	virtual void Compute_Matrix() override;

	_uint Get_Priority() { return m_iPriority; }
public:

	void MouseOver();
	void MouseNotOver(CUIObject* pExcept = nullptr);
	virtual bool Check_MouseOver(POINT fPos);
	virtual void On_MouseOver();
	virtual void On_MouseEnter();
	virtual void On_MouseExit();
	virtual void On_MouseLButtonDown(const POINT& tMousePoint);
	virtual void On_MouseLButtonUp(const POINT& tMousePoint);
	virtual void On_MouseRButtonDown();
	virtual void On_MouseRButtonUp();
	virtual void On_MouseClick();
	virtual void On_MouseRightClick();
	virtual void On_MouseDrag(const POINT& tMousePoint, const DIMOUSESTATE& tState);
	virtual void On_Dropped(CUIObject* pFloorObject);

	virtual _bool Is_GrabbablePoint(const POINT& tScreenMousePoint);
	_bool Is_RegisterUIManager() { return m_bRegisterUIManager; }
	CUIObject* Find_FocusedUI(POINT fPos);
	void Get_MouseOverObjects(const POINT& tMousePoint, list<CUIObject*>& listOut);

protected:


protected:
	static _uint			m_iStaticPriority;
	_uint					m_iPriority = {};
	_uint					m_iSRVIndex= 0;		

	CGameObject* m_pParent = { nullptr };

	_bool m_bDraggableX = { false };
	_bool m_bDraggableY = { false };
	// 그랩됐을 당시 자신의 위치.
	POINT m_tGrabbedOffset = { 0,0 };
	// 그랩 됐을 당시 마우스의 위치.
	POINT m_tGrabbedMousePos = { 0,0 };
	_bool m_bGrabbed = { false };
	_bool m_bRegisterUIManager = { true };
	_bool m_bContainer = { false };
private:
	_bool m_bMouseOver = { false };
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

class UIPriorityCompare
{
public:
	bool operator()(CUIObject* pSrc, CUIObject* pDst)
	{
		return pSrc->Get_Priority() > pDst->Get_Priority();
	}
};
END