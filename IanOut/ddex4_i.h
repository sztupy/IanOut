
//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------

HRESULT AddAnim(HWND hWnd, char filename[10])
{
	PFRMAnim6 TheAnim;
	HRESULT hRet;
	char buf[100];
	TheAnim = new TFRMAnim6();
	wsprintf(buf,"master.dat\\art\\critter\\%s.frm",filename);
	hRet = TheAnim->Load(hWnd,g_pDD,buf);
	if (hRet != DD_OK) return hRet;
	PlyAnim->Insert(TheAnim);
	return hRet;
}

long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_ACTIVATE:
		// Pause if minimized
		g_bActive = !((BOOL)HIWORD(wParam));
		return 0L;
		
	case WM_DESTROY:
		// Clean up and close the app
		ReleaseAllObjects();
		PostQuitMessage(0);
		return 0L;
		
		
	case WM_KEYDOWN:
		// Handle any non-accelerated key commands
		switch (wParam)
		{
		case VK_PAUSE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0L;
		case VK_UP:
			if ((Selected>1) && (Selected<=7)) Selected--;
			if ((Selected>8) && (Selected<=25)) Selected--;
			if ((Selected>26) && (Selected<=41)) Selected--;
			if ((Selected>42) && (Selected<=59)) Selected--;
			break;
		case VK_DOWN:
			if ((Selected>=1) && (Selected<7)) Selected++;
			if ((Selected>=8) && (Selected<25)) Selected++;
			if ((Selected>=26) && (Selected<41)) Selected++;
			if ((Selected>=42) && (Selected<59)) Selected++;
			break;
		case 'N':
			if (GamePos==1) { Ian->dir++; if (Ian->dir>5) Ian->dir=0; } 
			break;
		case 'M':
			if (GamePos==1) { Ian->dir--; if (Ian->dir<0) Ian->dir=5; } 
			break;
		case VK_ADD:
			if (gamma>=1) gamma+=0.1;
			else gamma+=0.05;	
			if (gamma>4) gamma = 4;
			SetPaletteBl(255);
			break;
		case VK_SUBTRACT:
			if (gamma>=1) gamma-=0.1;
			else gamma-=0.05;	
			if (gamma<0.2) gamma = 0.2;
			SetPaletteBl(255);
			break;
		case VK_MULTIPLY:
			gameSpeed-=5;
			if (gameSpeed<0) gameSpeed=0;
			break;
		case VK_DIVIDE:
			gameSpeed+=5;
			if (gameSpeed>100) gameSpeed=100;
			break;
			
		}
		break;
		
        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return TRUE;
    }
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}


void CreateIanPalette()
{
    int                     i;
	FILE				   *stream;
    
	
	if( (stream = fopen( "master.dat\\pal.pal", "rb" )) != NULL )
    {
		for (i=0; i < 256; i++)
		{
			fread(&pe[i].peRed, 1, 1, stream);	  
			fread(&pe[i].peGreen, 1, 1, stream);	  
			fread(&pe[i].peBlue, 1, 1, stream);	  
		}
		
		fclose( stream );
    }
}

IDirectDrawPalette *  LoadSplashScreen(LPDIRECTDRAWSURFACE7 g_pDDSB)
{
	IDirectDrawPalette     *ddpal;
	PALETTEENTRY            ape[256];
	DDSURFACEDESC2			 ddsd;
	HRESULT hRet;
	
	int x,y;
	int r,g,b;
	int stream;
	char buf[80];
	
	x=(rand()%7);
	wsprintf(buf,"master.dat\\art\\splash\\splash%i.rix",x);
	//InitFail(hWnd,DD_OK,buf);
	stream = _open(buf,_O_BINARY);
	
	for (x=0; x<10; x++) _read(stream,&y,1);
	for (x=0; x<256; x++)
	{
		_read(stream,&r,1);
		_read(stream,&g,1);
		_read(stream,&b,1);
		ape[x].peRed = 0;
		ape[x].peGreen = 0;
		ape[x].peBlue = 0;
		pe[x].peRed = r*4;
		pe[x].peGreen = g*4;
		pe[x].peBlue = b*4;
	}
	ape[0].peRed=0;ape[0].peGreen=0;ape[0].peBlue=0;
	ape[1].peRed=255;ape[1].peGreen=255;ape[1].peBlue=255;
	
	pe[0].peRed=0;pe[0].peGreen=0;pe[0].peBlue=0;
	pe[1].peRed=255;pe[1].peGreen=255;pe[1].peBlue=255;
	
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	hRet = g_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 , ape, &ddpal, NULL);
	if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj2");
	hRet = g_pDDSPrimary->SetPalette(ddpal);
	if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj3");
	
	ddpal->SetEntries(0,0,256,ape);
	
	hRet = g_pDDSB->Lock(NULL,&ddsd,0,NULL);
	if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj");
	
	LPSTR Buffer;
	Buffer = (LPSTR)ddsd.lpSurface;
	
	for (y=0;y<(int)ddsd.dwHeight;y++)
	{
		_read(stream,Buffer+(y*ddsd.lPitch),ddsd.dwWidth);
	}
	g_pDDSB->Unlock(NULL);
	
	g_pDDSPrimary->Blt(NULL,g_pDDSBack,NULL,0,NULL);
	
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
	
	_close(stream);
	return ddpal;
}

