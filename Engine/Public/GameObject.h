#pragma once
#include "Rect_Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct GameObjectDesc: public CTransform::TRANSFORM_DESC
	{
		CGameObject* pTarget = { nullptr };
	}GAMEOBJECT_DESC;
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	
		
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Compute_Matrix();
	virtual void Late_Update(_float fTimeDelta);
	void Final_Update();
	virtual HRESULT Render();
public:

	//*AddRef는 스스로 할 것.
	virtual void Add_Child(CGameObject* pChild);
	//*Release는 스스로 할 것.
	virtual void Remove_Child(CGameObject* pChild);
	class CComponent* Find_Component(const _wstring& strComponentTag);
	bool Is_Active() { return m_bActive; }
	bool Is_Dead() { return m_bDead; }
	bool Is_DontDestroy() { return m_bDontDestroy; }

	void Set_Active(bool bValue) { m_bActive = bValue; }
	void Toggle_Active() { m_bActive = !m_bActive; }
	void Set_Dead() { m_bDead = true; }
	void Set_Target(CGameObject* pTaraget) { Safe_Release(m_pTarget); m_pTarget = pTaraget; Safe_AddRef(m_pTarget); }
	void Set_DontDestroy(bool bValue) { m_bDontDestroy = bValue; }

	class CTransform* Get_Transform() { return m_pTransformCom; }
	virtual bool Check_Collision(const Ray& tRay, RaycastHit* pOut);
	virtual bool Check_Collision(CGameObject* pOther) { return false; };
protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_Component(CComponent* pComponent, const _wstring& strComponentTag);
	HRESULT Remove_Component(CComponent* pComponent);
protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	class CTransform*			m_pTransformCom = { nullptr };
	CGameObject*				m_pTarget = { nullptr };

	list<CGameObject*>			m_pChilds;
	map<const _wstring, class CComponent*>		m_Components;
	const _float4x4* m_pParentMatrix = {};
	_float4x4				m_WorldMatrix = {};

	bool						m_bActive = { true };
	bool						m_bDead = { false };
	bool						m_bDontDestroy = { false };	

protected:
	_uint						m_iObjID = {};
	static _uint				m_iObjCount;
	_uint						m_iLayerID = {};
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END