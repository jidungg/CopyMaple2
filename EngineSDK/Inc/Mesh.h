#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh);
	virtual HRESULT Initialize(void* pArg);

private:
	_char						m_szName[MAX_PATH] = "";


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END