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
		PFRM FrmCucc;	
	static int							Frame = 0;
	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	static int							mousetyp = 0;
	int									i;
	BOOL								kintvan;
	BOOL								oldal[4];
	BlockType							BlDat;

	int i2,ix,iy;

	for (x=0; x<170; x++)
		for (y=0; y<150; y++)
		{
		  BlDat[x][y] = BlockDat[x][y];
		}

	BlDat[Ian->x][Ian->y] = 1;
	BlDat[Aradesh->x][Aradesh->y] = 1;
	for (i=0; i<10; i++) BlDat[Paraszt[i]->x][Paraszt[i]->y] = 1;

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > (DWORD)gameSpeed)
        {
            LastTick = ThisTick;
				
            Ian->Next(BlDat);
			Aradesh->Next(BlDat);
			
			for (i=0; i<10; i++) { Paraszt[i]->Next(BlDat);}
        }

// -------------------------------------------------------------------
	olddims=dims;													//	
	UpdateInputState();												//
	if ((dims.lX == olddims.lX) && (dims.lY == olddims.lY)) Frame++; else Frame=0;
	if ((dims.rgbButtons[1] & 0x80) && !(olddims.rgbButtons[1] & 0x80)) 
		 if (mousetyp==0) { mousetyp=9; } else { mousetyp=0; }
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


	if (((-TerY+MousY)%24)<12)
	{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-226;
		if (x%2==1) x--;
	} else
	{
		y = (-TerY+MousY)/12;
		x = (-TerX+MousX)/16-1-226;
		if (x%2==0) x++;
	}

	DrawTerep(mousetyp,x,y);

	if ((mousetyp==9) && (Pressed(3)))
	{
	for (i2=0; i2<145; i2++)
	for (ix=0; ix<165; ix++) 
	{
		  iy = i2;
	  		  if ((Aradesh->x == ix) && (Aradesh->y == iy))  {
			  FrmCucc = (PFRM)Aradesh->Idle->Animation->FRMList6[Aradesh->dir]->First();
			  if (MouseIn(TerX+(ix+226)*16+FrmCucc->bx,
				         TerY+(iy)*12+FrmCucc->by,
						 TerX+(ix+226)*16+FrmCucc->bx+FrmCucc->x,
						 TerY+(iy)*12+FrmCucc->by+FrmCucc->y))
			  {
				  GamePos=5;
			  }
		  };
		  FrmCucc=NULL;
	}	
    
	}

	if ((mousetyp==9) && (Frame==40))
	{
	for (i2=0; i2<145; i2++)
	for (ix=0; ix<165; ix++) 
	{
		  iy = i2;
		  i = ItemInf->Map[ix][iy];
		  if ((i > 0) && (i<50))
		  {	
			  i--;
			  if (MouseIn(TerX+(ix+226)*16+ItemMap[i][0],
				         TerY+(iy)*12+ItemMap[i][1],
						 TerX+(ix+226)*16+ItemMap[i][0]+ItemInf->Tiles[i]->FRM->x,
						 TerY+(iy)*12+ItemMap[i][1]+ItemInf->Tiles[i]->FRM->y))
			  {
				 switch (i)
				 {
				 case 0:AddString("You see: Láda",1);break;
				 case 1:AddString("You see: Tage",1);AddString("Ez egy felirat",0);break;
				 case 2:AddString("You see: Asztal",1);AddString("Egy fa asztal",0);break;
				 case 3:AddString("You see: Szék",1);break;
				 case 4:AddString("You see: Ágy",1);break;
				 case 5:AddString("You see: Kályha",1);AddString("Kicsit rozsdás",0);break;
				 case 6:AddString("You see: Utcai lámpa",1);AddString("Valószínüleg már nem",0);AddString("müködik",0);break;
				 case 7:AddString("You see: WC",1);break;
				 case 8:AddString("You see: Fa",1);break;
				 case 9:AddString("You see: Fa",1);break;
				 case 10:AddString("You see: Fa",1);break;
				 case 11:AddString("You see: Fa",1);break;
				 case 12:AddString("You see: FA",1);break;
				 case 13:AddString("You see: Kút",1);AddString("De víz már nincs benne",0);break;
				 }
				  
			  }
		  }
		  		  if ((Aradesh->x == ix) && (Aradesh->y == iy))  {
			  FrmCucc = (PFRM)Aradesh->Idle->Animation->FRMList6[Aradesh->dir]->First();
			  if (MouseIn(TerX+(ix+226)*16+FrmCucc->bx,
				         TerY+(iy)*12+FrmCucc->by,
						 TerX+(ix+226)*16+FrmCucc->bx+FrmCucc->x,
						 TerY+(iy)*12+FrmCucc->by+FrmCucc->y))
			  {
				  AddString("You see: Aradesh",1);
			  }
		  };
		  FrmCucc=NULL;
		  for (i=0; i<10;i++)
		  if ((Paraszt[i]->x == ix) && (Paraszt[i]->y == iy))
		  {
			  FrmCucc = (PFRM)Paraszt[i]->Idle->Animation->FRMList6[Paraszt[i]->dir]->First();
			  if (MouseIn(TerX+(ix+226)*16+FrmCucc->bx,
				         TerY+(iy)*12+FrmCucc->by,
						 TerX+(ix+226)*16+FrmCucc->bx+FrmCucc->x,
						 TerY+(iy)*12+FrmCucc->by+FrmCucc->y))
			  {
				  AddString("You see: Lakos",1);
			  }
			
		  };
		  
	  }
       
	}

	if (MouseIn(0,0,640,380))
	{
	  if (mousetyp==0) BlitTo(g_pDDSBack,0,0,32,16,TerX+(x+226)*16,TerY+(y)*12,DDBLTFAST_SRCCOLORKEY,Select2->FRM->FRM);
	  if ((mousetyp==0) && (BlDat[x][y] == 1) && (Frame>40) && !((x==Ian->x) && (y==Ian->y))) IanOutTextC(TerX+(x+226)*16+16,TerY+y*12,4,"X");
	  BlitTo(g_pDDSBack, 0,0,640,99,0,381,0,KezeloP->FRM->FRM);
	  DrawStuff();
	  if (mousetyp==9) BlitTo(g_pDDSBack,0,0,Mouse2->FRM->x,Mouse2->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse2->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack, 0,0,640,99,0,381,0,KezeloP->FRM->FRM);
	  DrawStuff();
	  if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
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

	if (MouseIn(0,0,640,379) && (mousetyp==0) && (dims.rgbButtons[0] & 0x80) && !(olddims.rgbButtons[0] & 0x80)) Ian->GenerateTree(x,y,BlDat);
	

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

void Talk(void)
{
	HRESULT hRet;
	CString VLista[15];
	int		GoToList[15];
	CString DoList[15];
	int		NumList[15];

	char buf1[80];
	char buf2[80];

	static char filename[80] = "master.dat\\text\\talk\\duma.sve";

	static int curpos = 1;
	static int page = 1;

	int i,i2,i3,ib,ie;

	/*DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(255,255,255);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);*/

	if ((lstrcmpi(FullScreen->fname,"master.dat\\art\\interface\\alltlk.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"master.dat\\art\\interface\\alltlk.frm",1);
	}

	olddims = dims;
	UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;

	if (MousX>=640) { MousX=640;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=480) { MousY=480;}
	if (MousY<=0)   { MousY=0;  }

	BlitTo(g_pDDSBack,0,0,640,480,0,0,0,FullScreen->FRM->FRM);

	wsprintf(buf1,"text%i",curpos);

	i2=1;
	i3=1;
	for (i=0; i<15; i++)
	{
	   wsprintf(buf2,"cho%i_%i",i2,i3);
	   if (GetFromProf(filename,buf1,buf2) == "")
	   {
		   NumList[i2]=i3-1;
		   i3=1;
		   i2++;
		   wsprintf(buf2,"cho%i_%i",i2,i3);
	   }
	   VLista[i]=GetFromProf(filename,buf1,buf2);
	   if ((i3==1) && (VLista[i]!=""))
	   {
		   VLista[i]=char(' ')+VLista[i];
		   VLista[i]=char('~'+1)+VLista[i];
	   } else
	   {
		   VLista[i]=char(' ')+VLista[i];
		   VLista[i]=char(' ')+VLista[i];
	   }
	   wsprintf(buf2,"cho%ia",i2);
	   GoToList[i]=atoi(GetFromProf(filename,buf1,buf2));
	   wsprintf(buf2,"cho%ib",i2);
	   DoList[i]=GetFromProf(filename,buf1,buf2);
	   i3++;
	}

	for (i=1; i<=4; i++)
	{
	  wsprintf(buf2,"text%i",(page-1)*4+i);
	  WriteFromProf(140,230+(i-1)*10,1,1,filename,buf1,buf2);
	}

	i2=0;
	i3=0;ib=-1;ie=-1;
	for (i=0; i<15; i++)
	{
	  i3++;
	  if (MouseIn(130,330+i2*12,515,329+(i2+NumList[i3])*12))
	  {
	    ib=i2;ie=i2+NumList[i3]-1;
	  }
	  i2+=NumList[i3];
	  DisplayNum(0,i*12,1,NumList[i],3);
	  DisplayNum(40,i*12,1,i,3);
	}
	
    /*for (i=0; i<15; i++)
	{
	  if (MouseIn(130,330+i*12,515,340+i*12))
	  {
	   IanOutText(130,330+i*12,2,VLista[i]);
	   if ((Pressed(0)) && (GoToList[i]!=0))
	   {
		 page = 1;
		 curpos = GoToList[i];
	   }
	  } else
	  {
	   IanOutText(130,330+i*12,1,VLista[i]);
	  }
	}*/	

	for (i=0; i<15; i++)
	{
	  if ((i>=ib) && (i<=ie))
	  {
	   IanOutText(120+i,330+i*12,0,VLista[i]);
	   if ((Pressed(0)) && !((GoToList[i]==0) && (DoList[i]=="")))
	   {
		 if (GoToList[i]==0)
		 {
		  if (DoList[i]=="EXIT")
		  {
			  page = 1;
			  curpos = 1;
			  GamePos=1;
		  }
		  if (DoList[i]=="KILL")
		  {
			  page = 1;
			  curpos = 1;
			  GamePos=1;
		  }
		 } else
		 {
		  page = 1;
		  curpos = GoToList[i];
		 }
	   }
	  } else
	  {
	   IanOutText(120+i,330+i*12,1,VLista[i]);
	  }
	}

	if (MouseIn(140,270,520,280))
	{
		BlitTo(g_pDDSBack,0,0,MouseScr[4][0]->FRM->x, MouseScr[4][0]->FRM->y,MousX-MouseScr[4][0]->FRM->x/2,MousY-MouseScr[4][0]->FRM->y,DDBLTFAST_SRCCOLORKEY,MouseScr[4][0]->FRM->FRM);
		wsprintf(buf2,"text%i",(page)*4+1);
		if ((Pressed(0)) && (GetFromProf(filename,buf1,buf2)!="")) page++;
	} else
	if (MouseIn(140,230,520,240))
	{
		BlitTo(g_pDDSBack,0,0,MouseScr[0][0]->FRM->x, MouseScr[0][0]->FRM->y,MousX-MouseScr[0][0]->FRM->x/2,MousY,DDBLTFAST_SRCCOLORKEY,MouseScr[0][0]->FRM->FRM);	 
		if (Pressed(0)) page--;
	} else
	{
		BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);	 
	} 

	if (page<1) page=1;
	
	while (TRUE)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if (hRet == DD_OK)
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

void 
CreateChar(void)
{
    HRESULT                     hRet;
		
	static int					Frame = -25;
	static DWORD				ThisTick;
	static DWORD				LastTick = 0;
	char						buf[80];
	char						buf2[80];
	int							i,i2;
	static int					Points = 5;

	CountSk(&IanSkill);


	ThisTick = GetTickCount();

	if ((lstrcmpi(FullScreen->fname,"master.dat\\art\\interface\\edtrcrte.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"master.dat\\art\\interface\\edtrcrte.frm",1);
	}

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3;
			
        }

	olddims = dims;
	UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;

	if (MousX>=640) { MousX=640;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=480) { MousY=480;}
	if (MousY<=0)   { MousY=0;  }

	BlitTo(g_pDDSBack,0,0,640,480,0,0,0,FullScreen->FRM->FRM);

	for (i=0; i<7; i++)
	{
	  if ((IanSkill.primary[i]>10) || (IanSkill.primary[i]<1))
	   DisplayNum(44,37+i*33,7,IanSkill.primary[i],2); else
	   DisplayNum(44,37+i*33,6,IanSkill.primary[i],2);
		  
	  wsprintf(buf,"typ%i",IanSkill.primary[i]);
	  if (Selected==i+1) WriteFromProf(100,37+i*33+3,0,1,".\\master.dat\\text\\dialog\\char.sve","Primaryskill",buf);
					else WriteFromProf(100,37+i*33+3,1,1,".\\master.dat\\text\\dialog\\char.sve","Primaryskill",buf);
	  if ((MInPr(152,37+i*33,165,45+i*33,0)) && (Points>=1) && (IanSkill.primary[i]<10))
	  { IanSkill.AlapSk.primary[i]++; Points--; }
	  if ((MInPr(152,46+i*33,165,57+i*33,0)) && (IanSkill.primary[i]>1))
	  { IanSkill.AlapSk.primary[i]--; Points++; }
	  if (MInPr(20,38+i*33,166,60+i*33,0)) Selected = i+1;
	 
	}

	DisplayNum(111,282,6,Points,2);
	DisplayNum(508,228,6,3,2);

	WriteFromProf(86,8,2,2,".\\master.dat\\text\\dialog\\char.sve","Global","def1");
	WriteFromProf(195,8,2,2,".\\master.dat\\text\\dialog\\char.sve","Global","def2");
	WriteFromProf(274,8,2,2,".\\master.dat\\text\\dialog\\char.sve","Global","def3");
		
	WriteFromProf(355,8,2,1,".\\master.dat\\text\\dialog\\char.sve","Global","def4");
	WriteFromProf(355,233,2,1,".\\master.dat\\text\\dialog\\char.sve","Global","def5");

	WriteFromProf(22,286,2,1,".\\master.dat\\text\\dialog\\char.sve","Global","def6");
	WriteFromProf(25,330,2,1,".\\master.dat\\text\\dialog\\char.sve","Global","def7");

	if (MInPr(190,43,320,55,0)) Selected=8;
	if (Selected==8)
	WriteFromProf(190,43,0,1,".\\master.dat\\text\\dialog\\char.sve","Health","hea1"); else
	WriteFromProf(190,43,1,1,".\\master.dat\\text\\dialog\\char.sve","Health","hea1");

	IanSkill.HP = IanSkill.AlapSk.HP;
	wsprintf(buf,"%i/%i",IanSkill.HP,IanSkill.AlapSk.HP);
	IanOutText(190+64,43,1,buf);

	for (i=1; i<8; i++)
	{
		wsprintf(buf,"hea%i",i+1);
		if (MInPr(190,43+i*13,320,55+i*13,0)) Selected=8+i;
		if (Selected==8+i)
		WriteFromProf(190,43+i*13,0,1,".\\master.dat\\text\\dialog\\char.sve","Health",buf); else
		WriteFromProf(190,43+i*13,2,1,".\\master.dat\\text\\dialog\\char.sve","Health",buf); 
	}
	
	for (i=0; i<10; i++)
	{
		wsprintf(buf,"sec%i",i+1);
		if (MInPr(190,177+i*13,320,177+12+i*13,0)) Selected=16+i;
		if (Selected==16+i)
		WriteFromProf(190,177+i*13,0,1,".\\master.dat\\text\\dialog\\char.sve","Secondary",buf); else
		WriteFromProf(190,177+i*13,1,1,".\\master.dat\\text\\dialog\\char.sve","Secondary",buf);
		if ((i==4) || (i==5) || (i==9))
		wsprintf(buf,"%i%%",IanSkill.secondary[i]); else
		wsprintf(buf,"%i",IanSkill.secondary[i]); 
		IanOutText(285,177+i*13,1,buf);
	}

	for (i=0; i<18; i++)
	{
		wsprintf(buf,"ski%i",i+1);
		if (MInPr(346,24+11*i,600,24+11*i+10,0)) Selected=42+i;
		if (MInPr(346,26+11*i,346+17,26+11*i+10,0)) IanSkill.tagSk[i]=!IanSkill.tagSk[i];
		if  ((IanSkill.tagSk[i]) && !(Selected==42+i))
		WriteFromProf(375,24+11*i,3,1,".\\master.dat\\text\\dialog\\char.sve","Skills",buf); else
		if ((Selected==42+i) && !(IanSkill.tagSk[i]))
		WriteFromProf(375,24+11*i,0,1,".\\master.dat\\text\\dialog\\char.sve","Skills",buf); else
		if (Selected==42+i)
		WriteFromProf(375,24+11*i,2,1,".\\master.dat\\text\\dialog\\char.sve","Skills",buf); else
		WriteFromProf(375,24+11*i,1,1,".\\master.dat\\text\\dialog\\char.sve","Skills",buf);
		if ((MouseIn(346,26+11*i,346+17,26+11*i+10)) && (dims.rgbButtons[0] & 0x80))
		{
			i2=1;
		} else
		{
			i2=0;
		}
		BlitTo(g_pDDSBack,0,0,Buttons[i2]->FRM->x,Buttons[i2]->FRM->y,346,26+11*i,0,Buttons[i2]->FRM->FRM);
	} 
	for (i=0; i<18; i++)
	{
	 wsprintf(buf,"%i%%",IanSkill.skills[i]);
	 IanOutText(575,24+11*i,1,buf);
	}

	for (i=0; i<8; i++)
	{
		wsprintf(buf,"tra%i",i+1);

		if (MInPr(25,350+13*i,168,350+13*i+12,0)) Selected=26+i;
		if (MInPr(25,350+13*i,25+17,350+13*i+12,0)) IanSkill.perktra[i]=!IanSkill.perktra[i];
		if ((Selected==26+i) || (IanSkill.perktra[i]))
		WriteFromProf(47,350+13*i,0,1,".\\master.dat\\text\\dialog\\char.sve","Traits",buf); else
		WriteFromProf(47,350+13*i,1,1,".\\master.dat\\text\\dialog\\char.sve","Traits",buf);

		if (MInPr(168,350+13*i,317,350+13*i+12,0)) Selected=34+i;
		if (MInPr(300,350+13*i,317,350+13*i+12,0)) IanSkill.perktra[i+8]=!IanSkill.perktra[i+8];
		wsprintf(buf,"tra%i",i+9);
		if ((Selected==34+i) || (IanSkill.perktra[i+8]))
		WriteFromProf(290,350+13*i,0,3,".\\master.dat\\text\\dialog\\char.sve","Traits",buf); else
		WriteFromProf(290,350+13*i,1,3,".\\master.dat\\text\\dialog\\char.sve","Traits",buf);
		
		if ((MouseIn(25,353+13*i,25+17,353+13*i+10)) && (dims.rgbButtons[0] & 0x80))
		{ i2=1;	} else	{ i2=0;	}
		BlitTo(g_pDDSBack,0,0,Buttons[i2]->FRM->x,Buttons[i2]->FRM->y,25,353+13*i,0,Buttons[i2]->FRM->FRM);
		if ((MouseIn(300,353+13*i,317,353+13*i+10)) && (dims.rgbButtons[0] & 0x80))
		{ i2=1;	} else	{ i2=0;	}
		BlitTo(g_pDDSBack,0,0,Buttons[i2]->FRM->x,Buttons[i2]->FRM->y,300,353+13*i,0,Buttons[i2]->FRM->FRM);
	}

	wsprintf(buf2,"generic.frm");
	switch (Selected)
	{
	 case 1:wsprintf(buf2,"strength.frm");break;
	 case 2:wsprintf(buf2,"perceptn.frm");break;
	 case 3:wsprintf(buf2,"endur.frm");break;
	 case 4:wsprintf(buf2,"charisma.frm");break;
	 case 5:wsprintf(buf2,"intel.frm");break;
	 case 6:wsprintf(buf2,"agility.frm");break;
	 case 7:wsprintf(buf2,"luck.frm");break;

	 case 8:wsprintf(buf2,"hitpoint.frm");break;
	 case 9:case 10:case 11:
	 case 12:case 13:case 14:
	 case 15:wsprintf(buf2,"poisoned.frm");break;
	 case 16:wsprintf(buf2,"armorcls.frm");break;
	 case 17:wsprintf(buf2,"actionpt.frm");break;
	 case 18:wsprintf(buf2,"carryamt.frm");break;
	 case 19:wsprintf(buf2,"meleedam.frm");break;
	 case 20:wsprintf(buf2,"damresis.frm");break;
	 case 21:wsprintf(buf2,"poisnres.frm");break;
	 case 22:wsprintf(buf2,"here&now.frm");break;
	 case 23:wsprintf(buf2,"sequence.frm");break;
	 case 24:wsprintf(buf2,"healrate.frm");break;
	 case 25:wsprintf(buf2,"critchnc.frm");break;

	 case 26:wsprintf(buf2,"fastmeta.frm");break;
	 case 27:wsprintf(buf2,"bruiser.frm");break;
	 case 28:wsprintf(buf2,"smlframe.frm");break;
	 case 29:wsprintf(buf2,"onehand.frm");break;
	 case 30:wsprintf(buf2,"stranger.frm");break;
	 case 31:wsprintf(buf2,"kamikaze.frm");break;
	 case 32:wsprintf(buf2,"heavyhnd.frm");break;
	 case 33:wsprintf(buf2,"fastshot.frm");break;

	 case 34:wsprintf(buf2,"bldmess.frm");break;
	 case 35:wsprintf(buf2,"jinxed.frm");break;
	 case 36:wsprintf(buf2,"goodnatr.frm");break;
	 case 37:wsprintf(buf2,"addict.frm");break;
	 case 38:wsprintf(buf2,"drugrest.frm");break;
	 case 39:wsprintf(buf2,"empathy.frm");break;
	 case 40:wsprintf(buf2,"skilled.frm");break;
	 case 41:wsprintf(buf2,"gifted.frm");break;

	 case 42:wsprintf(buf2,"gunsml.frm");break;
	 case 43:wsprintf(buf2,"gunbig.frm");break;
	 case 44:wsprintf(buf2,"energywp.frm");break;
	 case 45:wsprintf(buf2,"unarmed.frm");break;
	 case 46:wsprintf(buf2,"melee.frm");break;
	 case 47:wsprintf(buf2,"throwing.frm");break;
	 case 48:wsprintf(buf2,"firstaid.frm");break;
	 case 49:wsprintf(buf2,"doctor.frm");break;
     case 50:wsprintf(buf2,"sneak.frm");break;
	 case 51:wsprintf(buf2,"lockpick.frm");break;
	 case 52:wsprintf(buf2,"steal.frm");break;
	 case 53:wsprintf(buf2,"traps.frm");break;
	 case 54:wsprintf(buf2,"science.frm");break;
	 case 55:wsprintf(buf2,"repair.frm");break;
	 case 56:wsprintf(buf2,"speech.frm");break;
	 case 57:wsprintf(buf2,"barter.frm");break;
	 case 58:wsprintf(buf2,"gambling.frm");break;
	 case 59:wsprintf(buf2,"outdoors.frm");break;
	}
	wsprintf(buf,"master.dat\\art\\skilldex\\%s",buf2);
	if ((lstrcmpi(SkillDex->fname,buf)))
	{
      LoadFRMSingle(&SkillDex,hWnd,buf,1);
	}
	BlitTo(g_pDDSBack,0,0,140,117,480,310,0,SkillDex->FRM->FRM);

	for (i=0; i<10; i++)
	{
		wsprintf(buf,"de%i_%i",Selected,i);
		WriteFromProf(350,280+i*14,11,1,".\\master.dat\\text\\dialog\\chard.sve","Dialog",buf);
	}

	if ((dims.rgbButtons[0] & 0x80) && MouseIn(457,450,457+26,450+26))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,457,450,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,457,450,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutText(457+30,457,2,"Vége");

	DisplayFrameRate();
	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
	
    // Flip the surfaces
    while (TRUE)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if (hRet == DD_OK)
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
	if (MInPr(457,450,457+26,450+26,0)) { FadeOutWh(); ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0); GamePos = 1; SetPaletteBl(255); }
}


#include "ddex4_u2.h"