#pragma once
#include "Base.h"

BEGIN(Client)
typedef struct Stat 
{
	Stat() = default;
	Stat(const json& js)
	{
		iHP = js["HP"];
		iSP = js["SP"];
		iEP = js["EP"];
		iATK = js["ATK"];
		fCrit = js["CRIT"];
		iDEF = js["DEF"];
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
	_int iHP { 0};
	_int iSP { 0};
	_int iEP { 0};
	_int iATK { 0}; 
	_float fCrit{ 0 };
	_int iDEF { 0};
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