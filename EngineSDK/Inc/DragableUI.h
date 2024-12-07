#pragma once
#include "Base.h"
BEGIN(Engine)
class IDragableUI
{
public:
	virtual _bool Is_DraggableY() { return m_bDraggableX; }
	virtual _bool Is_DraggableX() { return m_bDraggableY; }
	virtual _bool Is_GrabPoint(POINT tScreenPosition)abstract;
	virtual void Drag(DIMOUSESTATE tState) abstract;

protected:
	_bool m_bDraggableX = { true };
	_bool m_bDraggableY = { true };
};

END