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
    public CPawn
{
public:
	typedef struct ModelObjDesc : public CGameObject::GAMEOBJECT_DESC
	{
		CModel::TYPE eModelType = CModel::TYPE::TYPE_NONANIM;
		LEVELID eShaderProtoLevelID = LEVELID::LAST;
		LEVELID eModelProtoLevelID = LEVELID::LAST;
		_char  strModelProtoName[MAX_PATH];
		_char strShaderProtoName[MAX_PATH];
		DIRECTION direction = DIRECTION::XM;
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

	void Set_AnimationLoop(_uint iIdx, _bool bIsLoop);
	void Switch_Animation(_uint iIdx);

	float Get_AnimTime() { return m_pModelCom->Get_AnimTime(); }
	bool Is_AnimEnd() { return m_bAnimationEnd; }
protected:
	virtual HRESULT Ready_Components(void* pArg);

private:
	HRESULT Bind_ShaderResources();

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel::TYPE m_eModelType = CModel::TYPE::TYPE_NONANIM;
	bool m_bAnimationEnd = false;
public:
	static CModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END