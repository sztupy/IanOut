#include "IanOs.h"

#include "normit.h"

#include "load.h"
#include "done.h"
#include "../commonutils/date.h"

#include "../commonutils/textutil.h"
#include "../frmobject/ianskill.h"
#include "../commonutils/palette.h"
#include "../commonutils/mouse.h"

#include <io.h>
#include <fcntl.h> 

#include "time.h"

#include "../frmobject/freeimage.h"

#include "../tinyxml/tinyxml.h"

std::string TempDir;

HRESULT DeleteTempDir()
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	//DWORD dwAttrs; 
	//char szNewPath[MAX_PATH]; 
	//char szHome[MAX_PATH]; 

	bool fFinished = false; 

	hSearch = FindFirstFile((TempDir+"\\maps\\*.*").c_str(), &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		//AddToLog("type1");
		return DD_OK; 
	} 
	 
	while (!fFinished) 
	{ 
		DeleteFile((TempDir+"\\maps\\"+FileData.cFileName).c_str());
		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{  
				//AddToLog("type2");
				fFinished = true; 
			} 
			else 
			{
				//AddToLog("type3");
				return DD_OK;
			} 
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
		//AddToLog("type4");
		return DD_OK;
	} 
	//AddToLog("type5");
	return DD_OK;
}

extern long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i)
{
	HRESULT hRet;
    if (*LoadIn) delete (*LoadIn);
	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(hWnd, g_pDD, filename, i)) != DD_OK)
	   return InitFail(hWnd,hRet,"%s FAILED",filename); else return DD_OK;
}

HRESULT LoadFRMAnim(PFRMAnim *LoadIn, HWND hWnd, char* filename)
{
	HRESULT hRet;
	if (*LoadIn) delete (*LoadIn);
	*LoadIn = new TFRMAnim();
	if ((hRet = (*LoadIn)->Load(hWnd, g_pDD, filename,-1)) != DD_OK)
	   return InitFail(hWnd,hRet,"%s FAILED",filename); else return DD_OK;
}

