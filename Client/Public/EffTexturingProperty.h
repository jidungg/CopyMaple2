#pragma once
#include "Component.h"
BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CEffTexture;
class CEffTexturingProperty :
    public CComponent
{
protected:
	CEffTexturingProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffTexturingProperty(const CEffTexturingProperty& rhs);
	virtual ~CEffTexturingProperty() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* szDirPath, ifstream& inFile);
	HRESULT Bind_Texture( CShader* pShader, const _char* pConstantName, EFF_TEX_TYPE eType, _uint iTextureIndex = 0);
private:
	vector < CEffTexture* > m_vecTexture;

public:
	static CEffTexturingProperty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFile);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END