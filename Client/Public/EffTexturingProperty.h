#pragma once
#include "Component.h"
#include "EffControllable.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CEffTexture;
class CEffTexturingProperty :
	public CComponent, public IEffControllable
{
protected:
	CEffTexturingProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffTexturingProperty(const CEffTexturingProperty& rhs);
	virtual ~CEffTexturingProperty() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* szDirPath, ifstream& inFile);
	virtual void Reset() override;
	HRESULT Bind_Texture(CShader* pShader);
	void Set_TextureTransformData(EFF_TEX_TYPE eTexSlot, EFF_TEX_OPERATION_TYPE eOpType, _float fValue);
	ID3D11ShaderResourceView* CreateEmptySRV();
private:
	vector < CEffTexture* > m_vecTexture;

	_float2 m_f2TexcoordScale[12];
	_float m_f2TexcoordRotate[12];
	_float2 m_f2TexcoordTranslate[12];
	_float2 m_f2TexcoordCenter[12];
public:
	static CEffTexturingProperty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFile);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END