#pragma once

#include "GameObject.h"

/* 크랄이언트에서 만든 유아이용 객체드르이 부모가 되는 클래스다.*/
/* 직교투ㅡ여9ㅇ을 위한 데이터와 기능을 가지고 있을 꺼야. */

BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
class ENGINE_DLL CUIObject  : public CGameObject
{
public:
	typedef struct : public CRect_Transform::RECTTRANSFORM_DESC
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
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;


	virtual void Add_Child(CGameObject* pChild) override;
	void Increase_Priority();

	_uint Get_Priority() { return m_iPriority; }
public:

	virtual void On_MouseEnter();
	virtual void On_MouseOver();
	virtual void On_MouseExit();
	virtual bool Consume_MouseLButtonDown();
	virtual bool Consume_MouseLButtonUp();
	virtual bool Consume_MouseRButtonDown();
	virtual bool Consume_MouseRButtonUp();
	virtual bool Consume_MouseClick();
	bool Check_MouseOver(POINT fPos);

	CUIObject* Find_FocusedUI(POINT fPos);
protected:
	virtual HRESULT Bind_ShaderResources();

protected:
	static _uint			m_iStaticPriority;
	_uint					m_iPriority = {};

	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


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