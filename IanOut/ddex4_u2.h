void Credits(void)
{
	HRESULT hRet;
	DDBLTFX                     ddbltfx;

	CString   Dumcsi[200];


	int i,c;
	static int							Frame = -20;
	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	char								buf[80];
	//char								buf2[80];
	//time_t								ltime,ltime2;
	//tm									pointtm;
	int Stringnumb;
	FILE*	stream;

	Stringnumb=0;
	stream = fopen("master.dat\\text\\credits.txt","rt");

	while (!(feof(stream)))
	{
	  Stringnumb++;
//	  wsprintf(buf,"%i",Stringnumb);
//	  MessageBox(hWnd,buf,buf,MB_OK);

	  //fscanf(stream,"%s",buf);
	  fgets(buf,80,stream);
	  Dumcsi[Stringnumb-1]=buf;
	}
	/*pointtm.tm_hour=9;
	pointtm.tm_isdst=0;
	pointtm.tm_mday=3;
	pointtm.tm_min=30;
	pointtm.tm_mon=0;
	pointtm.tm_sec=0;
	pointtm.tm_yday=3;
	pointtm.tm_year=2001;
	ltime2=mktime(&pointtm);
	time( &ltime );
	Dumcsi[Stringnumb]="0";
	Dumcsi[Stringnumb+1]="2Statisztikai adatok a játékról";
	Dumcsi[Stringnumb+2]="0";
	Dumcsi[Stringnumb+3]="1A játék újraindítása óta eltelt idõ:";
    _gcvt(difftime(ltime2,ltime),20,buf);
	wsprintf(buf2,"3%s másodperc",buf);
	Dumcsi[Stringnumb+4]=buf2;
	Stringnumb+=5;*/
	fclose(stream);

	
	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > 40)
        {
			Frame++;//(ThisTick - LastTick)/40;
            LastTick = ThisTick;
	
        }

    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(0,0,0);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	for (i=0; i<Stringnumb; i++)
	{
		if (((i*12+480-Frame)<=700) && ((i*12+480-Frame)>-15))
		{
			wsprintf(buf,"%s",Dumcsi[i]);
			c=0;
			switch (buf[0]) {
			case '1':c = 1;break;
			case '2':c = 2;break;
			case '3':c = 3;break;
			case '4':c = 4;break;
			case '5':c = 5;break;
			case 'A':c = 0;break;
			case 'B':c = 1;break;
			case 'C':c = 2;break;
			case 'D':c = 3;break;
			case 'E':c = 4;break;
			case 'F':c = 5;break;
			}
			if ((buf[0]>='A') && (buf[0]<='F'))
			{
			 buf[0]=' ';
			 IanOutText(0,i*12+480-Frame,c,buf);
			} else
			{
			 buf[0]=' ';
			 IanOutTextC(315,i*12+480-Frame,c,buf);
			}
		}
	}
	DisplayFrameRate();
	olddims = dims;
	MousX = 200;
	UpdateInputState();
	if (dims.rgbButtons[1] & 0x80) Frame--;
	if (((Stringnumb*12+480-Frame)<-15) || (dims.rgbButtons[0] & 0x80)) { Frame = -20; GamePos=0; FadeOut(); /*SetPaletteBl(255);*/return;}


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

	if (Frame<-10) SetPaletteBl(255);

}

void 
MainMenu(void)
{
    HRESULT                     hRet;
	//DDBLTFX                     ddbltfx;
	
	static int					Frame = -25;
	static DWORD				ThisTick;
	static DWORD				LastTick = 0;
	//int							i;
	
	if ((lstrcmpi(FullScreen->fname,"master.dat\\art\\interface\\mainmenu.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"master.dat\\art\\interface\\mainmenu.frm",1);
		
	}

	ThisTick = GetTickCount();

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

	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,20,56,46))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,20,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,20,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,25,2,"Intro");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,61,56,93))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,61,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,61,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,66,2,"Nyú gáme");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,102,56,134))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,102,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,102,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,107,2,"Betöltés");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,143,56,175))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,143,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,143,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,148,2,"Beállítások");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,184,56,210))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,184,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,184,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,189,2,"Készítök");

	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,225,56,250))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,225,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,225,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,230,2,"Kilépés");

	DisplayFrameRate();

	IanOutText(0,465,1,"IanOut - A Post-Nuclear Ian-Playing Game v"VERSIONNUM);
	IanOutText(455,465,4,"Copyright (c) Sztupy corp.");
	
	if (Frame<256) SetPaletteBl(Frame);

	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80) && MouseIn(30,20,56,46))
      { GamePos=3; MousX=150;Frame = -25; FadeOut(); return;}
	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80) && MouseIn(30,61,56,93))
	  { GamePos=4; Frame=-25;FadeOut(); ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);SetPaletteBl(255);return;}
	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80) && MouseIn(30,184,56,210))
	  { GamePos=2; Frame = -25; FadeOut(); return;}
	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80) && MouseIn(30,225,56,250))
	  { FadeOut(); ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0); PostMessage(hWnd, WM_CLOSE, 0, 0); return;}


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
	
}

