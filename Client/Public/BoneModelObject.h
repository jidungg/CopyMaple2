#pragma once
#include "ModelObject.h"

BEGIN(Client)
class CBoneModelObject :
    public CModelObject
{
public:
	typedef struct BoneModelObjDesc : public CModelObject::MODELOBJ_DESC
	{
		const _float4x4* pSocketMatrix;
	}BONEMODELOBJ_DESC;

public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_BoneModelObject";

protected:
	explicit CBoneModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBoneModelObject(const CBoneModelObject& Prototype);
	virtual ~CBoneModelObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Compute_Matrix() override;

	virtual HRESULT Render() override;

protected:
	const _float4x4* m_pSocketMatrix = { nullptr };
public:
	static CBoneModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END