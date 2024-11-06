#pragma once

#include "GameObject.h"

/* ũ���̾�Ʈ���� ���� �����̿� ��ü�帣�� �θ� �Ǵ� Ŭ������.*/
/* �������ѿ�9���� ���� �����Ϳ� ����� ������ ���� ����. */

BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
class ENGINE_DLL CUIObject  : public CGameObject
{
public:
	typedef struct UIObjectDesc: public CRect_Transform::RECTTRANSFORM_DESC
	{
		CGameObject* pParent = { nullptr };
		CGameObject* pTarget = { nullptr };
		CTexture* pTextureCom = { nullptr };
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
	virtual void On_MouseLButtonDown();
	virtual void On_MouseLButtonUp();
	virtual void On_MouseRButtonDown();
	virtual void On_MouseRButtonUp();
	virtual void On_MouseClick();
	virtual void On_MouseRightClick();

	CUIObject* Find_FocusedUI(POINT fPos);
protected:


protected:
	static _uint			m_iStaticPriority;
	_uint					m_iPriority = {};
	_uint					m_iSRVIndex= 0;		


	CGameObject* m_pParent = { nullptr };
private:
	bool m_bMouseOver = false;
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