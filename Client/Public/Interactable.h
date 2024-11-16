#pragma once
#include "ModelObject.h"

BEGIN(Client)
class CPlayer;
class CInteractableObject
	:public CModelObject
{
protected:
	explicit CInteractableObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CInteractableObject(const CInteractableObject& prototype);
	virtual ~CInteractableObject() = default;
public:
	virtual void Interact(CPlayer* pActor) abstract;
	virtual _bool Is_InteractionPossible(CPlayer* pActor) abstract;
};

END