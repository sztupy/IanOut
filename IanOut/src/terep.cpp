#include "normit.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/mouse.h"
#include "../commonutils/date.h"

void terep::DrawTerep(int mousetyp,int x,int y)
{
	//HRESULT hRet;

	int iy,ix;

	static int OTerX = 1000;
	static int OTerY = 1000;
	
	ClearSurface(FullScreen2->FRM->FRM,0,0,0);
	ClearSurface(g_pDDSBack,0,0,0);
	
		
	int bx,by;

	int IanX = CritterInf->Critters.find(0)->second->x;
	int IanY = CritterInf->Critters.find(0)->second->y;
	PFRMPlayer IanCritter = CritterInf->Critters.find(0)->second;
	int xXx;
	CritterList::iterator iter2;

	int coloring = 10;
	int color;
	if ((date::Hour>18) || (date::Hour<9)) coloring = 8;
	if (((date::Hour>18) && (date::Minute>30)) ||
	   ((date::Hour<9)) && (date::Minute<30)) coloring = 7;
	if ((date::Hour>19) || (date::Hour<8)) coloring = 6;
	if (((date::Hour>19) && (date::Minute>30)) ||
	   ((date::Hour<8)) && (date::Minute<30)) coloring = 5;
	if ((date::Hour>20) || (date::Hour<7)) coloring = 4;
	if (((date::Hour>20) && (date::Minute>30)) ||
	   ((date::Hour<7)) && (date::Minute<30)) coloring = 3;
	if ((date::Hour>21) || (date::Hour<6)) coloring = 2;
	if ((date::Hour>22) || (date::Hour<5)) coloring = 1;

	play::StepOnExit = false;

	if ((OTerX!= TerX) || (OTerY != TerY)) {
		ClearSurface(g_pDDSBack2,0,0,0);
		for (ix=0;ix<128;ix++)
		for (iy=1;iy<129;iy++)
		{
			int i = MapInf->Map[ix][iy-1];
			if ((i!=0) && (MapInf->Tiles.count(i) !=0)) {
				PFRM frm;
				frm = MapInf->Tiles[i]->FRM;
				BlitTo(g_pDDSBack2,0,0,frm->x,frm->y,frm->bx+TerX+ix*48+iy*32+7,frm->by+TerY-ix*12+iy*24+3,DDBLTFAST_SRCCOLORKEY,frm->FRM);
			}
		}
		OTerX = TerX;
		OTerY = TerY;
	}

	if (coloring<9) { 
		BlitToAlt(g_pDDSBack,0,0,GetMaxX,GetMaxY,0,0,0,g_pDDSBack2,coloring);

		bx = TerX+LocConvertX(IanX,IanY)*16-100;
		by = TerY+LocConvertY(IanX,IanY)*12-45;

		BlitToMask(FullScreen2->FRM->FRM,bx,by,bx+197,by+95,0,0,0,g_pDDSBack2,MaskBMP2);
		BlitToAlt(g_pDDSBack,0,0,197,95,TerX+LocConvertX(IanX,IanY)*16-100,TerY+LocConvertY(IanX,IanY)*12-45,DDBLTFAST_SRCCOLORKEY,FullScreen2->FRM->FRM,(coloring+3<9 ? coloring+3 : 9));
	}
	else BlitTo(g_pDDSBack,0,0,GetMaxX,GetMaxY,0,0,0,g_pDDSBack2);

	if ((mousetyp==0) && (mouse::MouseIn(0,0,GetMaxX,GetMaxY-100)))	{
		BlitTo(g_pDDSBack,0,0,32,16,TerX+LocConvertX(x,y)*16,TerY+LocConvertY(x,y)*12,DDBLTFAST_SRCCOLORKEY,Select->FRM->FRM);
	}

	int VisXB = -1;//(-TerX)/16+TerY/12-50; 
	int VisXE = 1024;//(-TerX)/16+TerY/12+50;
	int VisYB = -1;//(-TerY)/12-2;
	int VisYE = 1024;//(-TerY)/12+70;

	if (VisXB<0) VisXB=0;
	if (VisXE<0) VisXE=0;
	if (VisYB<0) VisYB=0;
	if (VisYE<0) VisYE=0;

	if (VisXB>=512) VisXB=512;
	if (VisXE>=512) VisXE=512;
	if (VisYB>=512) VisYB=512;
	if (VisYE>=512) VisYE=512;

	 
	int numberitems = 0;
	FRMLocationMap::reverse_iterator iter;
	if (!StaticInf->Map.empty()) 
	{
	
	iter = StaticInf->Map.rbegin();
	while (iter != StaticInf->Map.rend())
	{
		 numberitems++;
		 iy = DeCompLocY(iter->second->loc);
		 ix = DeCompLocX(iter->second->loc);

		 if ((iy>=VisYB) && (iy<=VisYE) && (ix>=VisXB) && (ix<=VisXE)) {

		 PFRM frm;
		 if (iter->second->FRM)
			 frm = iter->second->FRM->FRM;
		 else
			 frm = iter->second->FRMA->GetCurFrame();


		 	color = coloring;
			xXx = IanCritter->Distance(ix,iy);
			if (xXx<6) color+=(6-xXx);
		
		if (iter->first != 2000000000) {
			iter2 = CritterInf->Critters.begin();
			while (iter2 !=CritterInf->Critters.end()) {
				if ((!iter2->second->AlreadyDrawn) && (iter2->second->conv >= iter->first)) {
					iter2->second->AlreadyDrawn = true;
					iter2->second->Draw(g_pDDSBack,TerX,TerY,coloring);
				}
				iter2++;
			}
		 }
		 

		if (color>8) {
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,TerX+LocConvertX(ix,iy)*16+frm->bx,TerY+LocConvertY(ix,iy)*12+frm->by,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		} else {
			BlitToAlt(g_pDDSBack,0,0,frm->x,frm->y,TerX+LocConvertX(ix,iy)*16+frm->bx,TerY+LocConvertY(ix,iy)*12+frm->by,DDBLTFAST_SRCCOLORKEY,frm->FRM,color);
		}
	}

		 if (iter->second->type==4) {
				 int ILX = play::Ian->x;
				 int ILY = play::Ian->y;
				 int DEX=ix;
				 int DEY=iy;
				 switch (iter->second->num%4) {
					 case 0:
						 //if ((ILY==DEY-1) && (ILX>=DEX  ) && (ILX<=DEX+6)) textfont::DrawChar(50,50,0,'X'); else
#define ONSTEP { play::StepOnExit = true; play::StepType = iter->second->ItemDesc->Count != 0; play::StepLocation = iter->second->ItemDesc->var1; play::StepEntrance = iter->second->ItemDesc->var2; }
						 if ((ILY==DEY  ) && (ILX>=DEX-1) && (ILX<=DEX+5)) ONSTEP else
						 //if ((ILY==DEY+1) && (ILX>=DEX-2) && (ILX<=DEX+4)) textfont::DrawChar(50,50,0,'X');
						 break;
					 case 1:
						 DEX = DEY+DEX;
						 ILX = ILY+ILX;
						 if ((ILX==DEX-1) && (ILX<=DEX+1) && (ILY>=DEY-7) && (ILY<=DEY+1)) ONSTEP
						 break;
					 case 2:
						 //if ((ILY==DEY-3) && (ILX==DEX+6)) textfont::DrawChar(50,50,0,'X'); else
						 if ((ILY==DEY-2) && (ILX>=DEX+2) && (ILX<=DEX+6)) ONSTEP else
						 if ((ILY==DEY-1) && (ILX>=DEX-2) && (ILX<=DEX+6)) ONSTEP else
						 if ((ILY==DEY  ) && (ILX>=DEX-2) && (ILX<=DEX+2)) ONSTEP //else
						 //if ((ILY==DEY+1) && (ILX==DEX-2)) textfont::DrawChar(50,50,0,'X');
						 break;
					 case 3:
						 if ((ILX>=DEX-2) && (ILX<=DEX+2) && (ILY>=DEY-3) && (ILY<=DEY)) ONSTEP //else
						 //if ((ILX==DEX-2) && (ILY==DEY+1)) textfont::DrawChar(50,50,0,'X'); else
						 //if ((ILX==DEX+2) && (ILY==DEY-4)) textfont::DrawChar(50,50,0,'X');
						 break;
#undef ONSTEP
				 }
			 }

		iter++;
	}
	}

	iter2 = CritterInf->Critters.begin();
	while (iter2 != CritterInf->Critters.end()) {
	if ((!iter2->second->AlreadyDrawn)) {
		iter2->second->AlreadyDrawn = true;
		iter2->second->Draw(g_pDDSBack,TerX,TerY,coloring);
	}
	if (play::DrawOutline) if (iter2->first != 0) iter2->second->Draw(g_pDDSBack,TerX,TerY,1000);
	iter2++;
	}

	//BlitToRo(g_pDDSBack,0,0,Select->FRM->x,Select->FRM->y,635,475,0,Select->FRM->FRM,52);
	textfont::DisplayNum(0,20,0,numberitems,4);

}