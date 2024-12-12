#pragma once
#include "Base.h"

BEGIN(Client)
typedef struct CEffectData
{
	CEffectData(json& jData)
	{
		eEffectType = jData["Type"];
	}
	EFF_TYPE eEffectType = { EFF_TYPE::LAST };

}EFFECT_DATA;

END