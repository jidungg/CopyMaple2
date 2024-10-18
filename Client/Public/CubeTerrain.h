#pragma once

#include "UIObject.h"
#include "TerrainObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CModel;
END


BEGIN(Client)
class CTerrainObject;
class CCubeTerrain final : public CGameObject
{
private:
	CCubeTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	CCubeTerrain(const CCubeTerrain& Prototype);
	virtual ~CCubeTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_float4 IndexToPos(_uint Index);
	_uint PosToIndex(const _float4& Pos);
	XMUINT3 Get_Size() { return m_vSize; }
	HRESULT Add_TerrainObject( CTerrainObject::TERRAINOBJ_DESC& tDesc);
	CTerrainObject* Get_TerrainObject(_uint Index);
	
	HRESULT Save_To_Json(string strNewFilepath);
private:
	HRESULT Load_From_Json(string strJsonFilePath);
private:
	string m_strJsonFilePath;
	XMUINT3 m_vSize = { 1, 1, 1 };
	
	vector<CTerrainObject*> m_vecCells;
public:
	static CCubeTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END