void Intro()
{
	HRESULT								hRet;
	DDBLTFX								ddbltfx;
	RECT								rcRect;

	static int							Fram = -255;
	static double						Lines[25];							
	static double						Linpe[25];
	static int							Lince[25];
	static int							Linti[25];
	int									Frame;
	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	char								buf[150];
	char								buf2[10];
	int									stringnumb;
	int									i;

	GetPrivateProfileString("Intro","text","0",buf,80,".\\master.dat\\text\\dialog\\intro.sve");
	stringnumb = atoi(buf);
	ThisTick = GetTickCount();
	
	if (Fram==-255)
	{
	 for (i=0; i<25; i++)
	 {
		 Lines[i]=rand()%540+50;
		 Linpe[i]=((double)(rand()%80-40))/20;
		 Lince[i]=rand()%4+11;
		 Linti[i]=rand()%150;
	 }
	}

	if (Fram<=0) { Fram+=5; } else
	if ((ThisTick - LastTick) > 70)
        {
			Fram++;
            LastTick = ThisTick;
	    }

	for (i=0; i<25; i++) {
		 Lines[i]+=Linpe[i];
		 Linti[i]-=3;
		 if ((Lince[i]<5) || (Lines[i]<50) || (Lines[i]>590))
		 {
		  Lines[i]=rand()%540+50;
		  Linpe[i]=((double)(rand()%80-40))/20;
		  Lince[i]=rand()%4+11;
		  Linti[i]=rand()%150;
		 }
		 if (Linti[i]<0) Lince[i]--;
	}

	Frame = Fram;
	if (Frame<0) Frame=0;
	SetPaletteBl(Fram+255);
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(0,0,0);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 5;
	
	   rcRect.top=50;
	  rcRect.left=50;
	rcRect.bottom=450;
	 rcRect.right=590;
    g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	wsprintf(buf2,"text%i",(Frame/50)+1);
	GetPrivateProfileString("Intro",buf2,"--",buf,80,".\\master.dat\\text\\dialog\\intro.sve");
	IanOutTextC(320,460,0,buf);

	if ((Frame>=-255) && (Frame<=325))
	{
	 if ((lstrcmpi(FullScreen->fname,"master.dat\\art\\interface\\intro1.frm")))
	 {
      LoadFRMSingle(&FullScreen,hWnd,"master.dat\\art\\interface\\intro1.frm",1);
	 }
	 BlitTo(g_pDDSBack,0,0,540,400,50,50,0,FullScreen->FRM->FRM);
	}

	if ((Frame>=321) && (Frame<=665))
	{
	 if ((lstrcmpi(FullScreen2->fname,"master.dat\\art\\interface\\intro2.frm")))
	 {
      LoadFRMSingle(&FullScreen2,hWnd,"master.dat\\art\\interface\\intro2.frm",1);
	 }
	 if ((Frame>=321) && (Frame<=325))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,50,50,0,FullScreen2->FRM->FRM,(50*(Frame-321)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,50,50,0,FullScreen2->FRM->FRM);
	 }
	}

	if ((Frame>=661) && (Frame<=825))
    {
	 if ((lstrcmpi(FullScreen->fname,"master.dat\\art\\interface\\intro4.frm")))
     {
      LoadFRMSingle(&FullScreen,hWnd,"master.dat\\art\\interface\\intro4.frm",1);
     }
     if ((Frame>=661) && (Frame<=665))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,50,50,0,FullScreen->FRM->FRM,(50*(Frame-661)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,50,50,0,FullScreen->FRM->FRM);
	 }
	}

	if ((Frame>=821) && (Frame<=1000))
	{
	 if ((lstrcmpi(FullScreen2->fname,"master.dat\\art\\interface\\intro3.frm")))
	 {
      LoadFRMSingle(&FullScreen2,hWnd,"master.dat\\art\\interface\\intro3.frm",1);
	 }
	 if ((Frame>=821) && (Frame<=825))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,50,50,0,FullScreen2->FRM->FRM,(50*(Frame-821)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,50,50,0,FullScreen2->FRM->FRM);
	 }
	}

	if ((Frame>=0 ) && (Frame<150)) 
	{IanOutTextC(260,200,0,"IanOut");}
	if ((Frame>150) && (Frame<300)) 
	{IanOutTextC(450,400,0,"Program:");
	 IanOutTextC(450,412,0,"Sztupák Szilárd");}
	if ((Frame>300) && (Frame<450)) 
	{IanOutTextC(430,150,0,"Ötlet:");
	 IanOutTextC(430,162,0,"Czehelszki Levente");
	 IanOutTextC(430,174,0,"Sztupák Szilárd");}
	if ((Frame>450) && (Frame<600)) 
	{IanOutTextC(200,350,0,"Szöveg:");
	 IanOutTextC(200,362,0,"Czehelszki Levente");}
	if ((Frame>600) && (Frame<750)) 
	{IanOutTextC(400,300,0,"Grafika:");
	 IanOutTextC(400,312,0,"Sztupák Szilárd");
	 IanOutTextC(400,324,0,"Black Isle Studio");}
	if ((Frame>750) && (Frame<900)) 
	{IanOutTextC(250,180,0,"Zene:");
	 IanOutTextC(250,192,0,"Még nincs");}

	DisplayFrameRate();

	for (i=0; i<10; i++)
	{
	 ZeroMemory(&ddbltfx, sizeof(ddbltfx));
     ddbltfx.dwSize = sizeof(ddbltfx);
     ddbltfx.dwFillColor = Lince[i];
	 rcRect.top=50;
	 rcRect.left=(int)Lines[i];
	 rcRect.bottom=450;
	 rcRect.right=(int)Lines[i]+1;
     g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	}

	UpdateInputState();
	if (((Frame/50)+1)>stringnumb) { Fram = -255; GamePos=0; FadeOut(); /*SetPaletteBl(255);*/return;}
	if ((dims.rgbButtons[0] & 0x80)) { Fram = -255; GamePos=0; FadeOut(); /*SetPaletteBl(255);*/return;}
	
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