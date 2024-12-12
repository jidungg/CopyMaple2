#include "stdafx.h"
#include "Face.h"	
#include "Texture.h"
#include <regex>
#include "Shader.h"

CFace::CFace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
	for (_uint i = 0; i < (_uint)FACE_STATE::LAST; i++)
	{
		m_pTexture[i] = nullptr;
		m_pTexture_C[i] = nullptr;
	}

}

CFace::CFace(const CFace& Prototype)
	: CComponent(Prototype)
{
	for (_uint i = 0; i < (_uint)FACE_STATE::LAST; i++)
	{
		m_pTexture[i] = Prototype.m_pTexture[i];
		Safe_AddRef(m_pTexture[i]);
		m_pTexture_C[i] = Prototype.m_pTexture_C[i];
		Safe_AddRef(m_pTexture_C[i]);
	}
}

HRESULT CFace::Initialize_Prototype(const _tchar* pTexDirPath)
{
	wstring wstrSubDir = L"ATTACK/";
	Ready_Face(pTexDirPath, wstrSubDir.c_str(), FACE_STATE::ATTACK);
	wstrSubDir = L"FOCUS/";
	Ready_Face(pTexDirPath, wstrSubDir.c_str(), FACE_STATE::FOCUS);
	wstrSubDir = L"IDLE/";
	Ready_Face(pTexDirPath, wstrSubDir.c_str(), FACE_STATE::IDLE);
	wstrSubDir = L"PAIN/";
	Ready_Face(pTexDirPath, wstrSubDir.c_str(), FACE_STATE::PAIN);
	wstrSubDir = L"TALK/";
	Ready_Face(pTexDirPath, wstrSubDir.c_str(), FACE_STATE::TALK);




	return S_OK;
}
HRESULT CFace::Ready_Face(const _tchar* pTexDirPath, const _tchar* pSubDirPath, FACE_STATE eState)
{
	path basePath = pTexDirPath;
	path subPath = pSubDirPath;
	path fullPath = basePath / subPath;
	for (const auto& entry : fs::directory_iterator(fullPath)) {
		if (entry.path().extension() == ".dds")
		{
			std::string filename = entry.path().stem().string();
			if (filename.size() < 3 || filename.substr(filename.size() - 2) != "_c") {
				string str = entry.path().string();
				wstring wstr(str.begin(), str.end());
				if(nullptr == m_pTexture[(_uint)eState])
					m_pTexture[(_uint)eState] = CTexture::Create(m_pDevice, m_pContext, wstr.c_str());
				else
				{
					m_pTexture[(_uint)eState]->Push_Texture(wstr.c_str());
				}
				str = fullPath.string();
				str += entry.path().stem().string() + "_c.dds";
				wstr = wstring(str.begin(), str.end());
				if (nullptr == m_pTexture_C[(_uint)eState])
					m_pTexture_C[(_uint)eState] = CTexture::Create(m_pDevice, m_pContext, wstr.c_str());
				else
				{
					m_pTexture_C[(_uint)eState]->Push_Texture(wstr.c_str());
				}
			}
		}
	}
	return S_OK;
}
void CFace::Update(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CFace::FACE_STATE::IDLE:
	case Client::CFace::FACE_STATE::TALK:
	case Client::CFace::FACE_STATE::ATTACK:
		if (m_fBlinkInterval < m_fBlinkTimeAcc)
		{
			if (m_fBlinkInterval + m_fBlinkTime <= m_fBlinkTimeAcc )
				m_fBlinkTimeAcc = 0.f;
			else if (m_fBlinkInterval + m_fBlinkTime / 3 * 2 <= m_fBlinkTimeAcc )
				m_eSubState = FACE_SUBSTATE::BLINK3;
			else if ( m_fBlinkInterval + m_fBlinkTime / 3 <= m_fBlinkTimeAcc )
				m_eSubState = FACE_SUBSTATE::BLINK2;
			else
				m_eSubState = FACE_SUBSTATE::BLINK1;
		}
		else
			m_eSubState = FACE_SUBSTATE::NORMAL;
		break;
	case Client::CFace::FACE_STATE::FOCUS:
	case Client::CFace::FACE_STATE::PAIN:
		m_eSubState = FACE_SUBSTATE::NORMAL;
		break;
	case Client::CFace::FACE_STATE::LAST:
		break;
	default:
		break;
	}
	m_fBlinkTimeAcc += fTimeDelta;
}

HRESULT CFace::Bind_FaceShaderResource(CShader* pShader, const _char* pConstantName)
{
	return m_pTexture[(_uint)m_eState]->Bind_ShaderResource(pShader, pConstantName, (_uint)m_eSubState);
}



CFace* CFace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTexDirPath)
{
	CFace* pInstance = new CFace(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTexDirPath)))
	{
		MSG_BOX("Failed to Created : CFace");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CFace::Clone(void* pArg)
{
	return new CFace(*this);
}

void CFace::Free()
{
	__super::Free();
	
	for (_uint i = 0; i < (_uint)FACE_STATE::LAST; i++)
	{
		Safe_Release(m_pTexture[i]);
		Safe_Release(m_pTexture_C[i]);
	}
}
