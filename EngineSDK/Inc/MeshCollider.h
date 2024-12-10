#pragma once
#include "Collider.h"

BEGIN(Engine)
class CMesh;
class ENGINE_DLL CCollider_Mesh :
    public CColliderBase
{
public:
	typedef struct MeshColliderDesc
	{
		CMesh* pMesh = { nullptr };
	}MESH_COLLIDER_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_Component_MeshCollider";
private:
	explicit CCollider_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCollider_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual	void Update(_fmatrix WorldMatrix) override;
	HRESULT Render() override;
	virtual _bool Contains(const FXMVECTOR& vPos) override;
	virtual _bool Intersects(CColliderBase* pOther) override;
	virtual _bool Intersects(CColliderBase* pOther, _vector* vOutNormal, _float* fOutDepth) override;
	bool RayCast(const Ray& tRay, RaycastHit* pOut) override;

private:
	ID3D11Buffer* m_pStagingVB = nullptr;
	ID3D11Buffer* m_pStagingIB = nullptr;
	_uint m_iNumVertices;
	_uint m_iNumIndexes;
public:
	static CCollider_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END