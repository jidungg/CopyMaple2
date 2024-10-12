#pragma once
#include "Collider.h"

BEGIN(Engine)
class CMesh;
class ENGINE_DLL CMeshCollider :
    public CCollider
{
public:
	typedef struct MeshColliderDesc
	{
		CMesh* pMesh = { nullptr };
	}MESH_COLLIDER_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_Component_MeshCollider";
private:
	explicit CMeshCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMeshCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	bool Check_Collision(CCollider* pOther, RaycastHit* pOut) override;
	bool Check_Collision(const Ray& tRay, RaycastHit* pOut) override;

private:
	ID3D11Buffer* m_pStagingVB = nullptr;
	ID3D11Buffer* m_pStagingIB = nullptr;
	_uint m_iNumVertices;
	_uint m_iNumIndexes;
public:
	static CMeshCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END