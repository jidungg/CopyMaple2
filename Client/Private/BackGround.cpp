#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"


CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround & Prototype)
	: CUIPanel{ Prototype }
{
}


HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	BACKGROUND_DESC*		pDesc = static_cast<BACKGROUND_DESC*>(pArg);

	/* 백그라우늗의 ㅁ크기아ㅘ 위치를 잡아주기위한 용오 .*/
	pDesc->fXOffset = g_iWinSizeX >> 1;
	pDesc->fYOffset = g_iWinSizeY >> 1;
	pDesc->fSizeX = g_iWinSizeX;
	pDesc->fSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}


CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*	pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

}
