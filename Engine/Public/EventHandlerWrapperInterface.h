#pragma once
#include "Base.h"

BEGIN(Engine)
class CEvent;
class  ENGINE_DLL IEventHandlerWrapperInterface : public CBase
{
public:
	virtual ~IEventHandlerWrapperInterface(){};
	void Exec(CEvent* evt)
	{
		Call(evt);
	}
	virtual void* GetInstanceAddress() = 0;

protected:
	virtual void Call(CEvent* evt) = 0;

};
END