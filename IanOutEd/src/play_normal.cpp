#include "windows.h"

#include "play.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

extern PFRMSingle ExitGrid[12];

void play::EditTerep(void)
{
    int                         x,y;
	int                     hRet;
	static int							Frame = 0;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	static int							mousetyp = 0;
	int									i,ix,iy;
	bool								kintvan;
	BlockType							BlDat;

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
	if ((MousX>=GetMaxX) && (MousY<=0)) {mousetyp = 2;} else    		//
	if ((MousX>=GetMaxX) && (MousY>=GetMaxY)) {mousetyp = 4;} else  		//
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else    		//
	if ((MousX<=0)   && (MousY>=GetMaxY)) {mousetyp = 6;} else  		//
	if (MousX>=GetMaxX) {mousetyp = 3;} else							//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=GetMaxY) {mousetyp = 5;} else							//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=false;													//

	int OTerX = TerX;
	int OTerY = TerY;

	if (MousX>=GetMaxX) { MousX=GetMaxX; TerX-=16; }						//	
	if (MousX<=0)   { MousX=0;   TerX+=16; }						//	
	if (MousY>=GetMaxY) { MousY=GetMaxY; TerY-=12; }						//	
	if (MousY<=0)   { MousY=0;   TerY+=12; }						//	

	TRect TerepBound;
	TerepBound.top = BoundRect.top;
	TerepBound.left = BoundRect.left;
	TerepBound.bottom = BoundRect.bottom+BoundRect.top;
	TerepBound.right = BoundRect.right+BoundRect.left;

	if (BoundTest && BoundType) {
		int ix,iy;
		bool b;
		while (true) {
			b = true;
			iy = (-TerX+4*(8-TerY))/64;
			ix = (-TerX+8-16*iy)/12;
			if (iy<BoundRect.top) { TerY-=12; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (-TerX+4*(8+GetMaxY-20-TerY))/64;
			ix = (-TerX+8-16*iy)/12;
			if (ix<BoundRect.left) { TerX-=16; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (GetMaxX-90-TerX+4*(8-TerY))/64;
			ix = (GetMaxX-90-TerX+8-16*iy)/12;
			if (ix>BoundRect.left+BoundRect.right) { TerX+=16; b = false; }
			if (b) break;
		} 
		while (true) {
			b = true;
			iy = (GetMaxX-90-TerX+4*(8+GetMaxY-20-TerY))/64;
			ix = (GetMaxX-90-TerX+8-16*iy)/12;
			if (iy>BoundRect.top+BoundRect.bottom) { TerY+=12; b = false; }
			if (b) break;
		} 
	}
	if (BoundTest && !BoundType) {
		int ix = BoundRect.left;
		int iy = BoundRect.top;
		if (TerX > -LocConvertX(ix-iy-iy/3,iy+ix/4)*16) {
			TerX = -LocConvertX(ix-iy-iy/3,iy+ix/4)*16;
		}
		ix = BoundRect.left+BoundRect.right;
		iy = BoundRect.top;
		if (TerX < GetMaxX-90-LocConvertX(ix-iy-iy/3,iy+ix/4)*16) {
			TerX = GetMaxX-90-LocConvertX(ix-iy-iy/3,iy+ix/4)*16;
		}
		ix = BoundRect.left;
		iy = BoundRect.top;
		if (TerY > -LocConvertY(ix-iy-iy/3,iy+ix/4)*12) {
			TerY = -LocConvertY(ix-iy-iy/3,iy+ix/4)*12;
		}
		ix = BoundRect.left;
		iy = BoundRect.top+BoundRect.bottom;
		if (TerY < GetMaxY-25-LocConvertY(ix-iy-iy/3,iy+ix/4)*12) {
			TerY = GetMaxY-25-LocConvertY(ix-iy-iy/3,iy+ix/4)*12;
		}
	}
	if ((TerX == OTerX) && (TerY == OTerY)) kintvan = true;			//
																	//
// -------------------------------------------------------------------
	
	y = (MousX-TerX+4*(8+MousY-TerY))/64;
	x = (MousX-TerX+8-16*y)/12;

	int xx2 = x/4;
	int yy2 = y/2-1;
	
	terep::DrawTerep(mousetyp,x,y);

	ClearRect(g_pDDSBack,0,GetMaxY-20,GetMaxX,GetMaxY,0);
	ClearRect(g_pDDSBack,0,0,GetMaxX,15,0);

    FRMCollection::iterator iter;
    FRMCollection *point = NULL;

	FRMPairCollection::iterator iterP;
    FRMPairCollection *pointP = NULL;

	FRMDeadCollection::iterator iterD;
    FRMDeadCollection *pointD = NULL;
	
	switch (putmode)
	{
		case 0:textfont::IanOutText(0,GetMaxY-20,1,"ROOF");point=&MapInf->Tiles;break;
		case 1:textfont::IanOutText(0,GetMaxY-20,1,"WALL");point=&StaticInf->TilesW;break;
		case 2:textfont::IanOutText(0,GetMaxY-20,1,"SCEN");point=&StaticInf->TilesS;break;
		case 3:textfont::IanOutText(0,GetMaxY-20,1,"BLCK");break;
		case 4:textfont::IanOutText(0,GetMaxY-20,1,"TILE");point=&MapInf->Tiles;break;
		case 5:textfont::IanOutText(0,GetMaxY-20,1,"ITEM");pointP=&StaticInf->TilesI;break;
		case 6:textfont::IanOutText(0,GetMaxY-20,1,"OBJ ");pointP=&StaticInf->TilesO;break;
		case 7:textfont::IanOutText(0,GetMaxY-20,1,"EXIT");break;
		case 8:textfont::IanOutText(0,GetMaxY-20,1,"DEAD");pointD=&StaticInf->TilesD;break;
		case 9:textfont::IanOutText(0,GetMaxY-20,1,"UTIL");break;
	}

	ClearRect(g_pDDSBack,GetMaxX-80,15,GetMaxX,GetMaxY-20,0);
	if (point) {
		if (point->count(Selected) == 0) {
			if (point->lower_bound(Selected) != point->end()) {
                Selected = point->lower_bound(Selected)->first;
			}
		}

	 iter = point->lower_bound(Selected);
	 for (ix=0; ix<15; ix++) if (iter != point->begin()) iter--;

	 ix = 0;
	 while ((iter != point->end()) && (ix<(GetMaxY-35)/12))
	 {
		 iy = iter->first;
		 textfont::DisplayNum(GetMaxX-80,15+ix*12,Selected == iy ? 0 : 1,iy,5);
		 iter++;ix++;
	 }
	 if (point->count(Selected) != 0) BlitTo(g_pDDSBack,0,0,(*point)[Selected]->FRM->x,(*point)[Selected]->FRM->y,GetMaxX-78,GetMaxY-20-(*point)[Selected]->FRM->y,0,(*point)[Selected]->FRM->FRM);
	}

	if (pointP) {
		if (pointP->count(Selected) == 0) {
			if (pointP->lower_bound(Selected) != pointP->end()) {
                Selected = pointP->lower_bound(Selected)->first;
			}
		}

		iterP = pointP->lower_bound(Selected);
		for (ix=0; ix<15; ix++) if (iterP != pointP->begin()) iterP--;

		ix = 0;
		while ((iterP != pointP->end()) && (ix<(GetMaxY-35)/12))
		{
			iy = iterP->first;
			textfont::DisplayNum(GetMaxX-80,15+ix*12,Selected == iy ? 0 : 1,iy,5);
			iterP++;ix++;
		}
		if (pointP->count(Selected) != 0) {
			PFRM frm;
			if ((*pointP)[Selected]->FRMA)
				frm = (*pointP)[Selected]->FRMA->GetCurFrame();
			else
				frm = (*pointP)[Selected]->FRM->FRM;
	
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,GetMaxX-78,GetMaxY-20-frm->y,0,frm->FRM);
		}
	}

	if (pointD) {
		if (pointD->count(Selected) == 0) {
			if (pointD->lower_bound(Selected) != pointD->end()) {
                Selected = pointD->lower_bound(Selected)->first;
			}
		}

 		iterD = pointD->lower_bound(Selected);
		for (ix=0; ix<15; ix++) if (iterD != pointD->begin()) iterD--;

		ix = 0;

		while ((iterD != pointD->end()) && (ix<(GetMaxY-35)/12))
		{
			iy = iterD->first;
			textfont::DisplayNum(GetMaxX-80,15+ix*12,Selected == iy ? 0 : 1,iy,8);
			iterD++;ix++;
		}
	 	if (pointD->count(Selected) != 0) {
			PFRM frm;
			frm = (*pointD)[Selected]->FRMA->GetCurFrame();
	
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,GetMaxX-78,GetMaxY-20-frm->y,0,frm->FRM);
		}
	}

	ref_to = NULL;
	if (putmode==9) {
		char buf[50];
		textfont::IanOutTextR(GetMaxX,30,1,"Boundaries");
		textfont::IanOutTextR(GetMaxX,45,1,"----------");
		bool InSide = false;

		if (mouse::MouseIn(GetMaxX-80,60,GetMaxX,89)) { ref_to = &BoundRect.left; InSide = true; } else InSide=false;
		textfont::IanOutText(GetMaxX-80,60,InSide ? 0 : 1,"X:");
		sprintf(buf,"%i",BoundRect.left);textfont::IanOutTextR(GetMaxX,75,InSide ? 0 : 1,buf);
		if (mouse::MouseIn(GetMaxX-80,90,GetMaxX,119)) { ref_to = &BoundRect.top; InSide = true; } else InSide=false;
		textfont::IanOutText(GetMaxX-80,90,InSide ? 0 : 1,"Y:");
		sprintf(buf,"%i",BoundRect.top); textfont::IanOutTextR(GetMaxX,105,InSide ? 0 : 1,buf);
		if (mouse::MouseIn(GetMaxX-80,120,GetMaxX,149)) { ref_to = &BoundRect.right; InSide = true; } else InSide=false;
		textfont::IanOutText(GetMaxX-80,120,InSide ? 0 : 1,"Width:");
		sprintf(buf,"%i",BoundRect.right);textfont::IanOutTextR(GetMaxX,135,InSide ? 0 : 1,buf);
		if (mouse::MouseIn(GetMaxX-80,150,GetMaxX,179)) { ref_to = &BoundRect.bottom; InSide = true; } else InSide=false;
		textfont::IanOutText(GetMaxX-80,150,InSide ? 0 : 1,"Height:");
		sprintf(buf,"%i",BoundRect.bottom);textfont::IanOutTextR(GetMaxX,165,InSide ? 0 : 1,buf);

		if (mouse::MouseIn(GetMaxX-80,200,GetMaxX,214)) InSide = true; else InSide=false;
		if (InSide && mouse::Pressed(0)) BoundType = !BoundType;
		if  (!BoundType) {
			textfont::IanOutText(GetMaxX-80,200,InSide ? 0 : 1,"Rectangular");
		} else {
			textfont::IanOutText(GetMaxX-80,200,InSide ? 0 : 1,"Diagonal");
		}

		if (mouse::MouseIn(GetMaxX-80,215,GetMaxX,229)) InSide = true; else InSide=false;
		if (InSide && mouse::Pressed(0)) Bound_plane = !Bound_plane;
		textfont::IanOutText(GetMaxX-80,215,Bound_plane ? 0 : 1,"Show");
		
		bool InPres = false;
		bool IsSet = false;
		if (mouse::MouseIn(GetMaxX-80,230,GetMaxX,244)) InSide = true; else InSide=false;
		if (InSide && mouse::Pressed(0)) { InPres = true; IsSet = true; };
		textfont::IanOutText(GetMaxX-80,230,InSide ? 0 : 1,"Set");

		if (mouse::MouseIn(GetMaxX-80,245,GetMaxX,260)) InSide = true; else InSide=false;
		if (InSide && mouse::Pressed(0)) { InPres = true; IsSet = false; };
		textfont::IanOutText(GetMaxX-80,245,InSide ? 0 : 1,"Clear");

		if (InPres) {
		int iy,ix,ix2,iy2;
		for (iy=BoundRect.top; iy<=BoundRect.top+BoundRect.bottom; iy++) {
			ix = BoundRect.left;
			iy2 = BoundType ? iy : iy+ix/4;
			ix2 = BoundType ? ix : ix-iy-iy/3;
			if ((ix2>=0) && (ix2<512) && (iy2>=0) && (iy2<512)) 
				BlockDat[ix2][iy2] = IsSet;
			
			ix = BoundRect.left+BoundRect.right;
			iy2 = BoundType ? iy : iy+ix/4;
			ix2 = BoundType ? ix : ix-iy-iy/3;
			if ((ix2>=0) && (ix2<512) && (iy2>=0) && (iy2<512)) 
				BlockDat[ix2][iy2] = IsSet;
		}
		for (ix=BoundRect.left; ix<=BoundRect.left+BoundRect.right; ix++) {
			iy = BoundRect.top;
			iy2 = BoundType ? iy : iy+ix/4;
			ix2 = BoundType ? ix : ix-iy-iy/3;
			if ((ix2>=0) && (ix2<512) && (iy2>=0) && (iy2<512)) 
				BlockDat[ix2][iy2] = IsSet;
			
            iy = BoundRect.top+BoundRect.bottom;
			iy2 = BoundType ? iy : iy+ix/4;
			ix2 = BoundType ? ix : ix-iy-iy/3;
			if ((ix2>=0) && (ix2<512) && (iy2>=0) && (iy2<512)) 
				BlockDat[ix2][iy2] = IsSet;
		}
		}

		if (mouse::MouseIn(GetMaxX-80,260,GetMaxX,274)) InSide = true; else InSide=false;
		if (InSide && mouse::Pressed(0)) BoundTest = !BoundTest;
		textfont::IanOutText(GetMaxX-80,260,BoundTest ? 0 : 1,"Test");
	}
	textfont::IanOutTextR(640,GetMaxY-20,dot_plane ? 0 : 1,"DOT");
	textfont::IanOutTextR(610,GetMaxY-20,wall_plane ? 0 : 1,"WALL");
	textfont::IanOutTextR(570,GetMaxY-20,item_plane ? 0 : 1,"ITEM");
	textfont::IanOutTextR(540,GetMaxY-20,block_plane ? 0 : 1,"BLCK");
	textfont::IanOutTextR(370,GetMaxY-20,auto_block ? 0 : 1,"AUTOBLCK");

	textfont::IanOutText(380,GetMaxY-20,mouse::MouseIn(380,GetMaxY-20,390,GetMaxY) ? 0 : 1,"+");
	textfont::IanOutText(450,GetMaxY-20,mouse::MouseIn(450,GetMaxY-20,460,GetMaxY) ? 0 : 1,"-");
	textfont::DisplayNum(390,GetMaxY-20,1,Selected,5);

	if (putmode!=0) {
		textfont::DisplayNum(690,0,0,(((x/4)<128)&&((y/2)<256)&&((x/4)>=0)&&((y/2)>=0)) ? MapInf->Map[x/4][y/2-1] : -99999,5);
		textfont::IanOutText(640,0,show_roof ? 0 : 1,"TILE");
	} else {
		textfont::DisplayNum(690,0,0,(((x/4)<128)&&((y/2)<256)&&((x/4)>=0)&&((y/2)>=0)) ? MapInf->Map2[x/4][y/2-1] : -99999,5);
		textfont::IanOutText(640,0,show_roof ? 0 : 1,"ROOF");
	}

//	textfont::DisplayNum(590,0,show_wall ? 0 : 1,((x<251)&&(y<201)&&(x>=0)&&(y>=0)) ? WallInf->Map[x][y] : -99999,5);
	textfont::IanOutText(550,0,show_wall ? 0 : 1,"WALL");
	
//	textfont::DisplayNum(490,0,show_item ? 0 : 1,((x<251)&&(y<201)&&(x>=0)&&(y>=0)) ? ItemInf->Map[x][y] : -99999,5);
	textfont::IanOutText(450,0,show_item ? 0 : 1,"ITEM");

	textfont::IanOutTextC(320,0,mouse::MouseIn(250,0,390,15) ? 0 : 1,"MAIN MENU");

	if (mouse::MInPr(250,0,390,12,0)) {
		GamePos = 0;
	}

	
	if (mouse::MInPr(550,0,640,15,0)) show_wall=!show_wall;
	if (mouse::MInPr(450,0,549,15,0)) show_item=!show_item;
	if (mouse::MInPr(641,0,800,15,0)) show_roof=!show_roof;
	

	if (mouse::MInPr(380,GetMaxY-20,390,GetMaxY,0)) Selected++;
	if (mouse::MInPr(450,GetMaxY-20,GetMaxY-20,GetMaxY,0)) Selected--;

	if (mouse::MInPr(0,GetMaxY-20,40,GetMaxY,0))
	{
		(putmode<9) ? putmode++ : putmode=0;
	}

	if (mouse::MInPr(611,GetMaxY-20,640,GetMaxY,0)) dot_plane=!dot_plane;
	if (mouse::MInPr(571,GetMaxY-20,610,GetMaxY,0)) wall_plane=!wall_plane;
	if (mouse::MInPr(541,GetMaxY-20,570,GetMaxY,0)) item_plane=!item_plane;
	if (mouse::MInPr(500,GetMaxY-20,540,GetMaxY,0)) block_plane=!block_plane;
	if (mouse::MInPr(280,GetMaxY-20,370,GetMaxY,0)) auto_block=!auto_block;
	
	if (mouse::MouseIn(0,15,GetMaxX-80,GetMaxY-20))
	{
		if ((putmode!=4) && (putmode!=0)) {
			if (mousetyp==0) BlitTo(g_pDDSBack,0,0,32,16,TerX+LocConvertX(x,y)*16,TerY+LocConvertY(x,y)*12,0,Select2->FRM->FRM);
		} else {
			if (mousetyp==0) BlitTo(g_pDDSBack,0,0,32,16,TerX+LocConvertX(x/4*4,y/2*2)*16,TerY+LocConvertY(x/4*4,y/2*2)*12,0,Select2->FRM->FRM);
		}
	} else
	{
	  if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	}

	  if (mouse::MouseIn(0,16,GetMaxX-80,GetMaxY-20))
	  {
		  if ((x<512)&&(y<512)&&(x>=0)&&(y>=0)) {
		  if (mouse::Pressed(6))  if (putmode==3) BlockDat[x][y] = 1;
		  if (mouse::Pressed(8))  if (putmode==3) BlockDat[x][y] = 2;
		  if (mouse::Pressed(0))
		  {
			  char buf[150],buf3[150],buf4[150];
			  if (putmode==1)
			  if (StaticInf->TilesW.count(Selected) != 0)
			     { 
					 PLocationDat Loc = new TLocationDat();
					 Loc->num=Selected;
					 Loc->type=0;
					 Loc->loc=CompLoc(x,y);
					 Loc->state = 0;
					 Loc->FRM = StaticInf->TilesW[Selected];
					 Loc->FRMA = NULL;
					 StaticInf->Map.insert( Location_Pair (Loc->loc,Loc) );
					 if (auto_block) BlockDat[x][y]=1;
				 }
			  if (putmode==2)
			  if (StaticInf->TilesS.count(Selected) != 0)
			  { 
					 PLocationDat Loc = new TLocationDat();
					 Loc->num=Selected;
					 Loc->type=1;
					 Loc->loc=CompLoc(x,y);
					 Loc->state = 0;
					 Loc->FRM = StaticInf->TilesS[Selected];
					 Loc->FRMA = NULL;
					 StaticInf->Map.insert( Location_Pair (Loc->loc,Loc) );
					 if (auto_block) BlockDat[x][y]=1;
				 }
			  if (putmode==4) if (MapInf->Tiles.count(Selected) != 0)
			  {
			    for (int zzz=0; zzz<BrushSize;zzz++)
				for (int zzz2=0; zzz2<BrushSize;zzz2++) {
					int xx2 = x/4-BrushSize/2+zzz;
					int yy2 = y/2-BrushSize/2+zzz2-1;
					if ((xx2>=0) && (yy2>=0) && (xx2<128) && (yy2<256))
						MapInf->Map[xx2][yy2] = Selected;
				}
			  }

			  if (putmode==0) if (MapInf->Tiles.count(Selected) != 0)
			  {
			    for (int zzz=0; zzz<BrushSize;zzz++)
				for (int zzz2=0; zzz2<BrushSize;zzz2++) {
					int xx2 = x/4-BrushSize/2+zzz;
					int yy2 = y/2-BrushSize/2+zzz2-1;
					if ((xx2>=0) && (yy2>=0) && (xx2<128) && (yy2<256))
						MapInf->Map2[xx2][yy2] = Selected;
				}
			  }

			  if (putmode==5)
			  if (StaticInf->TilesI.count(Selected) != 0)
			  { 
					 PLocationDat Loc = new TLocationDat();
					 Loc->num=Selected;
					 Loc->type=2;
					 Loc->loc=CompLoc(x,y);
					 Loc->state = 0;
					 Loc->FRM = StaticInf->TilesI[Selected]->FRM;
					 Loc->FRMA = new TFRMAnimCommunicator(StaticInf->TilesI[Selected]->FRMA);
					 StaticInf->Map.insert( Location_Pair (2000000000,Loc) );
					 //if (auto_block) BlockDat[x][y]=1;
			  }

			  if (putmode==6)
			  if (StaticInf->TilesO.count(Selected) != 0)
			  { 
					 PLocationDat Loc = new TLocationDat();
					 Loc->num=Selected;
					 Loc->type=3;
					 Loc->loc=CompLoc(x,y);
					 Loc->state = 0;
					 Loc->FRM = NULL;
					 Loc->FRMA = new TFRMAnimCommunicator(StaticInf->TilesO[Selected]->FRMA);

					 
					sprintf(buf,"OBJECTS_%i",Selected);
					sprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
					GetPrivateProfileString("OBJECTS",buf,"",buf4,150,buf3);
					sprintf(buf,"\\data\\objects\\%s",buf4);

					Loc->ItemDesc = new TItemDesc();
					Loc->ItemDesc->Inven = new TInventory();
					Loc->ItemDesc->ability = textutil::GetFromXML(GetFile(buf),"/desc.ability");
					Loc->ItemDesc->desclocation = GetFile(buf);
					Loc->ItemDesc->Count = 0;
					Loc->ItemDesc->var1 = 0;
					Loc->ItemDesc->var2 = 0;
					Loc->ItemDesc->direction = false;
					Loc->ItemDesc->change = false;

					 StaticInf->Map.insert( Location_Pair (Loc->loc,Loc) );
					 if (auto_block) BlockDat[x][y]=1;
			  }

			  if (putmode==7)
			  if (Selected>=0)
			  { 
					 PLocationDat Loc = new TLocationDat();
					 Loc->num=Selected%12;
					 Loc->type=4;
					 Loc->loc=CompLoc(x,y);
					 Loc->state = 0;
					  Loc->FRM = ExitGrid[Selected%12];
					 Loc->FRMA = NULL;

					Loc->ItemDesc = new TItemDesc();
					Loc->ItemDesc->Inven = NULL;
					Loc->ItemDesc->ability = "0000000000";
					Loc->ItemDesc->desclocation = "";
					Loc->ItemDesc->Count = 0;
					if ((Loc->num>=4) && (Loc->num<8)) Loc->ItemDesc->Count=1;
					Loc->ItemDesc->var1 = 0;
					Loc->ItemDesc->var2 = 0;
					Loc->ItemDesc->direction = false;
					Loc->ItemDesc->change = false;

					 StaticInf->Map.insert( Location_Pair (2000000000,Loc) );
			  }


		  }
		  
          if (mouse::Pressed(2))
		  {
			  if (putmode==1) 
			  {
				  FRMLocationMap::iterator iter;
				  if (!StaticInf->Map.empty()) {
					  iter = StaticInf->Map.begin();
					  while (iter != StaticInf->Map.end())
					  {
						  StaticInf->Map.begin();
						  if ((iter->second->type==0) && (y==DeCompLocY(iter->first)) && (x==DeCompLocX(iter->first & 1023)))
						  {
							  Selected = iter->second->num;
						  }
						  iter++;
					  }
				  }
				  
			  }
			  if (putmode==2)
			  {
				  FRMLocationMap::iterator iter;
				  if (!StaticInf->Map.empty()) {
					  iter = StaticInf->Map.begin();
					  while (iter != StaticInf->Map.end())
					  {
						  StaticInf->Map.begin();
						  if ((iter->second->type==1) && (y==DeCompLocY(iter->first)) && (x==DeCompLocX(iter->first)))
						  {
							  Selected = iter->second->num;
						  }
						  iter++;
					  }
				  }
			  }
			  if (putmode==4) Selected = MapInf->Map[x/4][y/2-1];
			  if (putmode==0) Selected = MapInf->Map2[x/4][y/2-1];
		  }
		  
		  if ((dims.buttons[1]))
		  {
			  if ((putmode==1) || (putmode==2) || (putmode>4)) 
			  {
				  FRMLocationMap::iterator iterMap;
				  iterMap = StaticInf->Map.begin();
				  while (iterMap != StaticInf->Map.end())
				  {
					  FRMLocationMap::iterator iterMapSave = StaticInf->Map.end();
					  if (iterMap->second->loc == CompLoc(x,y)) {
						int whichmode;
						switch (putmode) {
							case 1: whichmode=0;break;
							case 2: whichmode=1;break;
							case 5: whichmode=2;break;
							case 6: whichmode=3;break;
							case 7: whichmode=4;break;
							case 8: whichmode=5;break;
						}
						if (iterMap->second->type == whichmode) {
							delete iterMap->second;
							iterMapSave = iterMap;
						}
					  }
					  iterMap++;
					  if (iterMapSave != StaticInf->Map.end()) StaticInf->Map.erase(iterMapSave);
				  }
			  }
			  if ((putmode==3) || (auto_block)) BlockDat[x][y] = 0;
			  if (putmode==4) MapInf->Map[x/4][y/2-1] = 0;
			  if (putmode==0) MapInf->Map2[x/4][y/2-1] = 0;
			  
		  }
	  }
	  }

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
