#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTexture;
END
//모델에 대한 정보는 포함하지 않음
//오로지 인벤토리, 상점, 장비창 등 UI에서 표시돼야 하는 정보만 포함.
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