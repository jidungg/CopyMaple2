#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* szMapFileName);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	wstring m_strMapName;


private:
	HRESULT Ready_Cells();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* szMapFileName);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END