void CreateIanPalette()
{
    int                     i;
	gzFile				   stream;
    
	
	if( (stream = __IOopen( "\\color.pal", "rb" )) != NULL )
    {
		for (i=0; i < 256; i++)
		{
			gzread(stream,&pe[i].peRed, 1);	  pe[i].peRed*=4;
			gzread(stream,&pe[i].peGreen, 1);  pe[i].peGreen*=4;	  
			gzread(stream,&pe[i].peBlue, 1);	  pe[i].peBlue*=4;
			if (i==0) { pe[i].peRed=0;pe[i].peGreen=0;pe[i].peBlue=0;}

		}
		
		gzclose( stream );
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
	gzFile stream;
	char buf[80];
	
	x=(rand()%7);
	wsprintf(buf,"\\art\\splash\\splash%i.rix",x);
	//InitFail(hWnd,DD_OK,buf);
	stream = __IOopen(buf,"rb");
	
	for (x=0; x<10; x++) gzread(stream,&y,1);
	for (x=0; x<256; x++)
	{
		gzread(stream,&r,1);
		gzread(stream,&g,1);
		gzread(stream,&b,1);
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

	palette::Calculate16bit();
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	hRet = g_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 , ape, &ddpal, NULL);
	if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj2");
	
	ddpal->SetEntries(0,0,256,ape);
	
	hRet = g_pDDSB->Lock(NULL,&ddsd,0,NULL);
	if (hRet != DD_OK) InitFail(hWnd,hRet,"Baj");
	
	LPSTR LoadIn;
	LoadIn = (LPSTR)ddsd.lpSurface;
	
	int p = ddsd.lPitch;
	x = 640;
	y = 480;
	LPSTR	PointTo,PointTo2,PointTo3;
	WORD*	PointToInt;
	DWORD*	PointToInt32;
	int		i,i2;

	PointTo3 = (LPSTR)malloc(x);
	      	
		for (i=(GetMaxY-480)/2;i<(GetMaxY-480)/2+480;i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x);
			PointTo = (LPSTR)LoadIn+(i*p)+(GetMaxX-640)/2;
			if (BitDepth == 32) {
				PointToInt32 = (DWORD*)PointTo;
			} else {
				PointToInt = (WORD*)PointTo;
			}
			for (i2=0;i2<x;i2++) {
				if (BitDepth == 32) {
					*PointToInt32 = (DWORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt32+=1;
				} else {
					*PointToInt = (WORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt+=1;
				}
				PointTo2+=1;
			}
		}

		free(PointTo3);





	/*for (y=(GetMaxY-480)/2;y<(GetMaxY-480)/2+480;y++)
	{
		if (GetMaxX>=640) gzread(stream,Buffer+(y*ddsd.lPitch)+(GetMaxX-640)/2,640);
	}*/
	g_pDDSB->Unlock(NULL);
	
	g_pDDSPrimary->Blt(NULL,g_pDDSBack,NULL,0,NULL);
	
	while (true)
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
	
	gzclose(stream);
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
        return false;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);
	
	ClearLogFile();
	
	std::string MasterDat;

	FreeImage_Initialise(true);

	for (i=0; i<75; i++) LINIStr[i] = new TIniStr();
	
	for (i=0; i<10; i++) {
	   wsprintf(buf,"location_%i",i);
	   MasterDat = textutil::GetFromProf(".\\IanOut.ini","locations",buf);
	   if (i==0) TempDir = MasterDat;
	   SetFile(i,MasterDat);
	   //AddToLog(MasterDat.c_str());
	}

	DeleteTempDir();

	GetMaxX = GetPrivateProfileInt("display","width",640,".\\IanOut.ini");
	GetMaxY = GetPrivateProfileInt("display","height",480,".\\IanOut.ini");
	BitDepth = GetPrivateProfileInt("display","bitdepth",16,".\\IanOut.ini");
	gamma = (double)GetPrivateProfileInt("display","gamma",100,".\\IanOut.ini")/100;
	gameSpeed = GetPrivateProfileInt("display","gamespeed",40,".\\IanOut.ini");


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
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER  | DDSCAPS_SYSTEMMEMORY;
    hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");

	  ZeroMemory(&ddsd, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd);
      ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;
      ddsd.dwWidth = GetMaxX;
      ddsd.dwHeight = GetMaxY;
	  ddsd.lPitch = GetMaxX;
  	  hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBack2, NULL);
      if (hRet != DD_OK) return InitFail(hWnd,hRet,"CreateSurface_2 FAILED");

	SetClipRect(0,0,GetMaxX,GetMaxY);

	g_pDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
	lpDDGammaControl->GetGammaRamp(0, &DDGammaOld);
	lpDDGammaControl->GetGammaRamp(0, &DDGammaRamp); 

	g_pDDPal=LoadSplashScreen(g_pDDSBack);
	palette::FadeIn();

	CreateIanPalette();
	palette::Calculate16bit();

	AnimList = new TList();

	MapLoader = new TMapLoader(&MapInf,&StaticInf,&CritterInf,&BlockDat);

	LoadExitGrid(hWnd,g_pDD);
	
	hRet = LoadFRMSingle(&Mouse, hWnd, "\\art\\intrface\\stdarrow.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&Mouse2, hWnd, "\\art\\intrface\\actarrow.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&KezeloP, hWnd, "\\art\\intrface\\iface.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&FullScreen, hWnd, "\\art\\intrface\\mainmenu.frm",1);
	if (hRet!=DD_OK) return hRet;

	DDSetColorKey(FullScreen->FRM->FRM, RGB(0,0,0));
	hRet = LoadFRMSingle(&FullScreen2, hWnd, "\\art\\intrface\\mainmenu.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&MenuUp, hWnd, "\\art\\intrface\\menuup.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&MenuDown, hWnd, "\\art\\intrface\\menudown.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&Select, hWnd, "\\art\\intrface\\msef000.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Select2, hWnd, "\\art\\intrface\\msef001.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMSingle(&AmmoBox, hWnd, "\\art\\intrface\\ammox.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[0][0], hWnd, "\\art\\intrface\\tgskloff.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[0][1], hWnd, "\\art\\intrface\\tgsklon.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[1][0], hWnd, "\\art\\intrface\\invmaup.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[1][1], hWnd, "\\art\\intrface\\invmadn.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[2][0], hWnd, "\\art\\intrface\\lilredup.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[2][1], hWnd, "\\art\\intrface\\lilreddn.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[3][0], hWnd, "\\art\\intrface\\invdnout.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[3][1], hWnd, "\\art\\intrface\\invdnin.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[4][0], hWnd, "\\art\\intrface\\invupout.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[4][1], hWnd, "\\art\\intrface\\invupin.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[5][0], hWnd, "\\art\\intrface\\invbutup.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[5][1], hWnd, "\\art\\intrface\\invbutdn.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[6][0], hWnd, "\\art\\intrface\\sattkbup.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[6][1], hWnd, "\\art\\intrface\\sattkbdn.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[7][0], hWnd, "\\art\\intrface\\bigredup.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[7][1], hWnd, "\\art\\intrface\\bigreddn.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[8][0], hWnd, "\\art\\intrface\\endcmbtu.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[8][1], hWnd, "\\art\\intrface\\endcmbtd.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[9][0], hWnd, "\\art\\intrface\\endturnu.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[9][1], hWnd, "\\art\\intrface\\endturnd.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[23][0], hWnd, "\\art\\intrface\\hlgrn.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[23][1], hWnd, "\\art\\intrface\\hlred.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&Buttons[24][0], hWnd, "\\art\\intrface\\crossuse.frm",1);
	if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Buttons[24][1], hWnd, "\\art\\intrface\\crsshair.frm",1);
	if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&SkillDex,hWnd, "\\art\\skilldex\\generic.frm",1);
	if (hRet!=DD_OK) return hRet;
	
	for (i=0; i<20; i++)
	{
		hRet = LoadFRMSingle(&Temporary[i],hWnd,"\\art\\intrface\\blank.frm",1);
		if (hRet!=DD_OK) return hRet;

		if (i<5) hRet = LoadFRMAnim(&TempAnim[i],hWnd,"\\art\\intrface\\blank.frm");
		if (hRet!=DD_OK) return hRet;
		
	}

	hRet = LoadFRMSingle(&Arrows[0], hWnd, "\\art\\intrface\\suparrow.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&Arrows[1], hWnd, "\\art\\intrface\\sdnarrow.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[0][0], hWnd, "\\art\\intrface\\cancelh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[0][1], hWnd, "\\art\\intrface\\canceln.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[1][0], hWnd, "\\art\\intrface\\droph.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[1][1], hWnd, "\\art\\intrface\\dropn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[2][0], hWnd, "\\art\\intrface\\invenh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[2][1], hWnd, "\\art\\intrface\\invenn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[3][0], hWnd, "\\art\\intrface\\lookh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[3][1], hWnd, "\\art\\intrface\\lookn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[4][0], hWnd, "\\art\\intrface\\pushh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[4][1], hWnd, "\\art\\intrface\\pushn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[5][0], hWnd, "\\art\\intrface\\rotateh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[5][1], hWnd, "\\art\\intrface\\rotaten.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[6][0], hWnd, "\\art\\intrface\\skillh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[6][1], hWnd, "\\art\\intrface\\skilln.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[7][0], hWnd, "\\art\\intrface\\talkh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[7][1], hWnd, "\\art\\intrface\\talkn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[8][0], hWnd, "\\art\\intrface\\unloadh.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[8][1], hWnd, "\\art\\intrface\\unloadn.frm",1);if (hRet!=DD_OK) return hRet;

	hRet = LoadFRMSingle(&MenuScr[9][0], hWnd, "\\art\\intrface\\usegeth.frm",1);if (hRet!=DD_OK) return hRet;
	hRet = LoadFRMSingle(&MenuScr[9][1], hWnd, "\\art\\intrface\\usegetn.frm",1);if (hRet!=DD_OK) return hRet;
	
	hRet = LoadFRMAnim(&WaitMouse, hWnd, "\\art\\intrface\\wait2.frm");if (hRet!=DD_OK) return hRet;

	for (i=0; i<8; i++)
	{
		wsprintf(buf,"\\art\\intrface\\scr%i.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][0], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
		wsprintf(buf,"\\art\\intrface\\scr%ix.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][1], hWnd, buf,1);
		if (hRet!=DD_OK) return hRet;
	}
	
	date::Year = 2165;
	date::Month = 5;
	date::Day = 25;
	date::Hour = 11;
	date::Minute = 25;
	date::Second = 23;
	
	//ianskill::Nullaz(&IanSkill);
			
	MaskBMP = LoadBitmap(hInstance,"MASK");
	if (MaskBMP == NULL) return InitFail(hWnd, hRet, "MaskBitmap FAILED");
	MaskBMP2 = LoadBitmap(hInstance,"MASK2");
	if (MaskBMP2 == NULL) return InitFail(hWnd, hRet, "MaskBitmap2 FAILED");

	for (i=0 ; i<256; i++)
	{
		pe2[i].peBlue = 0;
		pe2[i].peRed = 0;
		pe2[i].peGreen = 0;
	}
	
	MapLocations = new TLocationList();

	palette::FadeOut();
	ClearSurface(g_pDDSBack,0,0,0); ClearSurface(g_pDDSPrimary,0,0,0);
	
		
	palette::SetPaletteBl(65535);
		
	g_pDDSOne = DDLoadBitmap(g_pDD, szBitmap, 0, 0, &hRet, DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY);
	if (g_pDDSOne == NULL)
		return InitFail(hWnd, hRet, "DDLoadBitmap FAILED");
		DDSetColorKey(g_pDDSOne, CLR_INVALID);
		
//	szovegnum=0;
	
	palette::Calculate();

	TimerList = new TList();
	TauntList = new TList();

	buttons::ChangeButtonList(ButtonList,1);

	return DD_OK;
}

HRESULT InitializeMap()
{
	char buf[150],buf2[150],buf3[150];
	int i = 0; int i2 = 0;

	TiXmlDocument XMLD(GetFile("\\data\\city.dat").c_str());
	XMLD.LoadFile();
	const TiXmlElement* parent = XMLD.RootElement();
	TiXmlNode* parent2;
            
	TiXmlNode* child = NULL;
	while( child = parent->IterateChildren("area",child ) )
	{
		int i = atoi2(child->ToElement()->Attribute("id"));
		int x=25;
		int y=25;
		bool visible=false;
		int size=3;
		std::string longname = "";
		std::string shortname = "";
		if (parent2 = child->FirstChild("location"))
		{
			x = atoi2(parent2->ToElement()->Attribute("xpos"));
			y = atoi2(parent2->ToElement()->Attribute("ypos"));
			visible = atoi2(parent2->ToElement()->Attribute("visible"))!= 0;
			size = atoi2(parent2->ToElement()->Attribute("size"));
		}
		if (parent2 = child->FirstChild("name"))
		{
			if (parent2->ToElement()->Attribute("long")) longname = parent2->ToElement()->Attribute("long");
			if (parent2->ToElement()->Attribute("short")) shortname = parent2->ToElement()->Attribute("short");
		}
		MapLocations->Locations->Insert(new TLocation(
			i,x,y,size,visible,
			shortname,longname));
	}

	/*
	while (textutil::GetFromProf(GetFile("\\data\\city.dat"),buf,"longname") != "") {
		int x = GetPrivateProfileInt(buf,"xpos",25,GetFile("\\data\\city.dat").c_str());
		int y = GetPrivateProfileInt(buf,"ypos",25,GetFile("\\data\\city.dat").c_str());
		bool visible = GetPrivateProfileInt(buf,"visible",0,GetFile("\\data\\city.dat").c_str()) != 0;
		int size = GetPrivateProfileInt(buf,"size",25,GetFile("\\data\\city.dat").c_str());
        MapLocations->Locations->Insert(new TLocation(
			i,x,y,size,visible,
			textutil::GetFromProf(GetFile("\\data\\city.dat"),buf,"shortname"),
			textutil::GetFromProf(GetFile("\\data\\city.dat"),buf,"longname")));
		i++;
		sprintf(buf,"Area_%i",i);
	}
	*/
	TravelMapX = atoi2(textutil::GetFromXML(GetFile("\\data\\city.dat"),".x").c_str());
		//GetPrivateProfileInt("global","start_x",25,GetFile("\\data\\city.dat").c_str());
	TravelMapY = atoi2(textutil::GetFromXML(GetFile("\\data\\city.dat"),".y").c_str());
		//GetPrivateProfileInt("global","start_y",25,GetFile("\\data\\city.dat").c_str());

	i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_loc").c_str());
		//GetPrivateProfileInt("global","start_location",0,GetFile("\\data\\maps.dat").c_str());

    sprintf(buf2,"/location-id=%i/name.filename",i);
    sprintf(buf,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/name.actorinf",i);
	sprintf(buf3,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	sprintf(buf2,"/location-id=%i/boundaries.type",i);
	TerepBoundType = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str()) != 0;
	sprintf(buf2,"/location-id=%i/boundaries.y",i);
	TerepBound.top = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.x",i);
	TerepBound.left = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.height",i);
	TerepBound.bottom = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.width",i);
	TerepBound.right = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	MapLoader->LoadMap(hWnd,g_pDD,buf,buf3);

	int location = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_loc").c_str());
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_entr").c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.x",location,i2);
	i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.y",location,i2);
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	
	PFRMPlayer IanChr = CritterInf->Critters.find(0)->second;

	IanChr->curth = 0;
	IanChr->MoveTo(i,i2);

	TerX = -LocConvertX(IanChr->x,IanChr->y)*16+GetMaxX/2;
	TerY = -LocConvertY(IanChr->x,IanChr->y)*12+GetMaxY/2;

	sprintf(buf2,"/location-id=%i/onenter:",location);
    
	play::TurnBasedMode = false;play::NormalPlay();

	IanOs::RunStat(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));

	return DD_OK;
}

HRESULT LoadLocation(int locnum, int entrnum)
{
	char buf[150],buf2[150],buf3[150];
	int i = locnum; int i2 = entrnum;

	sprintf(buf2,"/location-id=%i/name.filename",i);
    sprintf(buf,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/name.actorinf",i);
	sprintf(buf3,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	
	sprintf(buf2,"/location-id=%i/boundaries.type",i);
	TerepBoundType = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str()) != 0;
	sprintf(buf2,"/location-id=%i/boundaries.y",i);
	TerepBound.top = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.x",i);
	TerepBound.left = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.height",i);
	TerepBound.bottom = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.width",i);
	TerepBound.right = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	MapLoader->LoadMap(hWnd,g_pDD,buf,buf3);

	//int location = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),"/start.loc").c_str());
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_entr").c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.x",locnum,entrnum);
	i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.y",locnum,entrnum);
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	
	PFRMPlayer IanChr = CritterInf->Critters.find(0)->second;

	IanChr->curth = 0;
	IanChr->MoveTo(i,i2);

	TerX = -LocConvertX(IanChr->x,IanChr->y)*16+GetMaxX/2;
	TerY = -LocConvertY(IanChr->x,IanChr->y)*12+GetMaxY/2;

	sprintf(buf2,"/location-id=%i/onenter:",locnum);
    
	play::TurnBasedMode = false;play::NormalPlay();

	IanOs::RunStat(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));

	return DD_OK;
}