#include "play.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

int t2amount = 0;
int t3id = 0;
int t3var1 = 0;
int t3var2 = 0;
int t4type = 0;
int t4loc = 0;
int t4entr = 0;
int t5id = 0;

void play::EditObjects(void)
{
    int									x,y;
	int								hRet;
	static int							Frame = 0;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	static int							mousetyp = 0;
	int									i;
	bool								kintvan;
	bool								oldal[4];
	BlockType							BlDat;
	SDL_Rect							rcRect;

	static int color;

	for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		{
		  BlDat[x][y] = BlockDat[x][y];
		}

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > (Uint32)gameSpeed)
        {
            LastTick = ThisTick;
        }

// -------------------------------------------------------------------
	if ((MousX>=GetMaxX) && (MousY<=0)) {mousetyp = 2;} else    	//
	if ((MousX>=GetMaxX) && (MousY>=GetMaxY)) {mousetyp = 4;} else  //
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else    		//
	if ((MousX<=0)   && (MousY>=GetMaxY)) {mousetyp = 6;} else  	//
	if (MousX>=GetMaxX) {mousetyp = 3;} else						//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=GetMaxY) {mousetyp = 5;} else						//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=false;													//
	oldal[0]=false;oldal[2]=false;									//
	oldal[1]=false;oldal[3]=false;									//
																	//
	if (MousX>=GetMaxX) { MousX=GetMaxX; TerX-=16; }				//	
	if (MousX<=0)   { MousX=0;   TerX+=16; }						//	
	if (MousY>=GetMaxY) { MousY=GetMaxY; TerY-=12; }				//
	if (MousY<=0)   { MousY=0;   TerY+=12; }						//	
																	//
	if ((mousetyp == 2) && (oldal[0] != oldal[3])) kintvan=false;	//
	if ((mousetyp == 4) && (oldal[1] != oldal[3])) kintvan=false;	//
	if ((mousetyp == 6) && (oldal[2] != oldal[1])) kintvan=false;	//
	if ((mousetyp == 8) && (oldal[2] != oldal[0])) kintvan=false;	//
																	//
