#pragma once
#include "Interactable.h"
#include "Collider_Sphere.h"
#include "set"
BEGIN(Client)
class CAttachableBodyPart :
    public CInteractableObject
{
public:
	typedef struct AttachableBodyPartDesc : public CGameObject::GameObjectDesc
	{
		const XMFLOAT4X4* pParentMatrix = { nullptr };
		const XMFLOAT4X4* pSocketMatrix = { nullptr };
		CCollider_Sphere::SPHERE_COLLIDER_DESC tColliderDesc{};
	}ATTACHABLE_BODYPART_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_AttachableBodyPart";
protected:
	explicit CAttachableBodyPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CAttachableBodyPart(const CInteractableObject& Prototype);
	virtual ~CAttachableBodyPart() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Interact(CPlayer* pActor) override;
	virtual _bool Is_InteractionPossible(CPlayer* pActor) override;
	virtual void Compute_Matrix() override;

	void Detach_Players();
	void RemovePlayer(CPlayer* pPlayer);
	const XMFLOAT4X4* Get_ParentMatrix() const { return m_pParentMatrix; }
	const XMFLOAT4X4* Get_SocketMatrix() const { return m_pSocketMatrix; }
	_float Get_Radius();
private:
	const XMFLOAT4X4* m_pSocketMatrix = { nullptr };
	set<CPlayer*> m_setAttachedPlayer;
public:
	static CAttachableBodyPart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END