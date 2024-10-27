#pragma once
#include "ModelObject.h"
BEGIN(Client)
class CMimicBoneModelObject :
    public CModelObject
{
public:
	typedef struct MimicBoneModelObjectDesc : public CModelObject::MODELOBJ_DESC
	{
		const CModel* pTarget = { nullptr };
	}MIMICBONEOBJECT_DESC;

public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_MimicBoneObjectModel";

protected:
	explicit CMimicBoneModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMimicBoneModelObject(const CMimicBoneModelObject& Prototype);
	virtual ~CMimicBoneModelObject() = default;

protected:
		virtual HRESULT Ready_Components(void* pArg);

public:
	static CMimicBoneModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END