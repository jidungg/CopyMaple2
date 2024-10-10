#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CTerrainObject :
	public CGameObject
{
public:
	typedef struct TerrainObjDesc: public CGameObject::GAMEOBJECT_DESC
	{
		virtual ~TerrainObjDesc() = default;
		TERRAIN_OBJ_TYPE eType = TERRAIN_OBJ_TYPE::TERRAIN_OBJ_END;
		string modleName;
		_float3 Pos = { 0,0,0 };
		_float3 Rot = { 0,0,0 };
		int data = 0;//MonsterSpawner È¤Àº Portal¿ë
	}TERRAINOBJ_DESC;

protected:
	explicit CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrainObject(const CTerrainObject& Prototype);
	virtual ~CTerrainObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual json ToJson();
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	TERRAIN_OBJ_TYPE m_eTerrObjType = TERRAIN_OBJ_TYPE::TERRAIN_OBJ_END;
	string m_modleName;

	
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CModel* m_pVIBufferCom = { nullptr };
public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END