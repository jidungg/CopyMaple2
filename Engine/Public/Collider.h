#pragma once
#include "Component.h"
BEGIN(Engine)
class CCollider :
    public CComponent
{
public:
	enum class COLLIDER_TYPE
	{
		SPHERE,
		AABB,
		OBB,
		CAPSULE,
		MESH,
		LAST
	};
	typedef struct ColliderDesc
	{
		_float3		vCenter;
	}COLLIDER_DESC;
	static constexpr _tchar m_szCompTag[] = L"Com_Collider";
protected:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_fmatrix WorldMatrix) abstract;
	virtual _bool Intersect(CCollider* pOther)abstract;
	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) abstract;
	virtual HRESULT Render() { return S_OK; }

	COLLIDER_TYPE Get_ColliderType() const {return m_eType;}
protected:
	COLLIDER_TYPE m_eType = COLLIDER_TYPE::LAST;
	bool m_bCollide = false;
#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

public:
	virtual void Free() override;

};

END