#ifndef __PLAYER_H
#define __PLAYER_H

#include "../commonutils/ItemObj.h"
#include <stdio.h>
#include <string>
#include <vector>
#include "FRM.H"
#include "IanSkill.h"
#include "items.h"
#include "armwep.h"

class TFRMPlayer;
class TIanCritter;

typedef TFRMPlayer *PFRMPlayer;
typedef TIanCritter *PIanCritter;

typedef std::multimap<unsigned int,PFRMPlayer> CritterList;
typedef std::pair<unsigned int,PFRMPlayer> Critter_Pair;

extern short MoveTable[512][512];

extern PList AnimList;
void GarbageCollect();

namespace textutil {
	std::string GetFromProf(std::string fnam,std::string master,std::string slave);
	void AddString(std::string duma,int i2);
}

typedef struct __PDF
{
	bool bIdle;
	bool bMove;
	bool bRun;
	bool bComputer;
	
	int num;
	int Count;

	unsigned int proto;

	std::string CharName;

	std::string name;
	std::string deflocation;
	std::string protolocation;

	int var1;
	int var2;

	int karma;
	int group;

	PWeapon Unarmed1;
	PWeapon Unarmed2;
} PlayerDef;

class TIanCritter : public TItem
{
public:
	TIanCritter() {};
	~TIanCritter();

	int LoadCritters(const char* filename, FRMPairCollection &TilesI);
	int LoadMainCharacter(FRMPairCollection &TilesI);
	int ClearButMain();

	CritterList Critters;
};

class TFRMPlayer : public TItem
{
public:
	TFRMPlayer(PlayerDef PDef,PInventory IInven) 
		{
		  curth = 0;
		  afterth = 0;
		  dir = 0;
		  framenum = 0;
		  Player = PDef;
		  fr2 = 0;
		  fr3 = 0;
		  Inven = IInven;

		  Idle = NULL;	
		  Moving = NULL;	
		  Runing = NULL;	

		  GetWeapon = NULL;
		  PutWeapon = NULL;

		  Other[0] = NULL;	
		  Other[1] = NULL;	
		  Other[2] = NULL;	

		  DeadAnimation = NULL;

		  LocationSave = NULL;
		  PlayerSave = NULL;

		  FileExist.clear();

		  curhand = true;
		  StopMove = false;
		  visibletype = 0;
		  ianskill::Nullaz(&PlayerSkill);
		};
	~TFRMPlayer();

	std::string GetGraphicNameWeapon(char* descriptor, PWeapon Weapon);
	std::string GetGraphicName(char* descriptor);
	void LoadPlayer(bool isLoaded);
	
	void Draw(SDL_Surface* g_pDDSBack,int TerX, int TerY, int colorinf);

	void TFRMPlayer::GetDraw(int& XX, int& YY);

	void UseCommand(PItem Item,bool ChrType,BlockType &Block,bool type,std::string command);
	void UseOn(PLocationDat LocDat,BlockType &Block,bool type, signed char dirx, signed char diry);
	void Pickup(PLocationDat LocDat,BlockType &Block);
	void Attack(PFRMPlayer Item,PWeapon AttackWh);
	void TFRMPlayer::HitAndMiss(int type);

	int RealDistance(int tx, int ty, BlockType &Block);
	void GenerateTree(int tx, int ty, BlockType &Block);
	void MoveTo(int t_x, int t_y) {x = t_x; y=t_y; };
	void Next(BlockType &Block,FRMLocationMap &FRMMap,CritterList &Critters);
	void Move(BlockType &Block);

	void SetStaticState();

	void TFRMPlayer::LookAt(int atx, int aty);

	void GoAround(int atx, int aty, BlockType &Block);
	inline int Distance(int atx,int aty);

	void KillPerson(int diemode);
	void Die(PIanStatic StaticInf);

	void ChangeWeapon(bool whichhand, unsigned int num, unsigned int ammonum, FRMPairCollection &TilesI);
	void ChangeHand();

	int GetDir();

	bool AlreadyDrawn;

	unsigned int conv;
	int x,y;
	int ox,oy;
	int curth;
	int afterth;
	int framenum;
	int fr2,fr3;
	int dir;
	int cx,cy;
	bool Labpos;
	signed char RunPos;
	PlayerDef Player;
	short PosIt[101][2];

	PInventory Inven;
	Skills PlayerSkill;

	bool curhand;

	PWeapon Hand1;
	PWeapon Hand2;

	PWeapon WeaponSave;
	
	PLocationDat LocationSave;
	PFRMPlayer PlayerSave;
	bool UseType;
	bool StopMove;
	std::string execcomm;

	PFRMAnim DeadAnimation;
	int howtodie;

	int visibletype;

	std::map<std::string,bool> FileExist;

	PFRMCommunicator Idle;		/* 0 */
	PFRMCommunicator Moving;	/* 1 */
	PFRMCommunicator Runing;	/* 2 */

	PFRMCommunicator Other[3];  /* 20 */

	PFRMCommunicator GetWeapon;
	PFRMCommunicator PutWeapon;
};

#endif __PLAYER_H