#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CColliderBase;
END
BEGIN(Client)
class CEffModel;
class CEffectObject :
    public CGameObject
{
public:
	typedef struct EffectObjDesc : public CGameObject::GAMEOBJECT_DESC
	{
		LEVELID eModelProtoLevelID = { LEVELID::LAST };
		_char  strModelProtoName[MAX_PATH];
	}EFFECTOBJ_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_EffectObject";
protected:
	explicit CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEffectObject(const CEffectObject& Prototype);
	virtual ~CEffectObject() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Play_Animation();
private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources(CShader* pShader);
private:
	CEffModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
public:
	static CEffectObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END 