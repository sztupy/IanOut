#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "../commonutils/mouse.h"

extern PFRMSingle ExitGrid[12];

void terep::DrawTerep(int mousetyp,int x,int y)
{
		int iy,ix;//,ix2,iy2;
	DDBLTFX                     ddbltfx;
	
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(0,0,0);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	int bx,by;

	/*by = (-TerY)/12;
	bx = (-TerX)/16-by;

	if ((bx)/4-15<0) bx=60;
	if ((by)/2-5<0) by=10;
	if ((bx)/4+20>128) bx=432;
	if ((by)/2+35>256) by=442;

	for (ix=(bx)/4-15;ix<((bx)/4+20); ix++) 
	for (iy=(by)/2-5;iy<((by)/2+35); iy++)
	{
		int i = MapInf->Map[ix][iy];
		if ((i!=0) && (MapInf->Tiles.count(i) != 0))
		BlitTo(g_pDDSBack,0,0,MapInf->Tiles[i]->FRM->x,MapInf->Tiles[i]->FRM->y,TerX+(2*iy+4*ix-ix%2)*16+7+MapInf->Tiles[i]->FRM->bx,TerY+(2*iy-ix%2)*12+2+MapInf->Tiles[i]->FRM->by,DDBLTFAST_SRCCOLORKEY,MapInf->Tiles[i]->FRM->FRM);
	}*/


	for (ix=0;ix<128;ix++)
	for (iy=1;iy<129;iy++)
	{
		int i = MapInf->Map[ix][iy-1];
		if ((i!=0) && (MapInf->Tiles.count(i) !=0)) {
			PFRM frm;
			frm = MapInf->Tiles[i]->FRM;
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,frm->bx+TerX+ix*48+iy*32+7,frm->by+TerY-ix*12+iy*24+3,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		}
	}

	int VisXB = -1;//(-TerX)/16+TerY/12-50; 
	int VisXE = 1024;//(-TerX)/16+TerY/12+50;
	int VisYB = -1;//(-TerY)/12;
	int VisYE = 1024;//(-TerY)/12+70;


	if (VisXB<0) VisXB=0;
	if (VisXE<0) VisXE=0;
	if (VisYB<0) VisYB=0;
	if (VisYE<0) VisYE=0;

	if (VisXB>=512) VisXB=512;
	if (VisXE>=512) VisXE=512;
	if (VisYB>=512) VisYB=512;
	if (VisYE>=512) VisYE=512;

	FRMLocationMap::reverse_iterator iter;
	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2!=CritterInf->Critters.end())
	{
		iter2->second->AlreadyDrawn = false;
		iter2->second->conv = CompLoc(iter2->second->x,iter2->second->y);
		iter2++;
	}

	if (!StaticInf->Map.empty()) 
	{
	
	iter = StaticInf->Map.rbegin();
	while (iter != StaticInf->Map.rend())
	{
		 iy = DeCompLocY(iter->second->loc);
		 ix = DeCompLocX(iter->second->loc);

		 if ((iy>=VisYB) && (iy<=VisYE) && (ix>=VisXB) && (ix<=VisXE)) {

		 PFRM frm;
		 if (iter->second->FRM)
			 frm = iter->second->FRM->FRM;
		 else
			 frm = iter->second->FRMA->GetCurFrame();


		 if (iter->first != 2000000000) {
			iter2 = CritterInf->Critters.begin();
			while (iter2 !=CritterInf->Critters.end()) {
				if ((!iter2->second->AlreadyDrawn) && (iter2->second->conv >= iter->first)) {
					iter2->second->AlreadyDrawn = true;
					iter2->second->Draw(g_pDDSBack,TerX,TerY,9);
				}
				iter2++;
			}
		 }

		 if ((iter->second->type<2) && (show_wall))
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,TerX+LocConvertX(ix,iy)*16+frm->bx,TerY+LocConvertY(ix,iy)*12+frm->by,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		 if ((iter->second->type>=2) && (show_item))
			BlitTo(g_pDDSBack,0,0,frm->x,frm->y,TerX+LocConvertX(ix,iy)*16+frm->bx,TerY+LocConvertY(ix,iy)*12+frm->by,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		 }
		iter++;
	}
	}

	iter2 = CritterInf->Critters.begin();
	while (iter2 !=CritterInf->Critters.end()) {
		if ((!iter2->second->AlreadyDrawn)) {
			iter2->second->AlreadyDrawn = true;
			iter2->second->Draw(g_pDDSBack,TerX,TerY,9);
		}
		iter2++;
	}

	if (GamePos==1)
	if ((Selected>=0) && (putmode==7))
	{
	if ((MousY<GetMaxY-20) && (MousY>15))
		BlitTo(g_pDDSBack,0,0,ExitGrid[Selected%12]->FRM->x,ExitGrid[Selected%12]->FRM->y,TerX+LocConvertX(x,y)*16+ExitGrid[Selected%12]->FRM->bx,TerY+LocConvertY(x,y)*12+ExitGrid[Selected%12]->FRM->by,DDBLTFAST_SRCCOLORKEY,ExitGrid[Selected%12]->FRM->FRM);
	}

	if (GamePos==1)
	if ((Selected!=0) || (putmode>4)) {
	if (putmode==1)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (StaticInf->TilesW.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,StaticInf->TilesW[Selected]->FRM->x,StaticInf->TilesW[Selected]->FRM->y,TerX+LocConvertX(x,y)*16+StaticInf->TilesW[Selected]->FRM->bx,TerY+LocConvertY(x,y)*12+StaticInf->TilesW[Selected]->FRM->by,DDBLTFAST_SRCCOLORKEY,StaticInf->TilesW[Selected]->FRM->FRM);
	}

	if (putmode==2)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (StaticInf->TilesS.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,StaticInf->TilesS[Selected]->FRM->x,StaticInf->TilesS[Selected]->FRM->y,TerX+LocConvertX(x,y)*16+StaticInf->TilesS[Selected]->FRM->bx,TerY+LocConvertY(x,y)*12+StaticInf->TilesS[Selected]->FRM->by,DDBLTFAST_SRCCOLORKEY,StaticInf->TilesS[Selected]->FRM->FRM);
	}

	if (putmode==4)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (MapInf->Tiles.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,MapInf->Tiles[Selected]->FRM->x,MapInf->Tiles[Selected]->FRM->y,TerX+LocConvertX(x,y)*16+MapInf->Tiles[Selected]->FRM->bx,TerY+LocConvertY(x,y)*12+MapInf->Tiles[Selected]->FRM->by,DDBLTFAST_SRCCOLORKEY,MapInf->Tiles[Selected]->FRM->FRM);
	}
	
	if (putmode==5)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (StaticInf->TilesI.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,StaticInf->TilesI[Selected]->FRM->FRM->x,StaticInf->TilesI[Selected]->FRM->FRM->y,TerX+LocConvertX(x,y)*16+StaticInf->TilesI[Selected]->FRM->FRM->bx,TerY+LocConvertY(x,y)*12+StaticInf->TilesI[Selected]->FRM->FRM->by,DDBLTFAST_SRCCOLORKEY,StaticInf->TilesI[Selected]->FRM->FRM->FRM);
	}

	if (putmode==6)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (StaticInf->TilesO.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,StaticInf->TilesO[Selected]->FRMA->GetCurFrame()->x,StaticInf->TilesO[Selected]->FRMA->GetCurFrame()->y,TerX+LocConvertX(x,y)*16+StaticInf->TilesO[Selected]->FRMA->GetCurFrame()->bx,TerY+LocConvertY(x,y)*12+StaticInf->TilesO[Selected]->FRMA->GetCurFrame()->by,DDBLTFAST_SRCCOLORKEY,StaticInf->TilesO[Selected]->FRMA->GetCurFrame()->FRM);
	}

	if (putmode==8)
	{
	if ((MousY<GetMaxY-20) && (MousY>15) && (StaticInf->TilesD.count(Selected) !=0))
	  BlitTo(g_pDDSBack,0,0,StaticInf->TilesD[Selected]->FRMA->GetCurFrame()->x,StaticInf->TilesD[Selected]->FRMA->GetCurFrame()->y,TerX+LocConvertX(x,y)*16+StaticInf->TilesD[Selected]->FRMA->GetCurFrame()->bx,TerY+LocConvertY(x,y)*12+StaticInf->TilesD[Selected]->FRMA->GetCurFrame()->by,DDBLTFAST_SRCCOLORKEY,StaticInf->TilesD[Selected]->FRMA->GetCurFrame()->FRM);
	}

	}

	if (block_plane)
	for (iy=VisYB; iy<VisYE; iy++)
	for (ix=VisXB; ix<VisXE; ix++) 
		if (BlockDat[ix][iy] != 0) textfont::DrawChar(TerX+LocConvertX(ix,iy)*16+11,TerY+LocConvertY(ix,iy)*12+2,(BlockDat[ix][iy] == 1) ? 0 : 1,'X');

	if (Bound_plane) {
		if (BoundType) {
			for (iy=BoundRect.top; iy<=BoundRect.top+BoundRect.bottom; iy++) {
				textfont::DrawChar(TerX+LocConvertX(BoundRect.left,iy)*16+12,
								   TerY+LocConvertY(BoundRect.left,iy)*12+2,1,'\\');
				textfont::DrawChar(TerX+LocConvertX(BoundRect.left+BoundRect.right,iy)*16+12,
								   TerY+LocConvertY(BoundRect.left+BoundRect.right,iy)*12+2,1,'\\');
			}
			for (ix=BoundRect.left; ix<=BoundRect.left+BoundRect.right; ix++) {
				textfont::DrawChar(TerX+LocConvertX(ix,BoundRect.top)*16+12,
					TerY+LocConvertY(ix,BoundRect.top)*12+2,1,((ix-1)%4<=1 ? '-' : '/'));
				textfont::DrawChar(TerX+LocConvertX(ix,BoundRect.top+BoundRect.bottom)*16+12,
								   TerY+LocConvertY(ix,BoundRect.top+BoundRect.bottom)*12+2,1,((ix-1)%4<=1 ? '-' : '/'));
			}
		} else {
			for (iy=BoundRect.top; iy<=BoundRect.top+BoundRect.bottom; iy++) {
				textfont::DrawChar(TerX+LocConvertX(BoundRect.left-iy-iy/3,iy+BoundRect.left/4)*16+12,
								   TerY+LocConvertY(BoundRect.left-iy-iy/3,iy+BoundRect.left/4)*12+2,1,'|');
				textfont::DrawChar(TerX+LocConvertX(BoundRect.left+BoundRect.right-iy-iy/3,iy+(BoundRect.left+BoundRect.right)/4)*16+12,
								   TerY+LocConvertY(BoundRect.left+BoundRect.right-iy-iy/3,iy+(BoundRect.left+BoundRect.right)/4)*12+2,1,'|');
			}
			for (ix=BoundRect.left; ix<=BoundRect.left+BoundRect.right; ix++) {
				textfont::DrawChar(TerX+LocConvertX(ix-BoundRect.top-BoundRect.top/3,BoundRect.top+ix/4)*16+12,
								   TerY+LocConvertY(ix-BoundRect.top-BoundRect.top/3,BoundRect.top+ix/4)*12+2,1,'-');
				textfont::DrawChar(TerX+LocConvertX(ix-(BoundRect.top+BoundRect.bottom)-(BoundRect.top+BoundRect.bottom)/3,BoundRect.top+BoundRect.bottom+ix/4)*16+12,
								   TerY+LocConvertY(ix-(BoundRect.top+BoundRect.bottom)-(BoundRect.top+BoundRect.bottom)/3,BoundRect.top+BoundRect.bottom+ix/4)*12+2,1,'-');
			
			}
		}
	}

	if (dot_plane) {
	if (putmode==4) {
	for (iy=1; iy<129; iy++)
	for (ix=0; ix<128; ix++)
	{
		textfont::DrawChar(TerX+48*ix+32*iy+14,TerY+24*iy-12*ix,4,'.');
	} } else
	for (iy=VisYB; iy<VisYE; iy++)
	for (ix=VisXB; ix<VisXE; ix++) 
        if (ix%2==0) textfont::DrawChar(TerX+LocConvertX(ix,iy)*16+14,TerY+LocConvertY(ix,iy)*12,4,'.');
		
	}
	
    if (!StaticInf->Map.empty()) 
	{
     iter = StaticInf->Map.rbegin();
	 while (iter != StaticInf->Map.rend())
	 {
		 iy = DeCompLocY(iter->second->loc);
		 ix = DeCompLocX(iter->second->loc);
		 if ((wall_plane) && (iter->second->type<2)) {
			 textfont::DrawChar(TerX+LocConvertX(ix,iy)*16+14,TerY+LocConvertY(ix,iy)*12,3,'.');
		 }
		 if ((item_plane) && (iter->second->type>=2)) {
			 textfont::DrawChar(TerX+LocConvertX(ix,iy)*16+14,TerY+LocConvertY(ix,iy)*12,1,'.');
		 }
		 iter++;
	 }
	}
	
}