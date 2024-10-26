#include "ContainerObject.h"

bool CContainerObject::Check_Collision(const Ray& tRay, RaycastHit* pOut)
{
	bool bIsHit = false;
	RaycastHit tMinHit;
	tMinHit.fDist = 9999;

	bIsHit = __super::Check_Collision(tRay, pOut);
	if (bIsHit && pOut->fDist < tMinHit.fDist)
	{
		tMinHit = *pOut;
	}
	for (auto& child : m_vecPartObject)
	{
		if (child->Is_Active() == false || child->Is_Dead())
			continue;
		if (child->Check_Collision(tRay, pOut))
		{
			bIsHit = true;
			if (pOut->fDist < tMinHit.fDist)
			{
				tMinHit = *pOut;
			}
		}
	}
	if (bIsHit)
	{
		*pOut = tMinHit;
	}
    return false;
}
