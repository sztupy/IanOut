void SetPaletteBl(int Frame)
{
	int i;
	int Blue;
	int Green;
	int Red;
	if (Frame>255) Frame=255;
	if (Frame<0) Frame=0;
	for ( i = 0; i<=255; i++)
	{
		Blue = Frame;
		Green = Frame;
		Red = Frame;
		
		if (Blue>(int)(pe[i].peBlue*gamma)) Blue = (int)(pe[i].peBlue*gamma);
		if (Red>(int)(pe[i].peRed*gamma)) Red = (int)(pe[i].peRed*gamma);
		if (Green>(int)(pe[i].peGreen*gamma)) Green = (int)(pe[i].peGreen*gamma);

		if (Blue>255) Blue=255;
		if (Red>255) Red=255;
		if (Green>255) Green=255;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		pe2[i].peBlue = Blue;
		pe2[i].peRed = Red;
		pe2[i].peGreen = Green;
	
	}
	if (g_pDDPal->SetEntries(0, 0, 256, pe2) != DD_OK)
          return;
}

HRESULT RestoreAll();

void FadeOut()
{
	int i;
	HRESULT hRet;

	 //g_pDDSPrimary->Blt(NULL,g_pDDSBack,NULL,0,NULL);
	g_pDDSBack->Blt(NULL,g_pDDSPrimary,NULL,0,NULL);
			
	for (i=255; i>0; i-=4)
	{
		SetPaletteBl(i);
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
	
}

void FadeIn()
{
	int i;
	HRESULT hRet;

	 //g_pDDSPrimary->Blt(NULL,g_pDDSBack,NULL,0,NULL);
	 g_pDDSBack->Blt(NULL,g_pDDSPrimary,NULL,0,NULL);
			
	for (i=0; i<255; i+=4)
	{
		SetPaletteBl(i);
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
	
}


void SetPaletteNo()
{
	g_pDDPal->SetEntries(0, 0, 256, pe);     
}


void SetPaletteWh(int Frame)
{
	int i;
	int Blue;
	int Green;
	int Red;
	if (Frame>255) Frame=255;
	if (Frame<0) Frame=0;
	for ( i = 0; i<=255; i++)
	{
		Blue = 255-Frame;
		Green = 255-Frame;
		Red = 255-Frame;
		
		if (Blue<(int)(pe[i].peBlue*gamma)) Blue = (int)(pe[i].peBlue*gamma);
		if (Red<(int)(pe[i].peRed*gamma)) Red = (int)(pe[i].peRed*gamma);
		if (Green<(int)(pe[i].peGreen*gamma)) Green = (int)(pe[i].peGreen*gamma);

		if (Blue>255) Blue=255;
		if (Red>255) Red=255;
		if (Green>255) Green=255;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		pe2[i].peBlue = Blue;
		pe2[i].peRed = Red;
		pe2[i].peGreen = Green;
	}
	
	if (g_pDDPal->SetEntries(0, 0, 256, pe2) != DD_OK)
          return;
}

void FadeOutWh()
{
	int i;
	HRESULT hRet;

	 //g_pDDSPrimary->Blt(NULL,g_pDDSBack,NULL,0,NULL);
	g_pDDSBack->Blt(NULL,g_pDDSPrimary,NULL,0,NULL);
			
	for (i=255; i>0; i-=4)
	{
		SetPaletteWh(i);
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
	
}

HRESULT 
RestoreAll(void)
{
    HRESULT                     hRet;

    hRet = g_pDDSPrimary->Restore();
    if (hRet == DD_OK)
    {
        hRet = g_pDDSOne->Restore();
        if (hRet == DD_OK)
        {
            DDReLoadBitmap(g_pDDSOne, szBitmap);
        }
    }
    return hRet;
}


void DrawChar(
			  int  x,
			  int  y,
			  int  co,
			  char c)
{
	RECT		rcRect;

	rcRect.left   = FontMap[(c-32)]+1;
	rcRect.top    = (co*24)+1;
	rcRect.right  = FontMap[(c-31)]+1;
	rcRect.bottom = (co*24)+12;

	if (rcRect.left>467) 
	{
		rcRect.left-=468;
		rcRect.right-=468;
		rcRect.top+=12;
		rcRect.bottom+=12;
	}

	BlitTo(g_pDDSBack,rcRect.left,rcRect.top,rcRect.right,rcRect.bottom,x,y,DDBLTFAST_SRCCOLORKEY ,g_pDDSOne);
}


void IanOutText(int x, int y, int co, CString str)
{
	int		i;
	int		jelenx = x;
	int		sor = y+3;
	int		length = str.GetLength();
	int		maxjelenx = 0;
	int		maxsor = 0;
	HRESULT hRet;
	HDC     hdc;

	if (co==11)
	{
		
      hRet = g_pDDSBack->GetDC(&hdc);
	  if (!FAILED(hRet))
	  {
		
		SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,RGB(0,0,0));
		TextOut(hdc, x,y,str,length);
		g_pDDSBack->ReleaseDC(hdc);
	  }
	}
	
	for (i=0 ; i<length; i++)
	{
		if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='õ') &&
			(str[i]!='ü') &&
			(str[i]!='û') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Õ') &&
			(str[i]!='Ü') &&
			(str[i]!='Û') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			DrawChar(jelenx,sor,co,str[i]);
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
		if (str[i]=='\n')
		{
			sor += 12;
			jelenx=x;
		} else
	    if (str[i]=='á')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'a');
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'e');
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
	        DrawChar(jelenx-1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'i');
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if ((str[i]=='ö') || (str[i]=='õ'))
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if ((str[i]=='ü') || (str[i]=='û'))
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
	        DrawChar(jelenx+5,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'A');
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'E');
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
	        DrawChar(jelenx,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'I');
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if ((str[i]=='Ö') || (str[i]=='Õ'))
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if ((str[i]=='Ü') || (str[i]=='Û'))
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
		if (jelenx>maxjelenx) maxjelenx=jelenx;
		if (sor+13>maxsor) maxsor=sor+13;
	}

}

