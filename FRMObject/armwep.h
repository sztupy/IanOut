#ifndef __ARMWEP_H
#define __ARMWEP_H

#include "items.h"

class TWeapon;
class TWeaponMode;

typedef TWeapon *PWeapon;
typedef TWeaponMode *PWeaponMode;

class TWeaponMode : public TItem
{
public:
	TWeaponMode() {};
	~TWeaponMode() {};

	std::string name;
	int minr,maxr,sharp;
	int apuse,mindam,maxdam;
	int criticaldeath,normalcriticalchance;

	std::string before;
	std::string shoot;
	std::string after;

	int mode;
};

class TWeapon : public TItem
{
public:
	TWeapon() { WepMode = new TList(); };
	~TWeapon() { if (WepMode) delete WepMode; };

	PWeaponMode GetCurrentMode() { return PWeaponMode(Current); };
	void FirstMode() { Current = WepMode->First(); };
	void NextMode() { Current = WepMode->RNext(Current); };

	void Load(unsigned int itemnum,FRMPairCollection &TilesI/*const char* filename*/);
	HRESULT DrawOut(LPDIRECTDRAWSURFACE7 Back,int x, int y);
	HRESULT DrawOutSmall(LPDIRECTDRAWSURFACE7 Back,int x, int y,int maxx);

	bool HasGraphic;
	std::string deflocation;
	int WeaponType;
	unsigned int ammotype;
	unsigned int number;
	unsigned int numammo;
	unsigned int capacity;
	PList WepMode;

	PFRMAnim Picture;
protected:
	PItem Current;
};

#endif //__ARMWEP_H