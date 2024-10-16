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
class CModelObject :
    public CGameObject
{
public:
	typedef struct ModelObjDesc : public CGameObject::GAMEOBJECT_DESC
	{
		LEVELID eShaderProtoLevelID = LEVELID::LEVEL_END;
		LEVELID eModelProtoLevelID = LEVELID::LEVEL_END;
		_tchar  wstrModelProtoName[MAX_PATH];
		_tchar wstrShaderProtoName[MAX_PATH];
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

	virtual void ReplaceModel(CModel* pModel);

protected:
	virtual HRESULT Ready_Components(void* pArg);

private:
	HRESULT Bind_ShaderResources();

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	
public:
	static CModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END