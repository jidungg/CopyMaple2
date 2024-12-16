#include "Material.h"
#include "Texture.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}


HRESULT CMaterial::Initialize_Prototype(const _char* szDirPath, ifstream& inFile)
{
	m_vecTexture.resize(TEXTURE_TYPE_MAX);

	for (size_t texTypeIdx = 1; texTypeIdx < TEXTURE_TYPE_MAX; texTypeIdx++)
	{
		CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, szDirPath, inFile);
		m_vecTexture[texTypeIdx] = (pTexture);
	}
	return S_OK;
}

HRESULT CMaterial::Bind_Texture(CShader* pShader, const _char* pConstantName, TEXTURE_TYPE eType, _uint iTextureIndex)
{
	return m_vecTexture[(_uint)eType]->Bind_ShaderResource(pShader, pConstantName, iTextureIndex);
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFile)
{
	CMaterial* pInstance = new CMaterial(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szDirPath,inFile)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CMaterial::Free()
{
	__super::Free();
	for (auto& tex : m_vecTexture)
		Safe_Release(tex);
	m_vecTexture.clear();
}

CComponent* CMaterial::Clone(void* pArg)
{
	return nullptr;
}
