#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CController :
    public CBase
{
private:
	CController(void);
	virtual ~CController(void) = default;

public:
	static CController* Create();
	virtual void Free(void) override;

};

END