// --- TFRMAnim6 ---
//
// -----------------

TFRMAnim6::~TFRMAnim6()
{
	int i;
	for (i=0; i<6; i++)
	{
	if (FRMList6[i])
	{
	 PFRM Lista;
	 Lista = (PFRM)FRMList6[i]->First();
	 while (Lista)
	 {
	  Lista->FRM->Release();
	  Lista = (PFRM)FRMList6[i]->Next(Lista);
	 }
	 delete FRMList6[i];
	}
	}
}

TFRMAnim6::TFRMAnim6()
{
	int i;
	for (i=0; i<6; i++)
	{
	  FRMList6[i] = new TList();
	}
}

HRESULT TFRMAnim6::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD,char* filename)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	PFRM						FRMDat;
	int							stream;
	int							stream2;
	char						filename2[100];
	signed short				movetabl[100][2];
	int							x, y;
	int							i,i2;

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
	for (i2=0; i2<6; i2++)
	for (i=1; i<=framecount[0]; i++)
	{
      _read(stream,framedat,12);
	  x=framedat[0]*256+framedat[1];
      y=framedat[2]*256+framedat[3];

      ZeroMemory(&ddsd, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd);
      ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN /*| DDSCAPS_SYSTEMMEMORY */;

      ddsd.dwWidth = x;
      ddsd.dwHeight = y;
	  ddsd.lPitch = x;

	  FRMList6[i2]->Insert( new TFRM() );

	  FRMDat = (PFRM)(FRMList6[i2]->FLast);
	  hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
      if (hRet != DD_OK) return hRet;
		
      FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);
	  LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
	  FRMDat->FRM->Unlock(NULL);

	  FRMDat->x = x;
	  FRMDat->y = y;

	  FRMDat->bx = movetabl[i2*framecount[0]+i-1][0];
	  FRMDat->by = movetabl[i2*framecount[0]+i-1][1];

	  DDSetColorKey(FRMDat->FRM, RGB(0,0,0));

	 }

	for (i2=0; i2<6; i2++) Counter6[i2] = FRMList6[i2]->First();
	dir = 0;
	
	_close(stream);
	
	return DD_OK;
}

void TFRMAnim6::FirstFrame()
{
	int i2;
	for (i2=0; i2<6; i2++) { Counter6[i2] = FRMList6[i2]->First(); }
}

void TFRMAnim6::NextFrame()
{
	Counter6[dir] = FRMList6[dir]->RNext(Counter6[dir]);
}

PFRM TFRMAnim6::GetCurFrame()
{
	PFRM Lista;
	Lista = (PFRM)Counter6[dir];
	return Lista;
}

void TFRMAnim6::SetDir(int i_dir)
{
	dir = i_dir;
}

/* - TFRMCommunicator - */

TFRMCommunicator::TFRMCommunicator(PFRMAnim6 FRMAssign)
{
   Animation = FRMAssign;
   dir = 0;
   FirstFrame();
}

void TFRMCommunicator::FirstFrame()
{
	int i2;
	for (i2=0; i2<6; i2++) { Count6[i2] = Animation->FRMList6[i2]->First(); }
}

PFRM TFRMCommunicator::GetCurFrame()
{
	PFRM Lista;
	Lista = (PFRM)Count6[dir];
	return Lista;
}
