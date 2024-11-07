#pragma once
#include "Base.h"

BEGIN(Client)
typedef struct Stat 
{
	Stat() = default;
	Stat(json& js)
	{
		iHP = js["HP"];
		iSP = js["SP"];
		iEP = js["EP"];
		iATK = js["ATK"];
		iDEF = js["DEF"];
		iEXP = js["EXP"];
		iGold = js["Gold"];

		fJumpPower = js["JumpPower"];
		fRunSpeed = js["RunSpeed"];
		fWalkSpeed = js["WalkSpeed"];
		fAttackInterval = js["AttackInterval"];
		fAttackRange = js["AttackRange"];
	}
	_int iHP { 0};
	_int iSP { 0};
	_int iEP { 0};
	_int iATK { 0}; 
	_int iDEF { 0};
	_int iEXP { 0};
	_int iGold { 0};

	_float fJumpPower { 4.5f };
	_float fRunSpeed{ 2.5f };
	_float fWalkSpeed{ 1.5f };
	_float fAttackInterval{ 1.f };
	_float fAttackRange{ 2.f };
}STATUS;

END