#include "stdafx.h"
#include "Interactable.h"

CInteractableObject::CInteractableObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}


CInteractableObject::CInteractableObject(const CInteractableObject& Prototype)
	: CGameObject(Prototype)
{
}
