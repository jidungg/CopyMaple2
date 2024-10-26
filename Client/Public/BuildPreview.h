#pragma once
#include "TerrainObject.h"

BEGIN(Client)
class CBuildPreview :
    public CTerrainObject
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_BuildPreview";
public:
	explicit CBuildPreview(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBuildPreview(const CBuildPreview& Prototype);
	virtual ~CBuildPreview() = default;
public:
	virtual void Compute_Matrix();
public:
	static CBuildPreview* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END