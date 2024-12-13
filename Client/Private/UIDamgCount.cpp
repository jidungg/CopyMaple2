#include "stdafx.h"
#include "UIDamgCount.h"
#include "RenderTarget.h"
#include "RenderTarget_Manager.h"
#include "GameInstance.h"


_int arrDigitWidthCritical[10] = { 41, 29,40,40,42,40, 41,41,45,40 };
_int arrDigitWidth[10] = { 34, 25,35,33,33,33, 33,33,35,33 };

CUIDamgCount::CUIDamgCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWorldUIObject(pDevice, pContext)
{

}

CUIDamgCount::CUIDamgCount(const CUIDamgCount& Prototype)
	:CWorldUIObject(Prototype)
{
}

HRESULT CUIDamgCount::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIDamgCount::Initialize(void* pArg)
{
	UIDMGCOUNT_DESC* pDesc = (UIDMGCOUNT_DESC*)pArg;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeTimeAcc = m_fLifeTime;
	m_fRisingSpeed = pDesc->fSpeedPerSec;
	m_eDamgType = pDesc->eDamgType;
	
	wstring wstrTexture = L"";
	switch (m_eDamgType)
	{
	case Client::DAMG_TYPE::PLAYER_NORMAL:
			wstrTexture = L"UI_Texture_DamagCountPlayerNormal";
		break;
	case Client::DAMG_TYPE::PLAYER_CRITICAL:
			wstrTexture = L"UI_Texture_DamagCountPlayerCritical";
		break;
	case Client::DAMG_TYPE::MONSTER_NORMAL:
			wstrTexture = L"UI_Texture_DamagCountMonsterNormal";
		break;
	case Client::DAMG_TYPE::MONSTER_CRITICAL:
			wstrTexture = L"UI_Texture_DamagCountMonsterCritical";
		break;
	case Client::DAMG_TYPE::LAST:
		break;
	default:
		break;
	}
	if (Is_Critical())
	{
		m_iNumTextureStride = 54;
		m_arrNumTextureWidth = arrDigitWidthCritical;
	}
	else
	{
		m_iNumTextureStride = 40;
		m_arrNumTextureWidth = arrDigitWidth;
	}

	m_iNumTextureHeight = m_iNumTextureStride;

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(wstrTexture));

	Set_Damge(pDesc->iDamg);
	return S_OK;
}
HRESULT CUIDamgCount::Ready_Components(void* pArg)
{
	return S_OK;
}
void CUIDamgCount::Update(_float fTimeDelta)
{
	if (m_fLifeTimeAcc <= 0.f)
	{
		Set_Active(false);
		return;
	}
	m_fLifeTimeAcc -= fTimeDelta;
	m_pTransformCom->Go_Direction(_vector{ 0.f, 1.f, 0.f ,0.f}, fTimeDelta );
	__super::Update(fTimeDelta);
}

HRESULT CUIDamgCount::Render()
{
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;

	if (m_pShaderCom)
		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

	if (m_pVIBufferCom)
	{
		if (FAILED(m_pVIBufferCom->Bind_BufferDesc()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIDamgCount::Bind_ShaderResources(CShader* pShader)
{
	if (FAILED(__super::Bind_ShaderResources(pShader)))
		return E_FAIL;
	m_pTextureCom->Bind_ShaderResource(pShader, "g_Texture");
	
	pShader->Bind_RawValue("g_DigitCount", &m_iDigitCount, sizeof(_int));
	pShader->Bind_RawValue("g_NumberTextureStride", &m_iNumTextureStride, sizeof(_int));
	pShader->Bind_IntArray("g_arrDigitWidth", m_arrNumTextureWidth,10);

	_int arrDgitNumber[10]; 
	memset(arrDgitNumber, -1, sizeof(_int) * 10);

	for (_int i = 0; i < m_iDigitCount; i++)
	{
		arrDgitNumber[i] = m_vecNumbers[m_iDigitCount-i -1];
	}
	pShader->Bind_IntArray("g_arrDigitNumber", arrDgitNumber,10);

	return S_OK;
}
void CUIDamgCount::Set_Damge(_int iValue)
{
	m_vecNumbers.clear();
	while (iValue >= 1)
	{
		_int iCur = iValue % 10;
		m_vecNumbers.push_back(iCur);
		iValue /= 10;
	}
	m_iDigitCount = m_vecNumbers.size();
	_int iSizeX = 0;
	_int iSizeY = m_iNumTextureHeight;
	for (auto& i : m_vecNumbers)
	{
		iSizeX += m_arrNumTextureWidth[i];
	}
	m_pTransformCom->Scaling(max(1,iSizeX), max(1,iSizeY), 1);
}

void CUIDamgCount::Start()
{
	m_fLifeTimeAcc = m_fLifeTime;
	Set_Active(true);

}




CUIDamgCount* CUIDamgCount::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIDamgCount* pInstance = new CUIDamgCount(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIDamgCount");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIDamgCount::Clone(void* pArg)
{
	CUIDamgCount* pInstance = new CUIDamgCount(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIDamgCount");
		Safe_Release(pInstance);
	}

	return pInstance;
}
