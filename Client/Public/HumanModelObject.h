#pragma once
#include "ModelObject.h"
#include "Face.h"

BEGIN(Client)

class CHumanModelObject :
    public CModelObject
{
public:
	typedef struct HumanModelObjDesc : public CModelObject::MODELOBJ_DESC
	{
		const _tchar* szFaceProtoTag = { nullptr };
	}HUMANMODELOBJ_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_HumanModelObject";

protected:
	explicit CHumanModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHumanModelObject(const CHumanModelObject& Prototype);
	virtual ~CHumanModelObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Render() override;

	void Set_FaceState(CFace::FACE_STATE eState) { m_pFaceCom->Set_State(eState); }
private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	CFace* m_pFaceCom = { nullptr };
	_uint m_iFaceMeshIdx = { 0 };
	CShader* m_pFaceShaderCom = { nullptr };
public:
	static CHumanModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END