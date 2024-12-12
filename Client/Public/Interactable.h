#pragma once
#include "ModelObject.h"

BEGIN(Client)
class CPlayer;
class IInteractable
{
public:
	virtual void Interact(CPlayer* pActor) abstract;
	virtual _bool Is_InteractionPossible(CPlayer* pActor) abstract;
};

class CInteractableObject
	: public CGameObject, public IInteractable
{
protected:
	explicit CInteractableObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CInteractableObject(const CInteractableObject& Prototype);
	virtual ~CInteractableObject() = default;

};

END