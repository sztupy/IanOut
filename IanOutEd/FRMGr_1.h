// --- TFRMAnim ---
//
// ----------------

int TFRMAnim::LoadFRM(int stream, LPVOID LoadIn, int x, int y, int p)
{
   LPSTR		PointTo;
   int			i;
	
   PointTo = (LPSTR)LoadIn;
   for (i=0; i<y; i++)
   {
    _read(stream, PointTo+(i*p), x);
   }
    
   return 0;
}

HRESULT TFRMAnim::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD,char* filename)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	PFRM						FRMDat;
	int							stream2;
	int							stream;
	char						filename2[100];
	signed short				movetabl[100][2];
	int							x, y;
	int							i;

	wsprintf(filename2,"%s",filename);
	i = lstrlen(filename2);
	filename2[i-1] = 'p';
	filename2[i-2] = 's';
	filename2[i-3] = 'h';
	 
	wsprintf(filename2,"%s",filename);
	i = lstrlen(filename2);
	filename2[i-1] = 'p';
	filename2[i-2] = 's';
	filename2[i-3] = 'h';
 
	if ((stream2 = _open( filename2,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",filename2);
	
	 _read(stream2,movetabl,sizeof(movetabl));
 	 _close(stream2);

	if ((stream = _open( filename,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",filename);


    _read(stream,buf,9);
    _read(stream,framecount,1);
	_read(stream,buf,52);
	framecounter = framecount[0];
	for (i=1; i<=framecount[0]; i++)
	{
      _read(stream,framedat,12);
	  x=framedat[0]*256+framedat[1];
      y=framedat[2]*256+framedat[3];

      //wsprintf( buf, "(%d, %d) %d %d %d %d",
      //               x,y,framedat[0],framedat[1],framedat[2],framedat[3]);

	  //MessageBox(hWnd, buf, "Hiba", MB_OK);

	  ZeroMemory(&ddsd, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd);
      ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN /*| DDSCAPS_SYSTEMMEMORY */;

      ddsd.dwWidth = x;
      ddsd.dwHeight = y;
	  ddsd.lPitch = x;

	  //FRMDat = new TFRM();
	  FRMList->Insert( new TFRM() );

	  FRMDat = (PFRM)(FRMList->FLast);
	  hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
      if (hRet != DD_OK) return hRet;
		
      FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);
	  LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
	  FRMDat->FRM->Unlock(NULL);

	  FRMDat->x = x;
	  FRMDat->y = y;

      FRMDat->bx = movetabl[i-1][0];
	  FRMDat->by = movetabl[i-1][1];


	  DDSetColorKey(FRMDat->FRM, RGB(0,0,0));

	  //FRMList->Insert( FRMDat );
	}

/*	Counter = (FRMList->FLast);*/
	Counter = FRMList->First();
	
	_close(stream);

	return DD_OK;
}


void TFRMAnim::NextFrame()
{
	Counter = FRMList->RNext(Counter);
}

PFRM TFRMAnim::GetCurFrame()
{
	PFRM Lista;
	Lista = (PFRM)Counter;
	return Lista;
}

TFRMAnim::~TFRMAnim()
{
	if (FRMList)
	{
	 PFRM Lista;
	 Lista = (PFRM)FRMList->First();
	 while (Lista)
	 {
	  Lista->FRM->Release();
	  Lista = (PFRM)FRMList->Next(Lista);
	 }
	 delete FRMList;
	}
	
}
