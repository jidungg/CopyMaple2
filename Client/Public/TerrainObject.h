#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CVIBuffer_Rect;
class CCollider;
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
		wstring modleName;
		_float3 pos = {0,0,0};
		DIRECTION direction = DIRECTION::DIR_END;
		int data = 0;//MonsterSpawner È¤Àº Portal¿ë
		_uint index = 0;
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

	void Set_TerrainDir(DIRECTION eDir) { m_eTerrainDir = eDir; }

	virtual json ToJson();
	void Turn(DIRECTION eDir);
	void Rotate(DIRECTION eDir);
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	TERRAIN_OBJ_TYPE m_eTerrObjType = TERRAIN_OBJ_TYPE::TERRAIN_OBJ_END;
	wstring m_modleName;
	_uint m_iIndex = 0;

	DIRECTION m_eTerrainDir = DIRECTION::DIR_END;

	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = {nullptr};

public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END