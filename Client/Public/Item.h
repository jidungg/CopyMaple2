#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTexture;
END
//�𵨿� ���� ������ �������� ����
//������ �κ��丮, ����, ���â �� UI���� ǥ�õž� �ϴ� ������ ����.
BEGIN(Client)
class CItem :
    public CBase
{
  
private:
    wstring m_strName;
	wstring m_strDesc;
	wstring m_strIconImageTag;
	_uint m_iPrice = 0;
	_uint m_iSellPrice = 0;
	_uint m_iMaxStack = 0;
	_uint m_iCurrentStack = 0;
	
};

END