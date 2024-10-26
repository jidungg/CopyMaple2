#pragma once
#include "Model.h"

BEGIN(Engine)
class CMaterial;
class CMesh;
class CModel;
class CMimicBone;
class ENGINE_DLL CMimicBoneModel  : public CModel
{
public:
	typedef struct MimicBoneModelDesc
	{
		const CModel* pTarget = { nullptr };
	}MIMICBONEMODEL_DESC;
private:
	CMimicBoneModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMimicBoneModel(const CMimicBoneModel& Prototype);
	virtual ~CMimicBoneModel() = default;


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual bool Play_Animation(_float fTimeDelta) override;
protected:
	HRESULT Ready_Bones(ifstream& inFile, _uint iParentBoneIndex);


public:
	static CMimicBoneModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	static CMimicBoneModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
};

END

