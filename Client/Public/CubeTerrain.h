#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CModel;
END


BEGIN(Client)

class CCubeTerrain final : public CGameObject
{
private:
	CCubeTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* szMapFileName);
	CCubeTerrain(const CCubeTerrain& Prototype);
	virtual ~CCubeTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	wstring m_strJsonFilePath;
	_float3 m_vSize = { 1.f, 1.f, 1.f };

private:
	HRESULT Load_From_Json(wstring strJsonFilePath);
	HRESULT Save_To_Json(wstring strNewFilepath);

public:
	static CCubeTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* szMapFileName);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END