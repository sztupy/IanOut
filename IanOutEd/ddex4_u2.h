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
	IanOutTextC(123,25,2,"Terep megadása");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,61,56,93))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,61,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,61,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,66,2,"Szerkesztés");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,102,56,134))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,102,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,102,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,107,2,"X");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,143,56,175))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,143,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,143,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,148,2,"X");
	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,184,56,210))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,184,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,184,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,189,2,"X");

	if ((dims.rgbButtons[0] & 0x80) && MouseIn(30,225,56,250))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,225,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,225,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	IanOutTextC(123,230,2,"Kilépés");

	DisplayFrameRate();

	IanOutText(0,465,1,"IanEd - IanOut Level Editor v"VERSIONNUM);
	IanOutText(455,465,4,"Copyright (c) Sztupy corp.");
	
	if (Frame<256) SetPaletteBl(Frame);

	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80) && MouseIn(30,61,56,93))
	  { GamePos=1; Frame=-25;FadeOut(); ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);SetPaletteBl(255);return;}
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
