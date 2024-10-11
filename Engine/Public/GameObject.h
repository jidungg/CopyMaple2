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
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
public:
	virtual void Add_Child(CGameObject* pChild);
	class CComponent* Find_Component(const _wstring& strComponentTag);
	bool Is_Active() { return m_bActive; }
	void Set_Target(CGameObject* pTaraget) { m_pTarget = pTaraget; Safe_AddRef(m_pTarget); }
	class CTransform* Get_Transform() { return m_pTransformCom; }
protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_Component(CComponent* pComponent, const _wstring& strComponentTag, void* pArg = nullptr);

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CTransform*			m_pTransformCom = { nullptr };
	CGameObject*				m_pTarget = { nullptr };
	CGameObject*				m_pParent = { nullptr };
	list<CGameObject*>			m_pChilds;

	_uint						m_iObjID = {};
	static _uint				m_iObjCount;
	bool						m_bActive = true;
protected:
	map<const _wstring, class CComponent*>		m_Components;


protected:
	_uint						m_iData = {};
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END