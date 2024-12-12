#include "stdafx.h"
#include "UIDamgCount.h"
#include "RenderTarget.h"
#include "RenderTarget_Manager.h"
#include "GameInstance.h"


_int iNumTextureStrideCritical[10] = { 54, 54,54,54,54,54, 54,54,54,54 };
_int iNumTextureStride[10] = { 34, 22,32,32,33,32, 32,32,35,32 };

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	UIDMGCOUNT_DESC* pDesc = (UIDMGCOUNT_DESC*)pArg;
	Set_Damge(pDesc->iDamg);
	m_fLifeTime = pDesc->fLifeTime;
	m_fLifeTimeAcc = m_fLifeTime;
	m_fRisingSpeed = pDesc->fRisingSpeed;

	wstring wstrTexture = L"";
	switch (m_eDamgType)
	{
	case Client::DAMG_TYPE::PLAYER_NORMAL:
			wstrTexture = L"Player_Normal";
			m_iNumTextureStride = 40;
		break;
	case Client::DAMG_TYPE::PLAYER_CRITICAL:
			wstrTexture = L"Critical_Player";
			m_iNumTextureStride = 54;
		break;
	case Client::DAMG_TYPE::MONSTER_NORMAL:
			wstrTexture = L"Monster_Normal";
			m_iNumTextureStride = 40;
		break;
	case Client::DAMG_TYPE::MONSTER_CRITICAL:
			wstrTexture = L"Monster_Critical";
			m_iNumTextureStride = 54;
		break;
	case Client::DAMG_TYPE::LAST:
		break;
	default:
		break;
	}
	m_iNumTextureHeight = m_iNumTextureStride;

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Object_Stock(wstrTexture));
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
	m_pTransformCom->Go_Direction(_vector{ 0.f, 1.f, 0.f ,0.f}, fTimeDelta * m_fRisingSpeed);
	__super::Update(fTimeDelta);
}

HRESULT CUIDamgCount::Render()
{
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;

	if (m_pShaderCom)
		m_pShaderCom->Begin(2);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
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
	pShader->Bind_IntArray("g_arrDigitWidth", m_iNumTextureWidth,10);

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
	m_iValue = iValue;
	while (m_iValue >= 1)
	{
		_int iCur = iValue % 10;
		m_vecNumbers.push_back(iCur);
		iValue /= 10;
	}
	m_iDigitCount = m_vecNumbers.size();
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
