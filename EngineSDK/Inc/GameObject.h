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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Compute_Matrix();
	virtual void Late_Update(_float fTimeDelta);
	void Final_Update();
	virtual HRESULT Render();

public:
	virtual _bool Check_Collision(const Ray& tRay, RaycastHit* pOut);
	virtual _bool Check_Collision(CGameObject* pOther) { return false; };
	//*AddRef는 스스로 할 것.
	// LayerID를 자동으로 자신과 같게 세팅하니 주의.
	virtual void Add_Child(CGameObject* pChild);
	//*Release는 스스로 할 것.
	virtual void Remove_Child(CGameObject* pChild);
	class CComponent* Find_Component(const _wstring& strComponentTag);
	virtual void Hit(CGameObject* pFoe, _int fDamage) {}

	void Set_Active(bool bValue) { m_bActive = bValue; }
	void Toggle_Active() { m_bActive = !m_bActive; }
	void Set_Dead() { m_bDead = true; }
	void Set_Dead(_bool bValue) { m_bDead = bValue; }
	void Set_Target(CGameObject* pTaraget) { m_pTarget = pTaraget; }
	void Set_DontDestroy(bool bValue) { m_bDontDestroy = bValue; }
	void Set_LayerID(_uint iLayerID) { m_iLayerID = iLayerID; }
	void Set_Transform(_vector vPos = { 0,0,0,1 }, _vector vRotation = { 0,0,0,0 }, _float fScale = 1);
	void Set_Transform(CTransform* pTransform);

	class CTransform* Get_Transform() { return m_pTransformCom; }
	CGameObject* Get_Target() { return m_pTarget; }
	_uint Get_LayerID() { return m_iLayerID; }
	_uint Get_ObjID() { return m_iObjID; }
	_vector Get_TransformPosition() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_vector Get_WorldPosition();
	_float Get_Distance(CGameObject* pOther);
	CColliderBase* Get_Collider(_uint iColliderIndex);
	const _float4x4* Get_WorldMatrix() { return &m_WorldMatrix; }

	bool Is_Active() { return m_bActive; }
	bool Is_Dead() { return m_bDead; }
	bool Is_DontDestroy() { return m_bDontDestroy; }
	bool Is_Valid();
protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_Component(CComponent* pComponent, const _wstring& strComponentTag);
	HRESULT Remove_Component(CComponent* pComponent);
protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	class CTransform*			m_pTransformCom = { nullptr };

	list<CGameObject*>			m_pChilds;
	map<const _wstring, class CComponent*>		m_Components;
	const _float4x4* m_pParentMatrix = {};
	_float4x4				m_WorldMatrix = {};

	bool						m_bActive = { true };
	bool						m_bDead = { false };
	bool						m_bDontDestroy = { false };	

	vector<CColliderBase*> m_vecCollider;
protected:
	_uint						m_iObjID = {};
	static _uint				m_iObjCount;
	_uint						m_iLayerID = {};
private:
	CGameObject*				m_pTarget = { nullptr };
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END