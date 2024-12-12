#pragma once
#include "Component.h"
#include "EffControllable.h"
BEGIN(Engine)
class CShader;
END

BEGIN(Client)
struct EffMaterialData
{
	_float3 vAmbient = { 0,0,0 };
	_float3 vDiffuse = { 0,0,0 };
	_float3 vSpecular = { 0,0,0 };
	_float3 vEmissive = { 0,0,0 };
	_float fGlossiness = { 10 };
	_float fAlpha = { 1 };
};
class CTexture;
class CEffMaterialProperty :
	public CComponent, public IEffControllable
{
protected:
	CEffMaterialProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffMaterialProperty(const CEffMaterialProperty& other);
	virtual ~CEffMaterialProperty() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile);
	
	virtual void Reset() override;

	HRESULT Bind_Material(CShader* pShader);
	void Set_Alpha(_float fAlpha);
private:
	EffMaterialData m_tMaterialData;
	EffMaterialData m_tDefaultMaterialData;
public:
	static CEffMaterialProperty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& inFile);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END