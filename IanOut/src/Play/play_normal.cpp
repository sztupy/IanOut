#include "normit.h"
#include "math.h"
#include "../commonutils/palette.h"

namespace play {
	int							NumChState = 0;

	int							Frame = 0;
	DWORD						ThisTick;
	DWORD						LastTick = 0;
	int							mousetyp = 0;
	int							menet = 0;
	int							secondinc = 0;
	std::bitset<10>				ability;
	CritterList::iterator		iter2save;
	FRMLocationMap::iterator	itersave;
	PFRM						FRMMouse;
	BlockType					BlDat;
	int							addx;
	int							addy;
	PFRMPlayer					Ian;

	bool						DrawOutline = false;

	bool						StepOnExit = false;
	bool						StepType = false;
	bool						TurnBasedMode = false;

	bool						AllowUserInteract = true;

	int							StepLocation = 0;
	int							StepEntrance = 0;

	PFRMPlayer					CurrentPlayer;

	PInventory					InvenT = NULL;
	PInventoryItem				InvenTItem = NULL;
}

void play::CountVisible(void)
{
 	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2 != CritterInf->Critters.end()) {
		if (iter2->first!=0) {
			double sx = (iter2->second->x+iter2->second->y)*16;
			double sy = iter2->second->y*12;
			double dx = (Ian->x+Ian->y)*16;
			double dy = Ian->y*12;
			bool visible = true;
			while (!((sx == dx) && (sy == dy))) {
				double xdest = dx-sx;
				double ydest = dy-sy;
				double destination = sqrt(xdest*xdest+ydest*ydest);
				sx+=xdest/destination*0.5;
				sy+=ydest/destination*0.5;
				if (abs(sx-dx)<=5) sx=dx;
				if (abs(sy-dy)<=5) sy=dy;
				int ssy = sy/12;
				int ssx = sx/16-ssy;
				if ((ssx<0) || (ssy<0) || (ssx>=512) || (ssy>=512) || (BlockDat[ssx][ssy]==1)) {
					visible = false;
					break;
				}
			}
			if (visible) iter2->second->visibletype=137; else iter2->second->visibletype=60;
		}
		iter2++;
	}
}

void play::NormalPlay(void)
{
	int									x,y;
	HRESULT								hRet;
	int									i;
	bool								kintvan;
	PItem								TheItem;
    
	addx = (GetMaxX-640)/2;
	addy = (GetMaxY-480)/2;

	if (textutil::page<0) textutil::page=0;
	if (textutil::page>44) textutil::page=44;

	wsprintf(FullScreen->fname,"");

	for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		{
		  BlDat[x][y] = BlockDat[x][y];
		}

	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2 != CritterInf->Critters.end()) {
		BlDat[iter2->second->x][iter2->second->y] = 1;
		iter2->second->AlreadyDrawn = false;
		iter2->second->conv = CompLoc(iter2->second->x,iter2->second->y);		
		iter2++;
	}
	IanOs::BigBlock = &BlockDat;

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = GetTickCount();

	bool changestate = false;

	if ((ThisTick - LastTick) > (DWORD)gameSpeed)
	{
		changestate = true;
		NumChState++;
		LastTick = ThisTick;
		OnNextFr();
	}

	if (CritterInf->Critters.find(0)==CritterInf->Critters.end()) { InitFail(hWnd,DD_OK,"GAME OVER");return; }
	Ian = CritterInf->Critters.find(0)->second;
	if (mousetyp==22) DrawOutline = true; else DrawOutline=false;
	if (DrawOutline) CountVisible();

