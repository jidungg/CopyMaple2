#pragma once
#include "Base.h"

BEGIN(Client)
typedef struct Stat 
{
	Stat() = default;
	Stat(const json& js)
	{
		iSP = js["SP"];
		iEP = js["EP"];

		iSTR = js["STR"];
		iDEX = js["DEX"];
		iINT = js["INT"];
		iLUK = js["LUK"];
		iHP = js["HP"];
		fCrit = js["CRIT"];

		iEXP = js["EXP"];
		iGold = js["Gold"];
		iLEVEL = js["LEVEL"];

		fJumpPower = js["JumpPower"];
		fRunSpeed = js["RunSpeed"];
		fWalkSpeed = js["WalkSpeed"];
		fAttackInterval = js["AttackInterval"];
		fAttackRange = js["AttackRange"];

		fHPRecovery = js["HPRecovery"];
		fSPRecovery = js["SPRecovery"];
		fEPRecovery = js["EPRecovery"];
	}
	_int Get_AllStat()
	{
		return iSTR + iDEX + iINT + iLUK;
	}
	_int iSP { 0};
	_int iEP { 0};

	_int iSTR{ 0 };
	_int iDEX{ 0 };
	_int iINT{ 0 };
	_int iLUK{ 0 };
	_int iHP { 0};
	_float fCrit{ 0 };

	_int iEXP { 0};
	_int iGold { 0};
	_int iLEVEL{ 0 };

	_float fJumpPower { 10.f };
	_float fRunSpeed{ 2.5f };
	_float fWalkSpeed{ 1.5f };
	_float fAttackInterval{ 1.f };
	_float fAttackRange{ 2.f };
	_float fBodyHeight{ 0.5f };

	_float fHPRecovery = { 10 };
	_float fSPRecovery = { 10 };
	_float fEPRecovery = { 0.75 };

}STATUS;

END