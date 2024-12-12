#include "Collider.h"
#include "GameInstance.h"
#include "DebugDraw.h"

_uint CColliderBase::m_iGlobalID = 0;

CColliderBase::CColliderBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
	m_iID = m_iGlobalID++;

}

CColliderBase::CColliderBase(const CColliderBase& Prototype)
	:CComponent(Prototype)
	, m_eType{ Prototype.m_eType }
	, m_pEffect{ Prototype.m_pEffect }
	, m_pBatch{ Prototype.m_pBatch }
	, m_pInputLayout{ Prototype.m_pInputLayout }
{
	m_iID = m_iGlobalID++;
	Safe_AddRef(m_pInputLayout);
}

HRESULT CColliderBase::Initialize_Prototype()
{

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = { 0 };

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif
	return S_OK;
}

HRESULT CColliderBase::Initialize(void* pArg)
{
	CColliderBase::COLLIDER_DESC* pDesc = static_cast<CColliderBase::COLLIDER_DESC*>(pArg);
	m_vCenterOffset = pDesc->vCenter;

	return S_OK;
}


void CColliderBase::Free()
{
	__super::Free();
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	Safe_Release(m_pInputLayout);
}