void IanOutTextC(int x, int y, int co, CString str)
{
	int		i;
	int		jelenx = 0;
	int		sor = y+3;
	int		length = str.GetLength();
	int     kihossz = 0;
	int		maxjelenx = 0;
	int		maxsor = 0;

	for (i=0 ; i<length; i++)
	{
			if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='ü') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Ü') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
	    if (str[i]=='á')
		{
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if (str[i]=='ö')
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ü')
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if (str[i]=='Ö')
		{
	     	jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ü')
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
	}

	kihossz = jelenx;
	jelenx = x-kihossz/2;
	for (i=0 ; i<length; i++)
	{
		if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='ü') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Ü') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			DrawChar(jelenx,sor,co,str[i]);
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
		if (str[i]=='á')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'a');
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'e');
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
	        DrawChar(jelenx-1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'i');
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if (str[i]=='ö')
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ü')
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
	        DrawChar(jelenx+5,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'A');
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'E');
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
	        DrawChar(jelenx,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'I');
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if (str[i]=='Ö')
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ü')
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
		if (jelenx>maxjelenx) maxjelenx=jelenx;
		if (sor+13>maxsor) maxsor=sor+13;
	}
}


void DisplayFrameRate(void)
{
    DWORD               time2;
    char                buff[256];
    
    dwFrameCount++;
    time2 = GetTickCount() - dwFrameTime;
    if( time2 > 1000 )
    {
        dwFrames = (dwFrameCount*1000)/time2;
        dwFrameTime = GetTickCount();
        dwFrameCount = 0;
    }
    if( dwFrames == 0 )
    {
        return;
    }
    
    if (dwFrames != dwFramesLast)
    {
        dwFramesLast = dwFrames;
    }
    
    if( dwFrames > 99 )
    {
        dwFrames = 99;
    }
    buff[0] = (char)((dwFrames / 10) + '0');
    buff[1] = (char)((dwFrames % 10) + '0');
	buff[2] = ' ';
	buff[3] = 'F';
	buff[4] = 'P';
	buff[5] = 'S';
    buff[6] = '\0';
    IanOutText(0,0,3,buff);
}

void IanOutTextR(int x, int y, int co, CString str)
{
	int		i;
	int		jelenx = 0;
	int		sor = y+3;
	int		length = str.GetLength();
	int     kihossz = 0;
	int		maxjelenx = 0;
	int		maxsor = 0;

	for (i=0 ; i<length; i++)
	{
			if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='ü') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Ü') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
	    if (str[i]=='á')
		{
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if (str[i]=='ö')
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ü')
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if (str[i]=='Ö')
		{
	     	jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ü')
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
	}

	kihossz = jelenx;
	jelenx = x-kihossz;
	for (i=0 ; i<length; i++)
	{
		if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='ü') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Ü') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			DrawChar(jelenx,sor,co,str[i]);
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
		if (str[i]=='á')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'a');
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'e');
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
	        DrawChar(jelenx-1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'i');
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if (str[i]=='ö')
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ü')
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
	        DrawChar(jelenx+5,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'A');
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'E');
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
	        DrawChar(jelenx,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'I');
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if (str[i]=='Ö')
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ü')
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
		if (jelenx>maxjelenx) maxjelenx=jelenx;
		if (sor+13>maxsor) maxsor=sor+13;
	}
}



