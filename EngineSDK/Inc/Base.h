#pragma once

#include "Engine_Defines.h"

/* 1. ���۷��� ī��Ʈ�� �����Ѵ�. */
/* 1-1. ���۷��� ī��Ʈ�� �����Ѵ�. AddRef */
/* 1-2. ���۷��� ī��Ʈ�� �����Ѵ�. or �����Ѵ�. Release */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	virtual _uint AddRef();
	_uint Release();

private:
	_uint			m_iRefCnt = { 0 };

public:
	virtual void Free();

};

END