// --- TFRMSingle ---
//
// ------------------

HRESULT TFRMSingle::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, char* filename, int framenum)
{
    char						buf[100];
	char						buf2[50];						
	unsigned char				framedat[12];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	int							stream;
	int							x, y;
	int							i;
	LPVOID						ize;

	wsprintf(buf2,"%s",filename);
	wsprintf(fname,"%s",filename);
 
	if ((stream = _open( buf2,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",buf2);
	
    _read(stream,buf,62);
    
	for (i=1; i<=framenum; i++)
	{
     _read(stream,framedat,12);
     x=framedat[0]*256+framedat[1];
     y=framedat[2]*256+framedat[3];
	 if (i!=framenum) {
	  ize = malloc(x*y);
	  LoadFRM(stream,ize,x,y,x);
	  free(ize);
	 }
	}

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

    if (FRM) {FRM->FRM->Release();delete FRM;};
      
	FRM = new TFRM();

	hRet = g_pDD->CreateSurface(&ddsd, &FRM->FRM, NULL);
    if (hRet != DD_OK) return hRet;
	
    FRM->FRM->Lock(NULL, &ddsd, 0 , NULL);
	LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
	FRM->FRM->Unlock(NULL);

	FRM->x = x;
	FRM->y = y;

	DDSetColorKey(FRM->FRM, RGB(0,0,0));
	
	_close(stream);

	return DD_OK;
}


int TFRMSingle::LoadFRM(int stream, LPVOID LoadIn, int x, int y, int p)
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

// --- TIanMap ---
//
// ---------------

HRESULT TIanMap::LoadMap(HWND hWnd,char* filename)
{
	int x,y;
	int stream;

	if ((stream = _open(filename,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadMap FAILED");

	for (x=0; x<100; x++)
		for (y=0; y<100; y++)
		{
			_read(stream,&Map[x][y],1);
		}
	_close(stream);
	return DD_OK;
}

HRESULT TIanMap::LoadTiles(HWND hWnd, LPDIRECTDRAW7 g_pDD)
{
	int x;
	HRESULT hRet;
	FILE* stre;
	int numb = 0;
	char buf[50];
	
	hRet= DD_OK;
	stre = fopen("master.dat\\proto\\tiles.pro","r");
	if (stre == NULL)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadMap FAILED");

	fscanf(stre,"%i",&numb);
		
	for (x=0; x<10; x++)
	{
		if (x<numb) {
		fscanf(stre,"%s",buf);
		} else {
		wsprintf(buf,"master.dat\\art\\tiles\\brda000.frm");
		}
		
		if (Tiles[x]) delete Tiles[x];
		Tiles[x] = new TFRMSingle();

		if (Tiles[x]==NULL) return InitFail(hWnd,hRet,"ERROR");

		hRet = Tiles[x]->Load(hWnd,g_pDD,buf,1);
		if (hRet!=DD_OK) return InitFail(hWnd,hRet,"ERROR");
		
	}

	fclose(stre);
	return DD_OK;
}

TIanMap::~TIanMap()
{
	int x;
	for (x=0; x<10; x++)
	{
		if (Tiles[x])
		{
			delete Tiles[x];
		}
	}
}


// --- TIanWall ---
//
// ----------------

HRESULT TIanWall::LoadMap(HWND hWnd,char* filename)
{
	int x,y;
	int stream;

	if ((stream = _open(filename,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadWall FAILED");

	for (x=0; x<171; x++)
		for (y=0; y<151; y++)
		{
			_read(stream,&Map[x][y],1);
		}
	_close(stream);
	return DD_OK;
}

HRESULT TIanWall::LoadWalls(HWND hWnd, LPDIRECTDRAW7 g_pDD)
{
	int x;
	HRESULT hRet;
	FILE* stre;
	int numb = 0;
	char buf[50];
	
	hRet= DD_OK;
	stre = fopen("master.dat\\proto\\walls.pro","r");
	if (stre == NULL)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadMap FAILED");

	wsprintf(buf,"d");
	fscanf(stre,"%i",&numb);
	
	for (x=0; x<50; x++)
	{
		if (x<numb) {
		fscanf(stre,"%s",buf);
		} else {
		wsprintf(buf,"master.dat\\art\\tiles\\brda000.frm");
		}

		//if (Tiles[x]) delete Tiles[x];
		//MessageBox(hWnd, buf, "IanOut: A Post Nuclear Ian Playing Game", MB_OK);
		Tiles[x] = new TFRMSingle();

		if (Tiles[x]==NULL) return InitFail(hWnd,hRet,"ERROR");

		

		hRet = Tiles[x]->Load(hWnd,g_pDD,buf,1);
		if (hRet!=DD_OK) return InitFail(hWnd,hRet,"ERROR");
		
	}

	fclose(stre);
	return DD_OK;
}

TIanWall::~TIanWall()
{
	int x;
	for (x=0; x<50; x++)
	{
		if (Tiles[x])
		{
			delete Tiles[x];
		}
	}
}

// --- TIanItems ---
//
// -----------------

HRESULT TIanItems::LoadMap(HWND hWnd,char* filename)
{
	int x,y;
	int stream;

	if ((stream = _open(filename,_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadItems FAILED");

	for (x=0; x<171; x++)
		for (y=0; y<151; y++)
		{
			_read(stream,&Map[x][y],1);
		}
	_close(stream);
	return DD_OK;
}

HRESULT TIanItems::LoadItems(HWND hWnd, LPDIRECTDRAW7 g_pDD)
{
	int x;
	HRESULT hRet;
	FILE* stre;
	int numb = 0;
	char buf[50];
	
	hRet= DD_OK;
	stre = fopen("master.dat\\proto\\items.pro","r");
	if (stre == NULL)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadMap FAILED");

	wsprintf(buf,"d");
	fscanf(stre,"%i",&numb);
	
	for (x=0; x<50; x++)
	{
		if (x<numb) {
		 fscanf(stre,"%s",buf);
		} else {
		 wsprintf(buf,"master.dat\\art\\tiles\\brda000.frm");
		}

		//if (Tiles[x]) delete Tiles[x];
		//MessageBox(hWnd, buf, "IanOut: A Post Nuclear Ian Playing Game", MB_OK);
		Tiles[x] = new TFRMSingle();

		if (Tiles[x]==NULL) return InitFail(hWnd,hRet,"ERROR");

		

		hRet = Tiles[x]->Load(hWnd,g_pDD,buf,1);
		if (hRet!=DD_OK) return InitFail(hWnd,hRet,"ERROR");
		
	}

	fclose(stre);
	return DD_OK;
}

TIanItems::~TIanItems()
{
	int x;
	for (x=0; x<50; x++)
	{
		if (Tiles[x])
		{
			delete Tiles[x];
		}
	}
}
