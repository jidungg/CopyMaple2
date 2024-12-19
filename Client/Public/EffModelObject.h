#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CColliderBase;
END
BEGIN(Client)
class CEffModel;
class CEffModelObject :
    public CGameObject
{
public:
	typedef struct EffectObjDesc : public CGameObject::GAMEOBJECT_DESC
	{
		EffectObjDesc() {}
		EffectObjDesc(json jData)
		{
			eModelProtoLevelID = jData["LevelProto"];
			strcpy_s(strModelProtoName, jData["Model"].get<string>().c_str());
		}
		LEVELID eModelProtoLevelID = { LEVELID::LAST };
		_char  strModelProtoName[MAX_PATH];
		_bool bAttachToBone = { false };
		const _char* szBoneName = { nullptr };
		CGameObject* pAttachObject = { nullptr };
	}EFFECTOBJ_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_EffectObject";
protected:
	explicit CEffModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEffModelObject(const CEffModelObject& Prototype);
	virtual ~CEffModelObject() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Compute_Matrix() override;
	void Start_Animation(_float fStartDealy = 0,_bool bLoop = false, _float fDuration = -1, _float fSpeed = -1);

	void Register_OnAnimEndCallBack(const function<void(CEffModel*)>& fCallback);
	void Register_AnimEvent(ANIM_EVENT tAnimEvent);

	void Set_AnimSpeed(_float fSpd);
protected:
	virtual void On_EffectAnimEnd(CEffModel* pModel) { Set_Active(false); }
	virtual void On_EffectAnimStart(CEffModel* pModel) { Set_Active(true); }

private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources(CShader* pShader);
protected:
	CEffModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_fTimeAcc = { 0.f };
	_float m_fStartDelay = { 0 };
	_float m_fDuration = { 0 };

	_bool m_bAttachToBone = { false };
	const _float4x4* m_pSocketMatrix = { nullptr };
public:
	static CEffModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END 