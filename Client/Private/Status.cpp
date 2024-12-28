#include "stdafx.h"
#include "Status.h"

void Stat::Gain_Exp(_int iAmount)
{
	iEXP += iAmount;
	_int iReqExp = iLEVEL * iLEVEL;
	while (iReqExp <= iEXP)
	{
		iLEVEL++;
		iEXP -= iReqExp;
		iReqExp = iLEVEL * iLEVEL;
	}

}
