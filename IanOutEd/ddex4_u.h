#include "ddex4_u3.h"

//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
//-----------------------------------------------------------------------------

void 
UpdateFrame(void)
{
    int                         x,y;
	HRESULT                     hRet;
	static int							Frame = 0;
	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	static int							mousetyp = 0;
	int									i,ix,iy;
	BOOL								kintvan;
	BOOL								oldal[4];
	BlockType							BlDat;
	DDBLTFX								ddbltfx;
	RECT								rcRect;

	for (x=0; x<170; x++)
		for (y=0; y<150; y++)
		{
		  BlDat[x][y] = BlockDat[x][y];
		}

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > (DWORD)gameSpeed)
        {
            LastTick = ThisTick;
        }

// -------------------------------------------------------------------
	olddims=dims;													//	
	UpdateInputState();												//
	if ((dims.lX == olddims.lX) && (dims.lY == olddims.lY)) Frame++; else Frame=0;
	//if ((dims.rgbButtons[1] & 0x80) && !(olddims.rgbButtons[1] & 0x80)) 
	//	 if (mousetyp==0) { mousetyp=9; } else { mousetyp=0; }
																	//	
	MousX += dims.lX;												//	
	MousY += dims.lY;												//	
																	//	
	if ((MousX>=640) && (MousY<=0)) {mousetyp = 2;} else    		//
	if ((MousX>=640) && (MousY>=480)) {mousetyp = 4;} else  		//
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else    		//
	if ((MousX<=0)   && (MousY>=480)) {mousetyp = 6;} else  		//
	if (MousX>=640) {mousetyp = 3;} else							//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=480) {mousetyp = 5;} else							//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=FALSE;													//
	oldal[0]=FALSE;oldal[2]=FALSE;									//
	oldal[1]=FALSE;oldal[3]=FALSE;									//
																	//
	if (MousX>=640) { MousX=640; TerX-=15; }						//	
	if (MousX<=0)   { MousX=0;   TerX+=15; }						//	
	if (MousY>=480) { MousY=480; TerY-=15; }						//	
	if (MousY<=0)   { MousY=0;   TerY+=15; }						//	
																	//
	if (TerY>0) {TerY=0;oldal[0]=TRUE;kintvan=TRUE;}				//
	if (TerY<-1210) {TerY=-1210;oldal[1]=TRUE;kintvan=TRUE;}		//
																	//
	if (TerX>-3640) {TerX=-3640;oldal[2]=TRUE;kintvan=TRUE;}		//
	if (TerX<-5240) {TerX=-5240;oldal[3]=TRUE;kintvan=TRUE;}		//
	if ((mousetyp == 2) && (oldal[0] != oldal[3])) kintvan=FALSE;	//
	if ((mousetyp == 4) && (oldal[1] != oldal[3])) kintvan=FALSE;	//
	if ((mousetyp == 6) && (oldal[2] != oldal[1])) kintvan=FALSE;	//
	if ((mousetyp == 8) && (oldal[2] != oldal[0])) kintvan=FALSE;	//
																	//
