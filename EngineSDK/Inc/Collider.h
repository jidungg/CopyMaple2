#pragma once
#include "Component.h"
BEGIN(Engine)
union COLLISION_ID
{
	struct {
		_uint Left_Id;
		_uint Right_Id;
	};
	LONGLONG ID;
};
class ENGINE_DLL CColliderBase :
    public CComponent
{

public:
	enum class COLLIDER_TYPE
	{
		SPHERE,
		AABB,
		OBB,
		FRUSTUM,
		MESH,
		CAPSULE,
		LAST
	};
	typedef struct ColliderDesc
	{
		_float3		vCenter;
	}COLLIDER_DESC;
	static constexpr _tchar m_szCompTag[] = L"Com_Collider";
protected:
	CColliderBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CColliderBase(const CColliderBase& Prototype);
	virtual ~CColliderBase() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_fmatrix WorldMatrix) abstract;
	virtual _bool Intersects(CColliderBase* pOther)abstract;
	virtual _bool Contains(const FXMVECTOR& vPos)abstract;
	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) abstract;
	virtual HRESULT Render() abstract;

	COLLIDER_TYPE Get_ColliderType() const {return m_eType;}
	_uint Get_ID() { return m_iID; }
protected:
	COLLIDER_TYPE m_eType = COLLIDER_TYPE::LAST;
	_bool m_bCollide = false;
	_uint m_iID = 0;
	static _uint m_iGlobalID ;
#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

public:
	virtual void Free() override;

};

END