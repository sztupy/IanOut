#include "play.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"
#include "../commonutils/mouse.h"

extern int oldGamePos;

namespace play {
	PInventory InventorySave;
}

void play::EditInventory(void)
{
    int                     hRet;
	//DDBLTFX                     ddbltfx;
	
	static int					Frame = -25;
	static Uint32				ThisTick;
	static Uint32				LastTick = 0;
	static int					addnumber;

	
	PInventoryItem		InvenTItem = NULL;
	
	static int					InvenNumber = 0;
	
	if ((strcmp(FullScreen->fname,"\\art\\intrface\\loot.frm")))
	{
    	LoadFRMSingle(&FullScreen,"\\art\\intrface\\loot.frm",1);
	}

	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3;
			
        }

	if (MousX>=GetMaxX) { MousX=GetMaxX;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=GetMaxY) { MousY=GetMaxY;}
	if (MousY<=0)   { MousY=0;  }
	ClearSurface(g_pDDSBack,0,0,0);
	int addx = (GetMaxX-640)/2;
	int addy = (GetMaxY-480)/2;addy+=20;
	BlitTo(g_pDDSBack,0,0,FullScreen->FRM->x,FullScreen->FRM->y,
						addx+51,
						addy+4,
						0,FullScreen->FRM->FRM);
	
	static int page1;
	static int page2;
	int max1,max2;
	int i,i2;
	PInventory IanInventory = InventorySave;

	PItem Item;
	i = 0; i2=0;
	Item = IanInventory->Inven->First();
	while (Item!=NULL) {
		if ((i2>=page1) && (i2<=page1+5)) {
			PFRM frm = ((PInventoryItem)Item)->FRMA->GetCurFrame();
			if ((frm->x>50) || (frm->y>50)) {
				double d = ((double)frm->y/(double)frm->x*50);
				BlitToStretch(g_pDDSBack,0,0,frm->x,frm->y,addx+230,addy+8+70+i*55-d/2,addx+230+50,addy+8+70+i*55+d/2,0,frm->FRM);
			} else {
				BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+230,addy+8+45+i*55,0,frm->FRM);
			}
			int count = ((PInventoryItem)Item)->count;
			if (count!=1) {
				char buf[20];
				sprintf(buf,"x%i",count);
				textfont::IanOutTextR(addx+280,addy+95+i*55,0,buf);
			}
			if (mouse::MInPr(addx+230,addy+8+45+i*55,addx+230+50,addy+8+95+i*55,0)) {
				InvenTItem = (PInventoryItem)Item;
			}
			i++;
		}
		Item = IanInventory->Inven->Next(Item);
		i2++;
	}
	if (InvenTItem) IanInventory->GetItem(InvenTItem);
	max1=i2;
	    
	FRMPairCollection::iterator Item2;
	i = 0;i2 = 0;
	Item2 = StaticInf->TilesI.begin();
	while (Item2!=StaticInf->TilesI.end()) {
		if ((i2>=page2) && (i2<=page2+5)) {
			PFRM frm = Item2->second->FRMA->GetCurFrame();
			if ((frm->x>50) || (frm->y>50)) {
				double d = ((double)frm->y/(double)frm->x*50);
				BlitToStretch(g_pDDSBack,0,0,frm->x,frm->y,addx+350,addy+8+70+i*55-d/2,addx+400,addy+8+70+i*55+d/2,0,frm->FRM);
			} else {
				BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+350,addy+8+45+i*55,0,frm->FRM);
			}

			if (mouse::MInPr(addx+350,addy+8+45+i*55,addx+400,addy+8+95+i*55,0)) {
				IanInventory->AddItem(Item2->first,addnumber,0,StaticInf->TilesI);
			}
			i++;
		}
		Item2++;
		i2++;
	}
	max2 = i2;

    int bdown = 0;
	bool InSide = false;
	if (mouse::MouseIn(addx+170,addy+35,addx+190,addy+64)) InSide = true; else InSide = false;
	textfont::IanOutText(addx+50+125,addy+35,InSide ? 0 : 1,"^\n||");if (InSide && mouse::Pressed(3)) bdown = 3;

	if (mouse::MouseIn(addx+170,addy+65,addx+190,addy+94)) InSide = true; else InSide = false;
	textfont::IanOutText(addx+50+125,addy+65,InSide ? 0 : 1,"||\nv");if (InSide && mouse::Pressed(3)) bdown = 4;

	if (mouse::MouseIn(addx+425,addy+35,addx+445,addy+64)) InSide = true; else InSide = false;
	textfont::IanOutText(addx+50+380,addy+35,InSide ? 0 : 1,"^\n||");if (InSide && mouse::Pressed(3)) bdown = 5;

	if (mouse::MouseIn(addx+425,addy+65,addx+445,addy+94)) InSide = true; else InSide = false;
	textfont::IanOutText(addx+50+380,addy+65,InSide ? 0 : 1,"||\nv");if (InSide && mouse::Pressed(3)) bdown = 6;

	ref_to = NULL;
	if (mouse::MouseIn(0,addy-20,GetMaxX,addy)) { ref_to = &addnumber; InSide = true; } else { InSide = false; if (addnumber<1) addnumber=1; }
	char buf[150]; if (addnumber!=1) sprintf(buf,"Add %i items.",addnumber); else sprintf(buf,"Add 1 item.");
	textfont::IanOutTextC(GetMaxX/2,addy-20,InSide ? 0 : 1,buf);

	if (mouse::MInPr(addx+50+400,addy+330,addx+50+500,addy+360,3)) {
		page1 = 0;
		page2 = 0;
		GamePos = oldGamePos;
	}

	if (bdown == 3) page1--;
	if (bdown == 4) page1++;
	if (bdown == 5) page2--;
	if (bdown == 6) page2++;

	if ((page1+5)>max1) page1=max1-5;
	if ((page2+5)>max2) page2=max2-5;
	if (page1<0) page1=0;
	if (page2<0) page2=0;
	

	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	
    // Flip the surfaces
    SDL_Flip(g_pDDSBack);
	
	
}
