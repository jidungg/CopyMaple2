#pragma once
#include "Component.h"

BEGIN(Engine)
class CTexture;
class ENGINE_DLL CMaterial :
    public CComponent
{
protected:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* szDirPath, aiMaterial* pMaterial);

	HRESULT Bind_Texture(class CShader* pShader, const _char* pConstantName, aiTextureType eType, _uint iTextureIndex = 0);
private:
	vector < CTexture* > m_vecTexture;
public:
	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, aiMaterial* pMaterial);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END