// -------------------------------------------------------------------
	olddims=dims;													//
	mouse::UpdateInputState();										//
	if ((dims.lX == olddims.lX) && (dims.lY == olddims.lY)) Frame++; else Frame=0;
																	//
	if ((mousetyp<10) || (mousetyp>=20)) {							//
		MousX += dims.lX;											//
		MousY += dims.lY;											//
	} else {														//
		menet -= dims.lY;											//
		mousetyp = 10+menet/10;										//
		if (mousetyp>=20) mousetyp=19;								//
		if (mousetyp<10) mousetyp=10;								//
		if (dims.lY>0) {											//
			while ((mousetyp>9) && !ability[mousetyp-10]) {mousetyp--;menet-=10;}
		} else {													//
			while ((mousetyp<20) && !ability[mousetyp-10]) {mousetyp++;menet+=10;}
		}															//
		if (mousetyp>=20) for (i=0; i<10; i++) if (ability[i]) {mousetyp = 10+i;menet=5+i*10;}
		if (mousetyp<10) for (i=9; i>=0; i--) if (ability[i]) {mousetyp = 10+i;menet=5+i*10;}
		if (menet<0) menet=0;										//
		if (menet>100) menet=100;									//
	}																//
																	//
	if ((MousX>=GetMaxX) && (MousY<=0)) {mousetyp = 2;} else		//
	if ((MousX>=GetMaxX) && (MousY>=GetMaxX)) {mousetyp = 4;} else	//
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else			//
	if ((MousX<=0)   && (MousY>=GetMaxY)) {mousetyp = 6;} else		//
	if (MousX>=GetMaxX) {mousetyp = 3;} else						//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=GetMaxY) {mousetyp = 5;} else						//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=false;													//
	int OTerX = TerX;												//
	int OTerY = TerY;												//
																	//
	if (MousX>=GetMaxX) { MousX=GetMaxX; TerX-=16; }				//
	if (MousX<=0)   { MousX=0;   TerX+=16; }						//
	if (MousY>=GetMaxY) { MousY=GetMaxY; TerY-=12; }				//
	if (MousY<=0)   { MousY=0;   TerY+=12; }						//
																	//
	if (TerepBoundType) {
		int ix,iy;
		bool b;
		while (true) {
			b = true;
			iy = (-TerX+4*(8-TerY))/64;
			ix = (-TerX+8-16*iy)/12;
			if (iy<TerepBound.top) { TerY-=12; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (-TerX+4*(8+GetMaxY-100-TerY))/64;
			ix = (-TerX+8-16*iy)/12;
			if (ix<TerepBound.left) { TerX-=16; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (GetMaxX-TerX+4*(8-TerY))/64;
			ix = (GetMaxX-TerX+8-16*iy)/12;
			if (ix>TerepBound.left+TerepBound.right) { TerX+=16; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (GetMaxX-TerX+4*(8+GetMaxY-100-TerY))/64;
			ix = (GetMaxX-TerX+8-16*iy)/12;
			if (iy>TerepBound.top+TerepBound.bottom) { TerY+=12; b = false; }
			if (b) break;
		} 
	}
	if (!TerepBoundType) {
		int ix = TerepBound.left;
		int iy = TerepBound.top;
		if (TerX > -LocConvertX(ix-iy-iy/3,iy+ix/4)*16) {
			TerX = -LocConvertX(ix-iy-iy/3,iy+ix/4)*16;
		}
		ix = TerepBound.left+TerepBound.right;
		iy = TerepBound.top;
		if (TerX < GetMaxX-LocConvertX(ix-iy-iy/3,iy+ix/4)*16) {
			TerX = GetMaxX-LocConvertX(ix-iy-iy/3,iy+ix/4)*16;
		}
		ix = TerepBound.left;
		iy = TerepBound.top;
		if (TerY > -LocConvertY(ix-iy-iy/3,iy+ix/4)*12) {
			TerY = -LocConvertY(ix-iy-iy/3,iy+ix/4)*12;
		}
		ix = TerepBound.left;
		iy = TerepBound.top+TerepBound.bottom;
		if (TerY < GetMaxY-100-LocConvertY(ix-iy-iy/3,iy+ix/4)*12) {
			TerY = GetMaxY-100-LocConvertY(ix-iy-iy/3,iy+ix/4)*12;
		}
	}
	if ((TerX == OTerX) && (TerY == OTerY)) kintvan = true;			//
																	//
// -------------------------------------------------------------------

	if ((InGamePos == 0 ) && ((dims.rgbButtons[1] & 0x80) && !(olddims.rgbButtons[1] & 0x80)) )
	if (TurnBasedMode) {
		if (mousetyp==0) { mousetyp=9; } else
		if (mousetyp==9) {
			PWeapon Weapon;
			if (Ian->curhand) Weapon = Ian->Hand1; else Weapon = Ian->Hand2;
			if ((Weapon->GetCurrentMode()) && (Weapon->GetCurrentMode()->mode == 2)) {
				mousetyp = 22;
			} else {
				mousetyp = 0;
			}
		} else mousetyp=0;
	} else {
		if (mousetyp==0) { mousetyp=9; } else { mousetyp=0; }
	}
	if ((InGamePos != 0) && (mousetyp==0)) mousetyp = 9;
	if (!AllowUserInteract) mousetyp = 9;

	y = (MousX-TerX+4*(8+MousY-TerY))/64;
	x = (MousX-TerX+8-16*y)/12;
	if ((x%2)==1) x++;

	/*if (((-TerY+MousY)%24)<12)
	{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-y;
		if (x%2==1) x++;
		if (y%2==1) x++;
	} else
	{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-1-y;
		if (x%2==0) x--;
		if (y%2==1) x++;
	}*/

	unsigned int CurPosition = CompLoc(x,y);

	if ((lstrcmpi(TempAnim[0]->fname,"\\art\\intrface\\endanim.frm")))
	{
    	LoadFRMAnim(&TempAnim[0],hWnd,"\\art\\intrface\\endanim.frm");
		TempAnim[0]->FirstFrame();
	}

	
	//CritterInf->Critters.find(0)->second->LookAt(x,y);

	terep::DrawTerep(mousetyp,x,y);

	textfont::DisplayNum(15,35,0,Ian->Distance(x,y),3);

	TheItem = TauntList->First();
	while (TheItem != NULL) {
		PTauntObj FRMItem = (PTauntObj)TheItem;
		textfont::IanOutTextC(TerX+FRMItem->x,TerY+FRMItem->y,FRMItem->co,FRMItem->Taunt);
		TheItem = TauntList->Next(TheItem);
	}

	ClearRect(g_pDDSBack,0,GetMaxY-99,GetMaxX,GetMaxY,0);

	if (AllowUserInteract) {
		if ((!TurnBasedMode) || (CurrentPlayer == Ian)) {
			if (InGamePos == 0) { NormalMode(); }
		} else { AiMove(); }
		if (InGamePos == 1)	{ InventoryMode(); }
		if (InGamePos == 2) { LootMode(); }
	}
	if (InGamePos == 0) {
		if (AllowUserInteract) {
		int bdown = 0;
		bdown = ButtonList->GetButton(MousX,MousY,mouse::Pressed(6) || mouse::Pressed(7));
		if (mouse::Pressed(4) && (bdown != 100)) bdown = 0;
		if (bdown == 1) {
			if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=3))) {
				textutil::AddString("Not enough Action Points",1);
			} else {
				if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=4;
				buttons::ChangeButtonList(ButtonList,11);
				InGamePos = 1;mousetyp = 9;
			}
		}
		if (bdown == 2) {
			Ian->ChangeHand(hWnd,g_pDD);
			Ian->LoadPlayer(hWnd,g_pDD,true);
		}
		if (bdown == 50) {
			CountVisible();
			bool CanExit = true;
			CritterList::iterator iter = CritterInf->Critters.begin();
			while (iter!=CritterInf->Critters.end()) {
				if ((iter->first!=0) && (iter->second->visibletype!=60)) { CanExit = false; break; }
				iter++;
			}
			if (CanExit) {
				TurnBasedMode = false;
				DrawOutline = false;
				mousetyp = 0;
				buttons::ChangeButtonList(ButtonList,2);
			} else {
				textutil::AddString("Cannot end combat with nearby hostile creatures",1);
			}
		}
		if (bdown == 51) {
			if (CurrentPlayer==Ian)
				NextInTurn();
		}
		if (bdown == 100) {
			if (mouse::Pressed(4)) {
				if (Ian->curhand) {
					Ian->Hand1->NextMode();
				} else {
					Ian->Hand2->NextMode();
				}
			} else {
				PWeapon Weapon;
				if (Ian->curhand) Weapon = Ian->Hand1; else Weapon = Ian->Hand2;
				if (Weapon->GetCurrentMode()) {
					if (Weapon->GetCurrentMode()->mode == 10) {
						int numleft = Ian->Inven->CountItem(Weapon->ammotype,0);
						int need = Weapon->capacity-Weapon->numammo;
						//AddToLog("%i %i %i %i",numleft,need,Weapon->capacity,Weapon->numammo);
						if ((need!=0) && (numleft>0))
						if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=Weapon->GetCurrentMode()->apuse-1))) {
							textutil::AddString("Not enough Action Points",1);
						} else {
							if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=Weapon->GetCurrentMode()->apuse;
							if (numleft>=need) {
								Weapon->numammo = Weapon->capacity;
								delete Ian->Inven->GetItem(Weapon->ammotype,0,need);
							} else {
								Weapon->numammo = Weapon->numammo+numleft;
								delete Ian->Inven->GetItem(Weapon->ammotype,0,numleft);
							}
							if (Ian->curhand) {
								Ian->Hand1->NextMode();
							} else {
								Ian->Hand2->NextMode();
							}
						}
					} else
					if (Weapon->GetCurrentMode()->mode == 1) {
						mousetyp = 21;
					} else
					if (Weapon->GetCurrentMode()->mode == 2) {
						if (!TurnBasedMode) {
							TurnBasedMode = true;
							DrawOutline = true;
							CurrentPlayer = Ian;
							ianskill::CountSk(&CurrentPlayer->PlayerSkill);
							buttons::ChangeButtonList(ButtonList,3);
						}
						mousetyp = 22;
					}
				}
			}
		}
		}
		if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-99))
		{
			if (AllowUserInteract)
			if (mousetyp==0) BlitTo(g_pDDSBack,0,0,32,16,TerX+LocConvertX(x,y)*16,TerY+LocConvertY(x,y)*12,DDBLTFAST_SRCCOLORKEY,Select2->FRM->FRM);
			if ((x>=0) && (y>=0) && (x<512) && ( y<512))
			if ((mousetyp==0) && (BlDat[x][y] != 0) && (Frame>40) && !((x==Ian->x) && (y==Ian->y))) textfont::IanOutTextC(TerX+(x+y)*16+16,TerY+y*12,0,"X");
			BlitTo(g_pDDSBack, 0,0,640,99,addx,GetMaxY-99,0,KezeloP->FRM->FRM);
			textutil::DrawStuff();

			if (changestate) {
				if ((TurnBasedMode) && (!TempAnim[0]->GetLastFrame())) TempAnim[0]->NextFrame();
				if ((!TurnBasedMode) && (!TempAnim[0]->GetFirstFrame())) TempAnim[0]->PrevFrame();
			}
			PFRM frm = TempAnim[0]->GetCurFrame();
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+580,GetMaxY-99+38,DDBLTFAST_SRCCOLORKEY,frm->FRM);

			ButtonList->DrawButtons(g_pDDSBack,MousX,MousY,mouse::Pressed(6) || mouse::Pressed(7));
			DrawIanCucc(mouse::MouseIn(addx+266,GetMaxY-99+27,addx+266+188,GetMaxY-99+27+67) && (mouse::Pressed(6) || mouse::Pressed(7)));
			if ((TurnBasedMode) && (CurrentPlayer == Ian))
			for (int i=0; i<Ian->PlayerSkill.secondary[1]; i++) {
				BlitTo(g_pDDSBack,0,0,5,5,addx+315+i*9,GetMaxY-99+14,DDBLTFAST_SRCCOLORKEY,Buttons[23][0]->FRM->FRM);
			}
			if (AllowUserInteract)
			if ((mousetyp>=9) && (mousetyp<=20)) BlitTo(g_pDDSBack,0,0,Mouse2->FRM->x,Mouse2->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse2->FRM->FRM);
			if (AllowUserInteract)
			if (mousetyp==21) {
				BlitTo(g_pDDSBack,0,0,21,21,MousX-10,MousY-10,DDBLTFAST_SRCCOLORKEY,Buttons[24][0]->FRM->FRM);
			}
			if (AllowUserInteract)
			if (mousetyp==22) {
				BlitTo(g_pDDSBack,0,0,21,21,MousX-10,MousY-10,DDBLTFAST_SRCCOLORKEY,Buttons[24][1]->FRM->FRM);
			}
		} else
		{
			BlitTo(g_pDDSBack, 0,0,640,99,addx,GetMaxY-99,0,KezeloP->FRM->FRM);
			textutil::DrawStuff();

			if (changestate) {
				if ((TurnBasedMode) && (!TempAnim[0]->GetLastFrame())) TempAnim[0]->NextFrame();
				if ((!TurnBasedMode) && (!TempAnim[0]->GetFirstFrame())) TempAnim[0]->PrevFrame();
			}
			PFRM frm = TempAnim[0]->GetCurFrame();
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+580,GetMaxY-99+38,DDBLTFAST_SRCCOLORKEY,frm->FRM);

			ButtonList->DrawButtons(g_pDDSBack,MousX,MousY,mouse::Pressed(6) || mouse::Pressed(7));

			DrawIanCucc(mouse::MouseIn(addx+266,GetMaxY-99+27,addx+266+188,GetMaxY-99+27+67) && (mouse::Pressed(6) || mouse::Pressed(7)));

			if ((TurnBasedMode) && (CurrentPlayer == Ian))
			for (int i=0; i<Ian->PlayerSkill.secondary[1]; i++) {
				BlitTo(g_pDDSBack,0,0,5,5,addx+315+i*9,GetMaxY-99+14,DDBLTFAST_SRCCOLORKEY,Buttons[23][0]->FRM->FRM);
			}
			if (AllowUserInteract)
			if (mouse::MouseIn(addx+20,GetMaxY-99+20,addx+190,GetMaxY-99+50)) {
				BlitTo(g_pDDSBack,0,0,Arrows[0]->FRM->x, Arrows[0]->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Arrows[0]->FRM->FRM);
				if (mouse::Pressed(0)) {
					textutil::page++;
					if (textutil::page>44) textutil::page=44;
				}
			} else
			if (mouse::MouseIn(addx+20,GetMaxY-99+50,addx+190,GetMaxY-99+80)) {
				BlitTo(g_pDDSBack,0,0,Arrows[1]->FRM->x, Arrows[1]->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Arrows[1]->FRM->FRM);
				if (mouse::Pressed(0)) {
					textutil::page--;
					if (textutil::page<0) textutil::page=0;
				}
			} else 
			if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
		}
		
	}

	int colornum = 8;
	if (Ian->PlayerSkill.HP<=Ian->PlayerSkill.AlapSk.HP/2) colornum = 9;
	if (Ian->PlayerSkill.HP<=Ian->PlayerSkill.AlapSk.HP/4) colornum = 10;

	textfont::DisplayNum(addx+469,GetMaxY-99+39,colornum,Ian->PlayerSkill.HP,3);

	textfont::DisplayNum(GetMaxX-138,0,1,date::Year,4);
	textfont::DisplayNum(GetMaxX-105,0,1,date::Month,2);
	textfont::DisplayNum(GetMaxX- 85,0,1,date::Day,2);
	textfont::DisplayNum(GetMaxX- 65,0,1,date::Hour,2);
	textfont::DisplayNum(GetMaxX- 45,0,1,date::Minute,2);
	textfont::DisplayNum(GetMaxX- 25,0,1,date::Second,2);
	textfont::DisplayNum(0,50,1,mousetyp,2);

	if (!AllowUserInteract) {
		if ((changestate) && (NumChState%2==0)) WaitMouse->NextFrame();
		PFRM frm = WaitMouse->GetCurFrame();
		BlitTo(g_pDDSBack,0,0,frm->x,frm->y,MousX+frm->bx,MousY+frm->by,DDBLTFAST_SRCCOLORKEY,frm->FRM);
	}

	//if (AllowUserInteract)
	if ((mousetyp>0) && (mousetyp<9))
	{
		if (kintvan) i=1; else i=0;
		x = MouseScr[mousetyp-1][i]->FRM->x;
		y = MouseScr[mousetyp-1][i]->FRM->y;
		if ((mousetyp==1) || (mousetyp>6))
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==2)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==3)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==4)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==5)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==6)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
	}

	if (AllowUserInteract)
	if ((InGamePos < 3) && ((!TurnBasedMode) || (CurrentPlayer == Ian))) {
		if ((mousetyp>9) && (mousetyp<21)) {
			int i2=0;
			//for (int xXx=0; xXx<10; xXx++) if (ability[xXx]) i2++;
			i = 0;
			for (int xXx=9; xXx>=0; xXx--) {
				if (ability[xXx]) {
					BlitTo(g_pDDSBack,0,0,40,40,MousX+45,MousY+i*40,0,MenuScr[xXx][(mousetyp-10 == xXx)?0:1]->FRM->FRM);
					i++;
				}
				//BlitTo(g_pDDSBack,0,0,40,40,MousX+45,MousY-40,0,MenuScr[mousetyp - 10][0]->FRM->FRM);
			}
		}
	}

	textfont::DisplayNum(70,0,1,x,3);
	textfont::DisplayNum(100,0,1,y,3);
	textfont::DisplayNum(130,0,1,gameSpeed,2);
	/*textfont::DisplayNum(200,0,1,y^511,3);
	textfont::DisplayNum(250,0,1,x,3);*/
	textfont::DisplayNum(250,0,1,CurPosition,8);
	textutil::DisplayFrameRate();

	if (StepOnExit) {
		if (StepType) {
			TerX = -TravelMapX+GetMaxX/2;
			TerY = -TravelMapY+GetMaxY/2;
			GamePos = 6;
		} else {
			palette::FadeOut();
			StepOnExit = false;
			LoadLocation(StepLocation,StepEntrance);
			NormalPlay();
			palette::FadeIn();
			return;
		}
	}

	if (AllowUserInteract)
	if ((InGamePos == 0) && ((!TurnBasedMode) || (CurrentPlayer == Ian))) {
		if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-101) && (mousetyp>9) && (mousetyp<21) && !(dims.rgbButtons[0] & 0x80)) 
		{
			menet = mousetyp-10;
			mousetyp = 9;
		}
		if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-101) && (mousetyp==0) && (dims.rgbButtons[0] & 0x80) && !(olddims.rgbButtons[0] & 0x80))
		if ((Ian->cx == x) && (Ian->cy == y) &&
		((Ian->curth == 1) || (Ian->curth == 2)))
		{
			if (Ian->curth == 1) {
			Ian->curth = 2;
			} else {
			Ian->curth = 1;
			}
		} else
		{
			if (x<0) x = 0;
			if (y<0) y = 0;
			if (x>=512) x=511;
			if (y>=512) y=511;
			Ian->GenerateTree(x,y,BlDat);
		}
	}
    // Flip the surfaces
    while (true)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if  (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = RestoreAll();
            if (hRet != DD_OK)
                break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            break;
    }
	
}
