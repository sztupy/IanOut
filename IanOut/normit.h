#ifndef __NORMIT_H
#define __NORMIT_H

#include "../frmobject/specob.h"
#include "IanOs.h"
#include "play.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"
#include "../commonutils/date.h"
#include <bitset>
//#include <string>

namespace play {
	extern int							Frame;
	extern DWORD						ThisTick;
	extern DWORD						LastTick;
	extern int							mousetyp;
	extern int							menet;
	extern int							secondinc;
	extern std::bitset<10>				ability;
	extern CritterList::iterator		iter2save;
	extern FRMLocationMap::iterator		itersave;
	extern PFRM							FRMMouse;
	extern PInventory					InvenT;
	extern PInventoryItem				InvenTItem;
	extern BlockType					BlDat;
	extern int							addx;
	extern int							addy;

	extern bool							DrawOutline;

	extern bool							StepOnExit;
	extern bool							StepType;
	extern int							StepLocation;
	extern int							StepEntrance;

	extern bool							TurnBasedMode;

	extern bool							AllowUserInteract;
	
	extern PFRMPlayer					CurrentPlayer;

	void InventoryMode(void);
	void NormalMode(void);
	void LootMode(void);
	void CountVisible(void);

	void OnNextFr(void);

	void NextInTurn(void);

	void AiMove(void);

	inline void DrawIanCucc(bool i);
}

inline void play::DrawIanCucc(bool i)
{
	int x,y;
	x = addx+266;
	y = GetMaxY-99+27;
	if (i && (InGamePos == 0)) { x+=1;y-=2;};
	
	if (Ian->curhand) {
		Ian->Hand1->DrawOut(g_pDDSBack,x,y);
	} else {
		Ian->Hand2->DrawOut(g_pDDSBack,x,y);
	}
	
	if (i && (InGamePos == 0)) { x-=1;y+=2;};
	int length = 0;
	if (Ian->curhand) {
		if (Ian->Hand1->capacity != 0) length = 65.0/(double)Ian->Hand1->capacity*(double)Ian->Hand1->numammo;
	} else {
		if (Ian->Hand2->capacity != 0) length = 65.0/(double)Ian->Hand2->capacity*(double)Ian->Hand2->numammo;
	}
	
	BlitTo(g_pDDSBack,0,0,AmmoBox->FRM->x,length,184+x,y+65-length,DDBLTFAST_SRCCOLORKEY,AmmoBox->FRM->FRM);
}

#endif //__NORMIT_H