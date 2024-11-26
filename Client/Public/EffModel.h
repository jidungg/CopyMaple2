#pragma once
#include "Model.h"

BEGIN(Engine)
class CShader;
END
BEGIN(Client)
enum  NODE_TYPE
{
    NT_NODE,
    NT_MESH,
    NT_BILLBOARD,
    NT_LAST
};

class CEffModel  : public CComponent
{

protected:
    CEffModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffModel(const CEffModel& Prototype);
    virtual ~CEffModel() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
    virtual HRESULT Render(CShader* pShader);

public:
    HRESULT Bind_Texture( CShader* pShader, const _char* pConstantName, _uint iMeshIndex, EFF_TEX_TYPE eType, _uint iTextureIndex = 0);
    HRESULT Bind_BoneMatrices( CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
    _bool Update_Animation(_float fTimeDelta);
	void Play_Animation();


public:
    _uint Get_NumMeshes() const { return m_iNumMeshes; }
    _uint Get_BoneIndex(const _char* pBoneName) const;

    void Set_Loop(bool bLoop) { m_bLoop = bLoop; }
protected:
    HRESULT Ready_Bones(ifstream& inFile, _uint iParentBoneIndex);
    HRESULT Ready_Meshes(ifstream& inFile);
    HRESULT Ready_Materials(ifstream& inFile);
    HRESULT Ready_Texturings(ifstream& inFile, const _char* pModelFilePath);
    HRESULT Ready_Controls(ifstream& inFile);

private:

    vector<class CEffBone*>		m_vecBone;

	_uint m_iNumMeshes = { 0 };
    vector<class CEffMesh*>		m_vecMesh;

    vector<class CEffMaterialProperty*>				m_vecMaterial;

	vector<class CEffTexturingProperty* > 				m_vecTexturing;

	vector<class CEffController*>				m_vecControl;

    _float4x4					m_PreTransformMatrix = {};


    _bool m_bPlay = { false };
    _bool m_bLoop = { false };
public:
    static CEffModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END