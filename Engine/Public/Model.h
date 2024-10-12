#pragma once

#include "Component.h"

BEGIN(Engine)
class CMaterial;
class CMesh;
class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;


public:
	virtual HRESULT Initialize_Prototype(TYPE eModelType, const _char* pModelFilePath);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);
	virtual HRESULT Render();

public:
	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex = 0);

public:
	_uint Get_NumMeshes() const {return m_iNumMeshes;}
	CMesh* Get_Mesh(_uint iMeshIndex) const { return m_Meshes[iMeshIndex]; }
private:
	Assimp::Importer			m_Importer;
	const aiScene* m_pAIScene = { nullptr };

	_uint										m_iNumMeshes = { 0 };
	vector<CMesh*>		m_Meshes;

	_uint												m_iNumMaterials = {};
	vector<CMaterial*>						m_Materials;

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const _char* pModelFilePath);


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const _char* pModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END