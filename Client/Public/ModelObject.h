#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "Model.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)
class CModelObject :
    public CGameObject
{
public:
	typedef struct ModelObjDesc : public CGameObject::GAMEOBJECT_DESC
	{
		LEVELID eModelProtoLevelID = { LEVELID::LAST };
		_char  strModelProtoName[MAX_PATH];
		DIRECTION direction = { DIRECTION::XM };
		const CModel* pMimicTarget = { nullptr };
	}MODELOBJ_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_ModelObject";
protected:
	explicit CModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CModelObject(const CModelObject& Prototype);
	virtual ~CModelObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	float Get_AnimTime() { return m_pModelCom->Get_AnimTime(); }
	const _float4x4* Get_BoneMatrix(const _char* pBoneName) const;
	const CModel* Get_ModelCom() const { return m_pModelCom; }

	bool Is_AnimEnd() { return m_bAnimationEnd; }
	bool Is_AnimPostDelayEnd();

	void Set_AnimationLoop(_uint iIdx, _bool bIsLoop);
	void Set_Animation(_uint iIdx);
	void Set_AnimPostDelayPercent(_uint iIdx, _float fPercent);
	void Switch_Animation(_uint iIdx);
	HRESULT Replace_Model(MODELOBJ_DESC* pDesc);
protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources(CShader* pShader);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel::TYPE m_eModelType = CModel::TYPE::TYPE_NONANIM;
	CModel* m_pMimicTarget = { nullptr };
	bool m_bAnimationEnd = false;
public:
	static CModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END