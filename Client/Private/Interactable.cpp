#include "stdafx.h"
#include "Interactable.h"

CInteractableObject::CInteractableObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CInteractableObject::CInteractableObject(const CInteractableObject& prototype)
	:CModelObject(prototype)
{
}
