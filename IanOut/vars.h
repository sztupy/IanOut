#ifndef __VARS_H
#define __VARS_H

#include "global.h"
#include "../frmobject/buttons.h"
#include "../frmobject/wloc.h"

extern PIanMap						MapInf;
extern PIanStatic					StaticInf;
extern PIanCritter					CritterInf;

extern int							TerX; 
extern int							TerY;

extern int							GamePos;
extern int							InGamePos;

extern int							wallpos[6][2];
extern int							current;
extern BlockType					BlockDat;

//extern std::string					szoveg[50];
//extern int							szovegnum;

extern std::string					filename;

extern int							Selected;

extern PList						TimerList;
extern PList						TauntList;

extern buttons::PButtonList			ButtonList;

extern unsigned char				TravelMap[28][30];

extern int							TravelMapX,TravelMapY;

extern PLocationList				MapLocations;

extern PMapLoader					MapLoader;

extern SDL_Rect						TerepBound;
extern bool							TerepBoundType;

extern bool							graphoptions[8];

namespace play {
	extern PFRMPlayer					Ian;
}


#endif //__VARS_H