
//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------

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
				case 'D': dot_plane = !dot_plane;break;
				case 'W': wall_plane = !wall_plane;break;
				case 'I': item_plane = !item_plane;break;
				case 'B': block_plane = !block_plane;break;
				case '2': show_wall = !show_wall;break;
				case '1': show_item = !show_item;break;
				case VK_SPACE: (putmode<4) ? putmode++ : putmode=1; break;
				case 'Z': Selected--;break;
				case 'X': Selected++;break;				
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
 
 int x;
 int r,g,b;
 int stream;
 char buf[80];


 wsprintf(buf,"master.dat\\pal.pal");
 stream = _open(buf,_O_BINARY);

// for (x=0; x<10; x++) _read(stream,&y,1);
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
	int							i;
	
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


	hRet = LoadFRMSingle(&Mouse, hWnd, "master.dat\\art\\interface\\stdarrow.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&FullScreen, hWnd, "master.dat\\art\\interface\\mainmenu.frm",1);
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

	for (i=0; i<8; i++)
	{
	wsprintf(buf,"master.dat\\art\\interface\\scr%i.frm",i+1);
	hRet = LoadFRMSingle(&MouseScr[i][0], hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;
	wsprintf(buf,"master.dat\\art\\interface\\scr%ix.frm",i+1);
	hRet = LoadFRMSingle(&MouseScr[i][1], hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;
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

	for (i=0; i<75; i++) LINIStr[i] = new TIniStr();

	ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);
	//InitFail(hWnd,DD_OK,"OK");
	CreateIanPalette();


	SetPaletteBl(255);
   	g_pDDSOne = DDLoadBitmap(g_pDD, szBitmap, 0, 0, &hRet, DDSCAPS_OFFSCREENPLAIN);
	
	if (g_pDDSOne == NULL)
        return InitFail(hWnd, hRet, "DDLoadBitmap FAILED");
	DDSetColorKey(g_pDDSOne, CLR_INVALID);

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
			}
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }
}

