#pragma once
#include "Component.h"
#include "EffControllable.h"
BEGIN(Client)
class CTexture;
class CEffMaterialProperty :
    public CComponent, public IEffControllable
{
protected:
	CEffMaterialProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffMaterialProperty() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile);
private:
	_float3 m_vAmbient = { 0,0,0 };
	_float3 m_vDiffuse = { 0,0,0 };
	_float3 m_vSpecular = { 0,0,0 };
	_float3 m_vEmissive = { 0,0,0 };
	_float m_fGlossiness = { 10 };
	_float m_fAlpha = { 1 };
public:
	static CEffMaterialProperty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& inFile);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END