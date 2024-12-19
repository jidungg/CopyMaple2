#pragma once
#include "UIPanel.h"
BEGIN(Client)
template<typename T>
class CUIVerticalFill :
    public CUIPanel
{
public:
	typedef struct UIVerticalFillDesc : public CUIPanel::PANEL_DESC
	{
		T* pValue= { nullptr };
		T* pDefaultValue= { nullptr };
		_float fVerticalStart = { 0.054f };
		_float fVerticalEnd = { 0.85f };
	}UIVERTICALFILL_DESC;
protected:
	explicit CUIVerticalFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIVerticalFill(const CUIVerticalFill& Prototype);
	virtual ~CUIVerticalFill() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Render() override;
private:
	T* m_pValue = { nullptr };
	T* m_pDefaultValue = { nullptr };

	_float m_fVerticalStart = 0.f;
	_float m_fVerticalEnd = 0.f;

public:
	static CUIVerticalFill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	void Free();
};

END

template<typename T>
inline CUIVerticalFill<T>::CUIVerticalFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

template<typename T>
inline CUIVerticalFill<T>::CUIVerticalFill(const CUIVerticalFill& Prototype)
	: CUIPanel(Prototype)
{
}

template<typename T>
inline HRESULT CUIVerticalFill<T>::Initialize(void* pArg)
{
	UIVERTICALFILL_DESC* pDesc = (UIVERTICALFILL_DESC*)pArg;
	m_pValue = pDesc->pValue;
	m_pDefaultValue = pDesc->pDefaultValue;
	m_fVerticalStart = pDesc->fVerticalStart;
	m_fVerticalEnd = pDesc->fVerticalEnd;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

template<typename T>
inline HRESULT CUIVerticalFill<T>::Render()
{
	_float fRatio = *m_pValue / (_float)*m_pDefaultValue;
	_float fBaseLine = m_fVerticalStart + (m_fVerticalEnd - m_fVerticalStart) * (1- fRatio);
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fVerticalRatio", &fBaseLine, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(3)))
			return E_FAIL;
	}
	if (m_pVIBufferCom)
	{
		if (FAILED(m_pVIBufferCom->Bind_BufferDesc()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

	}

	return S_OK;
}

template<typename T>
inline CUIVerticalFill<T>* CUIVerticalFill<T>::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{ 
	CUIVerticalFill<T>* pInstance = new CUIVerticalFill<T>(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIVerticalFill<T>");
		Safe_Release(pInstance);
	}
	return pInstance;
}

template<typename T>
inline CGameObject* CUIVerticalFill<T>::Clone(void* pArg)
{
	CUIVerticalFill<T>* pInstance = new  CUIVerticalFill<T>(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone :  CUIVerticalFill<T>");
		Safe_Release(pInstance);
	}
	return pInstance;
}

template<typename T>
inline void CUIVerticalFill<T>::Free()
{
	__super::Free();
}
