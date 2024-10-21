#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* �ε�ȭ���� �����ϱ����� ��ü(�ε�ȭ���� ���, �ε� ��, �ε� ��Ʈ)�� ������. */
/* ���� ������ �ʿ��� �ڿ��� �ε��ϴ� ��Ȱ�� �ϴ� ��ü�� �������ش�.  */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVELID eNextLevelID);
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVELID			m_eNextLevelID = { LAST };
	class CLoader*	m_pLoader = { nullptr };

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END