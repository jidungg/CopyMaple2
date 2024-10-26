#pragma once
#include "BoneModelObject.h"

BEGIN(Client)
class CWeapon :
    public CBoneModelObject
{
public:
	typedef struct WeaponDesc : public CBoneModelObject::BONEMODELOBJ_DESC
	{
		const _float4x4* pBackSocketMatrix;
	}WEAPON_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Weapon";
protected:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& Prototype);
	virtual ~CWeapon() = default;
public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Compute_Matrix() override;

	void Set_Battle(bool bBattle);

private:
	bool m_bBattle = false;
	const _float4x4* m_pBackSocketMatrix = { nullptr };
public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
};

END