// -------------------------------------------------------------------
	
	y = (MousX-TerX+4*(8+MousY-TerY))/64;
	x = (MousX-TerX+8-16*y)/12;
	
	terep::DrawTerep(mousetyp,x,y);

	if (mouse::MInPr(611,GetMaxY-20,640,GetMaxY,0)) dot_plane=!dot_plane;
	if (mouse::MInPr(571,GetMaxY-20,610,GetMaxY,0)) wall_plane=!wall_plane;
	if (mouse::MInPr(541,GetMaxY-20,570,GetMaxY,0)) item_plane=!item_plane;
	if (mouse::MInPr(500,GetMaxY-20,540,GetMaxY,0)) block_plane=!block_plane;

	if (mouse::MInPr(250,0,390,12,0)) {
		GamePos = 0;
	}

	if (mouse::MInPr(380,GetMaxY-20,390,GetMaxY,0)) Selected++;
	if (mouse::MInPr(450,GetMaxY-20,GetMaxY-20,GetMaxY,0)) Selected--;
	
	if (mouse::Pressed(1)) Selected++;
	bool isPressed = false;
	if (mouse::Pressed(0)) isPressed = true;

	FRMLocationMap::iterator iter;
	iter = StaticInf->Map.begin();
	int Counter = 0;
	bool drawn = false;
	char buf2[150];
	sprintf(buf2,"");
	int ActualType = -1;
	color++;
	InventorySave = NULL;
	if (color>256) color=0;
	if (color<0) color=0;
	if (mouse::MouseIn(0,15,GetMaxX-100,GetMaxY-20))
	while (iter != StaticInf->Map.end()) {
		PFRM frm;
		int iy = DeCompLocY(iter->second->loc);
		int ix = DeCompLocX(iter->second->loc);
		if (iter->second->FRM != NULL) frm = iter->second->FRM->FRM;
			else frm = iter->second->FRMA->GetCurFrame();

		int cx = TerX+LocConvertX(ix,iy)*16+frm->bx;
		int cy = TerY+LocConvertY(ix,iy)*12+frm->by;



		if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
			if (Counter == Selected) {
				BlitToRo(g_pDDSBack,0,0,frm->x,frm->y,cx,cy,0,frm->FRM,palcal[color]);
				char buf[100];
				sprintf(buf2,"Location: %i (x: %i y: %i) Type: %i",iter->second->loc,
					DeCompLocX(iter->second->loc),
					DeCompLocY(iter->second->loc),
					iter->second->num);
				
				cy-=5;ActualType = iter->second->type;
				switch (iter->second->type) {
					case 0:
						textfont::IanOutTextR(cx,cy,1,"Wall");
						break;
					case 1:
						textfont::IanOutTextR(cx,cy,1,"Scenery");
						break;
					case 2:
						textfont::IanOutTextR(cx,cy,1,"Item");
						sprintf(buf,"Amount: %i",iter->second->state+1);
						textfont::IanOutText(cx+frm->x,cy,1,buf);
						if (mouse::Pressed(2)) { t2amount = iter->second->state+1; };
						if (mouse::Pressed(0)) { iter->second->state = t2amount-1; };
						break;
					case 3:
						textfont::IanOutTextR(cx,cy,1,"Object");
						textfont::IanOutTextR(cx,cy+15,1,iter->second->ItemDesc->desclocation);
						sprintf(buf,"Id: %i",iter->second->ItemDesc->Count);
						textfont::IanOutText(cx+frm->x,cy,1,buf);
						sprintf(buf,"Var_1: %i",iter->second->ItemDesc->var1);
						textfont::IanOutText(cx+frm->x,cy+15,1,buf);
						sprintf(buf,"Var_2: %i",iter->second->ItemDesc->var2);
						textfont::IanOutText(cx+frm->x,cy+30,1,buf);
						if (mouse::Pressed(2))
							{
								t3id = iter->second->ItemDesc->Count;
								t3var1 = iter->second->ItemDesc->var1;
								t3var2 = iter->second->ItemDesc->var2;
							};
						if (mouse::Pressed(0))
							{
								iter->second->ItemDesc->Count = t3id;
								iter->second->ItemDesc->var1 = t3var1;
								iter->second->ItemDesc->var2 = t3var2;
							};
						InventorySave = iter->second->ItemDesc->Inven;
						if (!iter->second->ItemDesc->Inven->Inven->Empty()) 
							textfont::IanOutText(cx+frm->x,cy+45,1,"Not Empty");
						break;
					case 4:
						textfont::IanOutTextR(cx,cy,1,"Exit grid");
						if (iter->second->ItemDesc->Count!=0) {
							sprintf(buf,"World Map");
						} else {
							sprintf(buf,"Entrance: %i",iter->second->ItemDesc->var2);
							textfont::IanOutText(cx+frm->x,cy+15,1,buf);
							sprintf(buf,"Location: %i",iter->second->ItemDesc->var1);
						}
						if (mouse::Pressed(2))
							{
								t4type = iter->second->ItemDesc->Count;
								t4loc = iter->second->ItemDesc->var1;
								t4entr = iter->second->ItemDesc->var2;
							};
						if (mouse::Pressed(0))
							{
								iter->second->ItemDesc->Count = t4type;
								iter->second->ItemDesc->var1 = t4loc;
								iter->second->ItemDesc->var2 = t4entr;
							};
						textfont::IanOutText(cx+frm->x,cy,1,buf);
						break;
					case 5:
						textfont::IanOutTextR(cx,cy,1,"Dead");
						textfont::IanOutTextR(cx,cy+15,1,iter->second->ItemDesc->desclocation);

						sprintf(buf,"Id: %i",iter->second->ItemDesc->Count);
						textfont::IanOutText(cx+frm->x,cy,1,buf);
						sprintf(buf,"Direction: %i",iter->second->ItemDesc->var1);
						textfont::IanOutText(cx+frm->x,cy+15,1,buf);
						sprintf(buf,"Type: %i",iter->second->ItemDesc->var2);
						textfont::IanOutText(cx+frm->x,cy+30,1,buf);

						if (mouse::Pressed(2))
							{
								t5id = iter->second->ItemDesc->Count;
							};
						if (mouse::Pressed(0))
							{
								iter->second->ItemDesc->Count = t5id;
							};
						InventorySave = iter->second->ItemDesc->Inven;
						if (!iter->second->ItemDesc->Inven->Inven->Empty()) 
							textfont::IanOutText(cx+frm->x,cy+45,1,"Not Empty");

						break;
				}
				drawn = true;
			}
			Counter++;
		}
		iter++;
	}
	if (!drawn) Selected = 0;

	ClearRect(g_pDDSBack,0,GetMaxY-20,GetMaxX,GetMaxY,0);
	ClearRect(g_pDDSBack,0,0,GetMaxX,15,0);
	ClearRect(g_pDDSBack,GetMaxX-100,15,GetMaxX,GetMaxY-20,0);

	textfont::IanOutText(0,GetMaxY-20,0,buf2);

	ref_to = NULL;
	bool InSide = false;
	char buf[150];
	textfont::IanOutTextR(GetMaxX,30,ActualType == 2 ? 0 : 1,"Items");
	textfont::IanOutTextR(GetMaxX,45,1,"------------");
	if (mouse::MouseIn(GetMaxX-100,60,GetMaxX,75))
		{ InSide=true; ref_to = &t2amount; } else { InSide=false; if (t2amount<1) t2amount=1; };
	sprintf(buf,"Amun: %i",t2amount);
	textfont::IanOutText(GetMaxX-100,60,InSide ? 0 : 1,buf);

	textfont::IanOutTextR(GetMaxX,90,ActualType == 3 ? 0 : 1,"Objects");
	textfont::IanOutTextR(GetMaxX,105,1,"------------");
	if (mouse::MouseIn(GetMaxX-100,120,GetMaxX,134))
		{ InSide=true; ref_to = &t3id; } else { InSide=false; if (t3id<0) t3id=0; };
	sprintf(buf,"Id: %i",t3id);
	textfont::IanOutText(GetMaxX-100,120,InSide ? 0 : 1,buf);
	if (mouse::MouseIn(GetMaxX-100,135,GetMaxX,149))
		{ InSide=true; ref_to = &t3var1; } else { InSide=false; };
	sprintf(buf,"Var_1: %i",t3var1); 
	textfont::IanOutText(GetMaxX-100,135,InSide ? 0 : 1,buf);
	if (mouse::MouseIn(GetMaxX-100,150,GetMaxX,164))
		{ InSide=true; ref_to = &t3var2; }  else { InSide=false; };
	sprintf(buf,"Var_2: %i",t3var2);
	textfont::IanOutText(GetMaxX-100,150,InSide ? 0 : 1,buf);

	textfont::IanOutTextR(GetMaxX,180,ActualType == 4 ? 0 : 1,"Exit grid");
	textfont::IanOutTextR(GetMaxX,195,1,"------------");
	if (t4type != 0) {
		if (mouse::MouseIn(GetMaxX-100,240,GetMaxX,254))
			{ InSide=true; } else {InSide = false; }
			if ((InSide) && (mouse::Pressed(0))) t4type = !t4type;
		sprintf(buf,"World Map");
		textfont::IanOutText(GetMaxX-100,240,InSide ? 0 : 1,buf);
	} else {
		if (mouse::MouseIn(GetMaxX-100,225,GetMaxX,239))
			{ InSide=true; ref_to = &t4loc; }  else { InSide=false; if (t4loc<0) t4loc=0; };
		sprintf(buf,"Entr: %i",t4loc);
		textfont::IanOutText(GetMaxX-100,225,InSide ? 0 : 1,buf);
		if (mouse::MouseIn(GetMaxX-100,210,GetMaxX,224))
			{ InSide=true; ref_to = &t4entr; }  else { InSide=false; if (t4entr<0) t4entr=0; };
		sprintf(buf,"Loc: %i",t4entr);
		textfont::IanOutText(GetMaxX-100,210,InSide ? 0 : 1,buf);
		if (mouse::MouseIn(GetMaxX-100,240,GetMaxX,254))
			{ InSide=true; } else {InSide = false; }
			if ((InSide) && (mouse::Pressed(0))) t4type = !t4type;
		sprintf(buf,"Location");
		textfont::IanOutText(GetMaxX-100,240,InSide ? 0 : 1,buf);
	}

	textfont::IanOutTextR(GetMaxX,270,ActualType == 5 ? 0 : 1,"Dead");
	textfont::IanOutTextR(GetMaxX,285,1,"------------");
	if (mouse::MouseIn(GetMaxX-100,300,GetMaxX,314))
			{ InSide=true; ref_to = &t5id; }  else { InSide=false; if (t5id<0) t5id=0; };
	sprintf(buf,"Id: %i",t5id);
	textfont::IanOutText(GetMaxX-100,300,InSide ? 0 : 1,buf);

	textfont::IanOutText(380,GetMaxY-20,mouse::MouseIn(380,GetMaxY-20,390,GetMaxY) ? 0 : 1,"+");
	textfont::IanOutText(450,GetMaxY-20,mouse::MouseIn(450,GetMaxY-20,460,GetMaxY) ? 0 : 1,"-");
	textfont::DisplayNum(390,GetMaxY-20,1,Selected,5);

	textfont::IanOutTextC(320,0,mouse::MouseIn(250,0,390,15) ? 0 : 1,"MAIN MENU");

	textfont::IanOutTextR(640,GetMaxY-20,dot_plane ? 0 : 1,"DOT");
	textfont::IanOutTextR(610,GetMaxY-20,wall_plane ? 0 : 1,"WALL");
	textfont::IanOutTextR(570,GetMaxY-20,item_plane ? 0 : 1,"ITEM");
	textfont::IanOutTextR(540,GetMaxY-20,block_plane ? 0 : 1,"BLCK");


    if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	
	textfont::DisplayNum(55,0,1,x,3);
	textfont::DisplayNum(85,0,1,y,3);
	textfont::DisplayNum(115,0,1,CompLoc(x,y),8);
	 
	  if ((mousetyp>0) && (mousetyp<9))
	  {
		if (kintvan) i=1; else i=0;
		x = MouseScr[mousetyp-1][i]->FRM->x;
		y = MouseScr[mousetyp-1][i]->FRM->y;
		if ((mousetyp==1) || (mousetyp>6))
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==2)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==3)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==4)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==5)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==6)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);

	}



	//DisplayNum(130,0,1,gameSpeed,2);
	textutil::DisplayFrameRate();

	// Flip the surfaces
    SDL_Flip(g_pDDSBack);
	
}