static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS                    wc;
    DDSURFACEDESC2              ddsd;
    DDSCAPS2                    ddscaps;
    HRESULT                     hRet;
	char						buf[100];
	int							i,i2;
	PlayerDef					PLoad;
	PlayerDefAnim				PLoadAni;
	
	srand( (unsigned)time( NULL ) );
	
    // Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_WAIT);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NAME;
    wc.lpszClassName = NAME;
    RegisterClass(&wc);
	
    // Create a window
    hWnd = CreateWindowEx(WS_EX_TOPMOST,
		NAME,
		TITLE,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInstance,
		NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);
	
	InitDirectInput(hWnd, hInstance);
	SetAcquire();
	
	
    ///////////////////////////////////////////////////////////////////////////
    // Create the main DirectDraw object
    ///////////////////////////////////////////////////////////////////////////
    hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "DirectDrawCreateEx FAILED");
	
	
    // Get exclusive mode
    hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "SetCooperativeLevel FAILED");
	
    // Set the video mode to 640x480x8
    hRet = g_pDD->SetDisplayMode(640, 480, 8, 0, 0);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "SetDisplayMode FAILED");
	
    // Create the primary surface with 1 back buffer
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
		DDSCAPS_FLIP |
		DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "CreateSurface FAILED");
	
    // Get a pointer to the back buffer
    ZeroMemory(&ddscaps, sizeof(ddscaps));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");
	
	g_pDDPal=LoadSplashScreen(g_pDDSBack);
	FadeIn();
	
	MapInf = new TIanMap();
	
	MapInf->LoadMap(hWnd,"master.dat\\maps\\shady.map");
	MapInf->LoadTiles(hWnd,g_pDD);
	
	WallInf = new TIanWall();
	if (WallInf == NULL) return InitFail(hWnd,hRet,"Baj");
	
	WallInf->LoadMap(hWnd,"master.dat\\maps\\shady.wal");
	WallInf->LoadWalls(hWnd,g_pDD);
	
	ItemInf = new TIanItems();
	if (ItemInf == NULL) return InitFail(hWnd,hRet,"Baj");
	
	ItemInf->LoadMap(hWnd,"master.dat\\maps\\shady.ite");
	ItemInf->LoadItems(hWnd,g_pDD);
	
	
	//g_pDDSBack->SetClipper(g_pDClip);
	
	hRet = LoadFRMSingle(&Mouse, hWnd, "master.dat\\art\\interface\\stdarrow.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&Mouse2, hWnd, "master.dat\\art\\interface\\actarrow.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&KezeloP, hWnd, "master.dat\\art\\interface\\iface.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&FullScreen, hWnd, "master.dat\\art\\interface\\mainmenu.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&FullScreen2, hWnd, "master.dat\\art\\interface\\mainmenu.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&MenuUp, hWnd, "master.dat\\art\\interface\\menuup.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&MenuDown, hWnd, "master.dat\\art\\interface\\menudown.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&Select, hWnd, "master.dat\\art\\interface\\msef000.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Select2, hWnd, "master.dat\\art\\interface\\msef001.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&Buttons[0], hWnd, "master.dat\\art\\interface\\tgskloff.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[1], hWnd, "master.dat\\art\\interface\\tgsklon.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&SkillDex,hWnd, "master.dat\\art\\skilldex\\generic.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	for (i=0; i<8; i++)
	{
		wsprintf(buf,"master.dat\\art\\interface\\scr%i.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][0], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
		wsprintf(buf,"master.dat\\art\\interface\\scr%ix.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][1], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
	}
	
	PlyAnim = new TList();
	
	AddAnim(hWnd,"nmmaxxaa");
	PLoadAni.AIdle=(PFRMAnim6)PlyAnim->Last();
	AddAnim(hWnd,"nmmaxxab");
	PLoadAni.AMoving=(PFRMAnim6)PlyAnim->Last();
	
	PLoad.bIdle = TRUE;
	PLoad.bMove = TRUE;
	PLoad.bComputer = FALSE;
	PLoad.maxtaunt = 0;
	wsprintf(PLoad.name,"nmmaxx");
	Ian = new TFRMPlayer(PLoad);
	Ian->LoadPlayer(PLoadAni);
	if (hRet!=DD_OK) return InitFail(hWnd, hRet, "Ian FAILED");
	Ian->MoveTo(44,26);
	
	AddAnim(hWnd,"narobeaa");
	PLoadAni.AIdle=(PFRMAnim6)PlyAnim->Last();
	AddAnim(hWnd,"narobeab");
	PLoadAni.AMoving=(PFRMAnim6)PlyAnim->Last();
	
	PLoad.bIdle = TRUE;
	PLoad.bMove = TRUE;
	PLoad.bComputer = TRUE;
	PLoad.maxtaunt = 2;
	wsprintf(PLoad.taunt[0],"Aradesh vagyok");
	wsprintf(PLoad.taunt[1],"Ez egy felirat");
	wsprintf(PLoad.name,"narobe");
	Aradesh = new TFRMPlayer(PLoad);
	Aradesh->LoadPlayer(PLoadAni);
	if (hRet!=DD_OK) return InitFail(hWnd, hRet, "Aradesh FAILED");
	Aradesh->MoveTo(45,25);
	
	AddAnim(hWnd,"nfpeasaa");
	PLoadAni.AIdle=(PFRMAnim6)PlyAnim->Last();
	AddAnim(hWnd,"nfpeasab");
	PLoadAni.AMoving=(PFRMAnim6)PlyAnim->Last();
	
	for (i=0; i<10; i++)
	{
		PLoad.bIdle = TRUE;
		PLoad.bMove = TRUE;
		PLoad.bComputer = TRUE;
		PLoad.maxtaunt = 2;
		wsprintf(PLoad.taunt[0],"Shady Sands a kiráj");
		wsprintf(PLoad.taunt[1],"Paraszt vagyok");
		wsprintf(PLoad.name,"nfpeas");
		Paraszt[i] = new TFRMPlayer(PLoad);
		Paraszt[i]->LoadPlayer(PLoadAni);
		if (hRet!=DD_OK) return InitFail(hWnd, hRet, "Paraszt%i FAILED",i+1);
		i2 = 5+rand()%30;
		Paraszt[i]->MoveTo(50+(rand()%20*2)+i2%2,i2);
	}
	
	for (i=0 ; i<256; i++)
	{
		pe2[i].peBlue = 0;
		pe2[i].peRed = 0;
		pe2[i].peGreen = 0;
	}
	
	int x,y;
	int stream;
	
	if ((stream = _open("master.dat\\maps\\shady.blk",_O_BINARY)) == -1)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadBlock FAILED");
	
	for (x=0; x<171; x++)
		for (y=0; y<151; y++)
		{
			_read(stream,&BlockDat[x][y],1);
		}
		_close(stream);
		
		FadeOut();
		ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);
		CreateIanPalette();
		
		SetPaletteBl(255);
		
		g_pDDSOne = DDLoadBitmap(g_pDD, szBitmap, 0, 0, &hRet, DDSCAPS_OFFSCREENPLAIN);
		if (g_pDDSOne == NULL)
			return InitFail(hWnd, hRet, "DDLoadBitmap FAILED");
		DDSetColorKey(g_pDDSOne, CLR_INVALID);
		
		szovegnum=0;
		
		for (i=0; i<75; i++) LINIStr[i] = new TIniStr();
		
		Nullaz(&IanSkill);
		
		return DD_OK;
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG                         msg;
    
    if (InitApp(hInstance, nCmdShow) != DD_OK)
        return FALSE;
	
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive)
        {
            switch (GamePos)
			{
			case 0:
				MainMenu();
				break;
			case 1:
				UpdateFrame();
				break;
			case 2:
				Credits();
				break;
			case 3:
				Intro();
				break;
			case 4:
				CreateChar();
				break;
			case 5:
				Talk();
				break;
			}
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }
}

