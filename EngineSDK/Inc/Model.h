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
	virtual HRESULT Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);
	virtual HRESULT Render();

public:
	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex = 0);
	void Play_Animation(_float fTimeDelta);


public:
	_uint Get_NumMeshes() const {return m_iNumMeshes;}
	CMesh* Get_Mesh(_uint iMeshIndex) const { return m_Meshes[iMeshIndex]; }
	_uint Get_BoneIndex(const _char* pBoneName);

private:
	TYPE						m_eModelType = { TYPE_END };

	_uint						m_iNumMeshes = { 0 };
	vector<class CMesh*>		m_Meshes;

	_uint												m_iNumMaterials = {};
	vector<CMaterial*>						m_Materials;

	vector<class CBone*>		m_Bones;
	_float4x4					m_PreTransformMatrix = {};
private:
	HRESULT Ready_Meshes(ifstream& inFile);
	HRESULT Ready_Materials(ifstream& inFile, const _char* pModelFilePath);
	HRESULT Ready_Bones(ifstream& inFile, _int iParentBoneIndex);


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const _char* pModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END