// -------------------------------------------------------------------

	//if (((-TerY+MousY)%24)<12)
	{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-226;
		//if (x%2==1) x--;
	} //else
	/*{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-1-226;
		if (x%2==0) x++;
	}*/

	DrawTerep(mousetyp,x,y);

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
	
	rcRect.top=460;
	rcRect.left=0;
	rcRect.bottom=480;
	rcRect.right=640;
	g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	rcRect.top=0;
	rcRect.left=0;
	rcRect.bottom=15;
	rcRect.right=640;
	g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	switch (putmode)
	{
	case 1:IanOutText(0,460,1,"WALL");break;
	case 2:IanOutText(0,460,1,"ITEM");break;
	case 3:IanOutText(0,460,1,"BLCK");break;
	case 4:IanOutText(0,460,1,"TILE");break;
	}

	IanOutTextR(640,460,dot_plane ? 0 : 1,"DOT");
	IanOutTextR(610,460,wall_plane ? 0 : 1,"WALL");
	IanOutTextR(570,460,item_plane ? 0 : 1,"ITEM");
	IanOutTextR(540,460,block_plane ? 0 : 1,"BLCK");

	IanOutText(490,460,1,"+");
	IanOutText(450,460,1,"-");
	DisplayNum(460,460,1,Selected,2);

	DisplayNum(610,0,show_wall ? 0 : 1,WallInf->Map[x][y]-1,2);
	IanOutText(570,0,show_wall ? 0 : 1,"WALL");
	
	DisplayNum(540,0,show_item ? 0 : 1,ItemInf->Map[x][y]-1,2);
	IanOutText(510,0,show_item ? 0 : 1,"ITEM");

	if (MInPr(570,0,640,15,0)) show_wall=!show_wall;
	if (MInPr(510,0,569,15,0)) show_item=!show_item;
	

	if (MInPr(490,460,499,480,0)) Selected++;
	if (MInPr(450,460,460,480,0)) Selected--;

	if (MInPr(0,460,40,480,0))
	{
		(putmode<4) ? putmode++ : putmode=1;
	}


	if (MInPr(611,460,640,480,0)) dot_plane=!dot_plane;
	if (MInPr(571,460,610,480,0)) wall_plane=!wall_plane;
	if (MInPr(541,460,570,480,0)) item_plane=!item_plane;
	if (MInPr(500,460,540,480,0)) block_plane=!block_plane;
	
	if (MouseIn(0,15,640,460))
	{
		if (mousetyp==0) BlitTo(g_pDDSBack,0,0,32,16,TerX+(x+226)*16,TerY+(y)*12,DDBLTFAST_SRCCOLORKEY,Select2->FRM->FRM);
		//if (mousetyp==9) BlitTo(g_pDDSBack,0,0,Mouse2->FRM->x,Mouse2->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse2->FRM->FRM);
	} else
	{

	  if ((putmode==1) && (Selected>=0) && (Selected<50))
	    BlitTo(g_pDDSBack,0,0,WallInf->Tiles[Selected]->FRM->x,WallInf->Tiles[Selected]->FRM->y,320-WallInf->Tiles[Selected]->FRM->x/2,240-WallInf->Tiles[Selected]->FRM->y/2,0,WallInf->Tiles[Selected]->FRM->FRM);
	

	  if ((putmode==2) && (Selected>=0) && (Selected<14))
	    BlitTo(g_pDDSBack,0,0,ItemInf->Tiles[Selected]->FRM->x,ItemInf->Tiles[Selected]->FRM->y,320-ItemInf->Tiles[Selected]->FRM->x/2,240-ItemInf->Tiles[Selected]->FRM->y/2,0,ItemInf->Tiles[Selected]->FRM->FRM);


	  if ((putmode==4) && (Selected>=0) && (Selected<6))
	    BlitTo(g_pDDSBack,0,0,80,35,280,223,0,MapInf->Tiles[Selected]->FRM->FRM);
		
	  if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
	}

	  if (MouseIn(0,0,640,460))
	  {
		  if (Pressed(0))
		  {
			  if (putmode==1) WallInf->Map[x][y] = Selected+1;
			  if (putmode==2) ItemInf->Map[x][y] = Selected+1;
			  if (putmode==3) BlockDat[x][y] = 1;
			  if (putmode==4)
			  {
					if ((Selected>=0) && (Selected<6))
				  for (ix=0; ix<100; ix++) {
					  for (iy=0; iy<100; iy++) { 
						  if (((ix*48+iy*32+TerX) == (TerX+(x+226)*16)) &&
							  ((iy*24-ix*12+TerY)	== (TerY+y*12)))
						  {
							  MapInf->Map[iy][ix] = (unsigned char)Selected; 
						  }
					  }
				  }
			  }
		  }
		  
		  if (Pressed(2))
		  {
			  if (putmode==1) Selected = WallInf->Map[x][y] - 1;
			  if (putmode==2) Selected = ItemInf->Map[x][y] - 1;
		  }
		  
		  if ((dims.rgbButtons[1] & 0x80))
		  {
			  if (putmode==1) WallInf->Map[x][y] = 0;
			  if (putmode==2) ItemInf->Map[x][y] = 0;
			  if (putmode==3) BlockDat[x][y] = 0;
		  }
	  }

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

	DisplayNum(70,0,1,x,3);
	DisplayNum(100,0,1,y,3);
	DisplayNum(130,0,1,gameSpeed,2);
	DisplayFrameRate();

	// Flip the surfaces
    while (TRUE)
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

#include "ddex4_u2.h"