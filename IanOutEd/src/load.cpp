#include "load.h"
#include "done.h"
#include "../commonutils/textutil.h"

#include "../commonutils/palette.h"
#include "../commonutils/mouse.h"

#include "../frmobject/freeimage.h"

extern long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i)
{
	HRESULT hRet;

	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(hWnd, g_pDD, filename, i)) != DD_OK)
	   return InitFail(hWnd,hRet,"%s FAILED",filename); else return DD_OK;
}

HRESULT LoadLevel(HWND hWnd, const char* filename)
{
	HRESULT hRet;
	hRet = DD_OK;

	if (MapInf) delete MapInf;
	if (StaticInf) delete StaticInf;

	MapInf = new TIanMap();
	if (MapInf == NULL) return InitFail(hWnd,hRet,"LoadTilesMap Item Baj");

	MapInf->LoadMap(hWnd,filename);
	MapInf->LoadTiles(hWnd,g_pDD);
	
	int x,y;
	gzFile stream;
	
	if ((stream = __IOopen(filename,"rb")) == NULL)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadBlock FAILED");
	gzseek( stream, 256*256*2, SEEK_SET );
	for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		{
			gzread(stream,&BlockDat[x][y],1);
		}
	gzclose(stream);

	/*for (x=0; x<512; x++)
		for (y=511; y>1; y--)
			if ((x%4)/2 != 0) BlockDat[x][y] = BlockDat[x][y-1];*/

	StaticInf = new TIanStatic();
	if (StaticInf == NULL) return InitFail(hWnd,hRet,"LoadStaticMap Item Baj");

	StaticInf->LoadStatic(hWnd,g_pDD,filename);
	if (CritterInf) delete CritterInf;
	CritterInf = new TIanCritter();
	if (CritterInf == NULL) return InitFail(hWnd,hRet,"LoadCritter Baj");
	hRet = CritterInf->LoadCritters(hWnd,g_pDD,"\\maps\\blank.act",StaticInf->TilesI);
	if (hRet != DD_OK) return InitFail(hWnd,hRet,"LoadCritter Baj");

	return DD_OK;
}

void CreateIanPalette()
{
    int                     i;
	gzFile					stream;
    
	
	if( (stream = __IOopen( "\\color.pal", "rb" )) != NULL )
    {
		for (i=0; i < 256; i++)
		{
			gzread(stream, &pe[i].peRed, 1);	  pe[i].peRed*=4;
			gzread(stream, &pe[i].peGreen, 1);  pe[i].peGreen*=4;	  
			gzread(stream, &pe[i].peBlue, 1);	  pe[i].peBlue*=4;
			if (i==0) { pe[i].peRed=0;pe[i].peGreen=0;pe[i].peBlue=0;}

		}
		gzclose( stream );
    }
	palette::Calculate16bit();
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


 wsprintf(buf,GetFile("\\color.pal").c_str());
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
 palette::Calculate16bit();
 //hRet = g_pDDSPrimary->SetPalette(ddpal);
 //if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj3");
 
 ddpal->SetEntries(0,0,256,ape);
 
 return ddpal;
}


HRESULT InitApp(HINSTANCE hInstance, int nCmdShow)
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

	FreeImage_Initialise(true);

	ClearLogFile();
	std::string MasterDat;

	for (i=0; i<75; i++) LINIStr[i] = new TIniStr();
	for (i=0; i<10; i++) {
	   wsprintf(buf,"location_%i",i);
	   MasterDat = textutil::GetFromProf(".\\IanOutEd.ini","locations",buf);
	   SetFile(i,MasterDat);
	   
	}
	
	GetMaxX = GetPrivateProfileInt("display","width",640,".\\IanOutEd.ini");
	GetMaxY = GetPrivateProfileInt("display","height",480,".\\IanOutEd.ini");
	BitDepth = GetPrivateProfileInt("display","bitdepth",16,".\\IanOutEd.ini");
	gamma = (double)GetPrivateProfileInt("display","gamma",100,".\\IanOutEd.ini")/100;
	gameSpeed = GetPrivateProfileInt("display","gamespeed",40,".\\IanOutEd.ini");

	
	mouse::InitDirectInput(hWnd, hInstance);
	mouse::SetAcquire();
 
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
	if (BitDepth == 32) {
		hRet = g_pDD->SetDisplayMode(GetMaxX, GetMaxY, 32, 0, 0);
	} else {
		hRet = g_pDD->SetDisplayMode(GetMaxX, GetMaxY, 16, 0, 0);
	}
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "SetDisplayMode FAILED");

	ScreenRect.left=0;
	ScreenRect.right=GetMaxX;
	ScreenRect.top=0;
	ScreenRect.bottom=GetMaxY;
	SetClipRect(0,0,GetMaxX,GetMaxY);

	   // Create the primary surface with 1 back buffer
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwBackBufferCount = 1;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "CreateSurface FAILED");

    // Get a pointer to the back buffer
    ZeroMemory(&ddscaps, sizeof(ddscaps));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_SYSTEMMEMORY;
    hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");

	g_pDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
	lpDDGammaControl->GetGammaRamp(0, &DDGammaOld);
	lpDDGammaControl->GetGammaRamp(0, &DDGammaRamp); 

	g_pDDPal=LoadSplashScreen(g_pDDSBack);
	CreateIanPalette();

	AnimList = new TList();
	BoundRect.top = 0;
	BoundRect.left = 0;
	BoundRect.bottom = 511;
	BoundRect.right = 511;

	LoadExitGrid(hWnd,g_pDD);

	wsprintf(buf,"\\maps\\blank.map");
	hRet = LoadLevel(hWnd,buf);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\stdarrow.frm");
	hRet = LoadFRMSingle(&Mouse, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\mainmenu.frm");
	hRet = LoadFRMSingle(&FullScreen, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\menuup.frm");
	hRet = LoadFRMSingle(&MenuUp, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\menudown.frm");
	hRet = LoadFRMSingle(&MenuDown, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\msef000.frm");
	hRet = LoadFRMSingle(&Select, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\msef001.frm");
	hRet = LoadFRMSingle(&Select2, hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\tgskloff.frm");
	hRet = LoadFRMSingle(&Buttons[0], hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	wsprintf(buf,"\\art\\intrface\\tgsklon.frm");
	hRet = LoadFRMSingle(&Buttons[1], hWnd, buf,1);
	if (hRet!=DD_OK) return hRet;

	for (i=0; i<8; i++)
	{
		wsprintf(buf,"\\art\\intrface\\scr%i.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][0], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
		wsprintf(buf,"\\art\\intrface\\scr%ix.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][1], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
	}

	for (i=0 ; i<256; i++)
	{
		pe2[i].peBlue = 0;
		pe2[i].peRed = 0;
		pe2[i].peGreen = 0;
	}

	ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);

	palette::SetPaletteBl(65535);

 	g_pDDSOne = DDLoadBitmap(g_pDD, szBitmap, 0, 0, &hRet, DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY);
	
	if (g_pDDSOne == NULL)
        return InitFail(hWnd, hRet, "DDLoadBitmap FAILED");
	DDSetColorKey(g_pDDSOne, CLR_INVALID);

	AddToLog("13");

	return DD_OK;
}