void IanOutNum1(int x, int y, int c,CString str)
{
	int		i;
	int		length = str.GetLength();
	int     be;

	for (i=0; i<length; i++)
	{
		if ((str[i]>='0') && (str[i]<='9'+2))
		{
			be = (str[i]-'0')*14;
			if (c==7) be+=168;
			BlitTo(g_pDDSBack,be,144,be+14,168,x+i*14,y,0,g_pDDSOne);
		}
	}
}

void IanOutNum2(int x, int y, int c,CString str)
{
	int		i;
	int		length = str.GetLength();
	int     be;

	for (i=0; i<length; i++)
	{
		if ((str[i]>='0') && (str[i]<='9'+2))
		{
			be = (str[i]-'0')*9;
			if (c==9) be+=120;
			if (c==10) be+=240;
			BlitTo(g_pDDSBack,be,169,be+9,186,x+i*9,y,0,g_pDDSOne);
		}
		if ((str[i]=='-'))
		{
			be = 12*9;
			if (c==9) be+=120;
			if (c==10) be+=240;
			BlitTo(g_pDDSBack,be,169,be+9,186,x+i*9,y,0,g_pDDSOne);
		}
	}
}

void DisplayNum(int x, int y, int c,short i, short count)
{
    char                buff[256];
    int					i2,i3;
	i2=0;
	for (i3=1; i3<=count;i3++)
	{
		i2=i2*10;
		i2=i2+1;
	}
	i2=9*i2;
	
	buff[0]=' ';
    if( i > i2 )
    {
        i = i2;
    }
	if (i<-i2)
	{
		i = -i2;
	}
	if (i<0) { buff[0] = '-'; i =-i;};
	
	switch(count)
	{
	case 1:
		buff[1]=(char)i+'0';
		break;
	case 2:
		buff[1] = (char)((i / 10) + '0');
		buff[2] = (char)((i % 10) + '0');
		break;
	case 3:
		buff[1] = (char)((i / 100) + '0');i-=i/100*100;
		buff[2] = (char)((i / 10) + '0');
		buff[3] = (char)((i % 10) + '0');
		break;
	case 4:
		buff[1] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[2] = (char)((i / 100) + '0');i-=i/100*100;
		buff[3] = (char)((i / 10) + '0');
		buff[4] = (char)((i % 10) + '0');
		break;
	case 5:
		buff[1] = (char)((i / 10000) + '0');i-=i/10000*10000;
		buff[2] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[3] = (char)((i / 100) + '0');i-=i/100*100;
		buff[4] = (char)((i / 10) + '0');
		buff[5] = (char)((i % 10) + '0');
		break;
	}
	buff[count+1] = '\0';
	if ((c>=0) && (c<=5)) IanOutText(x,y,c,buff);
	if ((c>=6) && (c<=7)) IanOutNum1(x,y,c,buff);
	if ((c>=8) && (c<=10)) IanOutNum2(x,y,c,buff);
}

void WriteFromProf(int x,int y,int c,int i3,CString fnam,CString master,CString slave)
{
	int i,i2;
	char buf[80];
	i2=-1;
	for (i=0; i<75; i++)
	{
		if (LINIStr[i]->FindIt(fnam,master,slave) == 0) i2=i;
	}
	if (i2!=-1)
	{
	 wsprintf(buf,"%s",LINIStr[i2]->Get());
	 if (i3==1) IanOutText(x,y,c,buf);
	 if (i3==2) IanOutTextC(x,y,c,buf);
	 if (i3==3) IanOutTextR(x,y,c,buf);
	} else
	{
	  GetPrivateProfileString(master,slave,"",buf,80,fnam);
	  for (i=1; i<75; i++)
	  {
		LINIStr[i-1]->Set(LINIStr[i]->fname,LINIStr[i]->master,LINIStr[i]->slave,LINIStr[i]->buffer);
	  }
	  LINIStr[74]->Set(fnam,master,slave,buf);
	  if (i3==1) IanOutText(x,y,c,buf);
	 if (i3==2) IanOutTextC(x,y,c,buf);
	 if (i3==3) IanOutTextR(x,y,c,buf);
	}
}

CString GetFromProf(CString fnam,CString master,CString slave)
{
	int i,i2;
	char buf[80];
	i2=-1;
	for (i=0; i<75; i++)
	{
		if (LINIStr[i]->FindIt(fnam,master,slave) == 0) i2=i;
	}
	if (i2!=-1)
	{
	 wsprintf(buf,"%s",LINIStr[i2]->Get());
	 return buf;
	} else
	{
	  GetPrivateProfileString(master,slave,"",buf,80,fnam);
	  for (i=1; i<75; i++)
	  {
		LINIStr[i-1]->Set(LINIStr[i]->fname,LINIStr[i]->master,LINIStr[i]->slave,LINIStr[i]->buffer);
	  }
	  LINIStr[74]->Set(fnam,master,slave,buf);
	  return buf;
	}
}