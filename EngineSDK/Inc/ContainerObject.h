#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CContainerObject :
    public CGameObject
{
public:
    virtual bool Check_Collision(const Ray& tRay, RaycastHit* pOut);
    vector<CGameObject*>			m_vecPartObject;
};

END