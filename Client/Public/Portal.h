#pragma once
#include "EffModelObject.h"
#include "Interactable.h"
BEGIN(Client)
class CPortal :
	public CInteractableObject
{
public:
	typedef struct PortalDesc : public CEffModelObject::EFFECTOBJ_DESC
	{
		LEVELID eLevelID = { LEVELID::LAST };
	}PORTAL_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Portal";
protected:
	explicit CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPortal(const CPortal& Prototype);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual _bool Check_Collision(CGameObject* pOther) override;
	virtual HRESULT Render()override;

	//Interactable
	virtual void Interact(CPlayer* pActor) override;
	virtual _bool Is_InteractionPossible(CPlayer* pActor) override;

private:
	HRESULT Bind_ShaderResources(CShader* pShader);
protected:
	LEVELID m_eLevelID = { LEVELID::LAST };
	CEffModel* m_pEffModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END