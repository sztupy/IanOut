#include "global.h"
#include "graphic.h"
#include "vars.h"
#include "../commonutils/engine.h"

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------

//Skills						IanSkill;

int							TerX = 0; 
int							TerY = 0;

PFRMSingle					FullScreen = NULL;
PFRMSingle					FullScreen2 = NULL;
PFRMSingle					Temporary[20];
PFRMAnim					TempAnim[5];
PFRMSingle					Mouse = NULL;
PFRMSingle					Mouse2 = NULL;
PFRMSingle					KezeloP = NULL;
PFRMSingle					Select = NULL;
PFRMSingle					Select2 = NULL;
PFRMSingle					SkillDex = NULL;
PFRMSingle					MouseScr[8][2];
PFRMSingle					Arrows[2];
PFRMSingle					AmmoBox;


PFRMSingle					MenuScr[10][2];

PFRMAnim					WaitMouse;

PIanMap						MapInf;
PIanStatic					StaticInf;
PIanCritter					CritterInf;

//HBITMAP						MaskBMP;
//HBITMAP						MaskBMP2;

int							InGamePos = 0;

int							GamePos = 3;

int							wallpos[6][2];
int							current;
BlockType					BlockDat;

//std::string					szoveg[50];

std::string					filename;

int							Selected = 1;

PList						TimerList;
PList						TauntList;

buttons::PButtonList		ButtonList;

unsigned char				TravelMap[28][30];

int							TravelMapX,TravelMapY;

SDL_Rect					TerepBound;
bool						TerepBoundType;

PLocationList				MapLocations;

PMapLoader					MapLoader;

bool						graphoptions[8];


int BlitFRMTo(SDL_Surface* Target,PFRM Source,int x, int y,Uint32 flags,signed char xAl,signed char yAl)
{
	if (xAl<0) {
		if (yAl<0) return BlitTo(Target,0,0,Source->x,Source->y,x,y,flags,Source->FRM);
		if (yAl==0) return BlitTo(Target,0,0,Source->x,Source->y,x,y-Source->y/2,flags,Source->FRM);
		if (yAl>0) return BlitTo(Target,0,0,Source->x,Source->y,x,y-Source->y,flags,Source->FRM);
	}
	if (xAl==0) {
		if (yAl<0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x/2,y,flags,Source->FRM);
		if (yAl==0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x/2,y-Source->y/2,flags,Source->FRM);
		if (yAl>0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x/2,y-Source->y,flags,Source->FRM);
	}
	if (xAl>0) {
		if (yAl<0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x,y,flags,Source->FRM);
		if (yAl==0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x,y-Source->y/2,flags,Source->FRM);
		if (yAl>0) return BlitTo(Target,0,0,Source->x,Source->y,x-Source->x,y-Source->y,flags,Source->FRM);
	}
	return -1;
}
