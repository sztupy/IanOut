#include "windows.h"

#include "load.h"
#include "done.h"
#include "../commonutils/textutil.h"

#include "../commonutils/palette.h"
#include "../commonutils/mouse.h"

int LoadFRMSingle(PFRMSingle *LoadIn, char* filename, int i)
{
	AddToLog(5,"Load> Loading a new picture: %s",filename);
	int hRet;

	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(  filename, i)) != 0)
	   return InitFail(hRet,"%s FAILED",filename); else return 0;
}

int LoadLevel(const char* filename)
{
	AddToLog(2,"Load> Loading map file: %s", filename);
	int hRet;
	hRet = 0;

	if (MapInf) delete MapInf;
	if (StaticInf) delete StaticInf;

	MapInf = new TIanMap();
	if (MapInf == NULL) return InitFail(hRet,"LoadTilesMap Item Baj");

	MapInf->LoadMap(filename);
	MapInf->LoadTiles();
	
	int x,y;
	gzFile stream;
	
	if ((stream = __IOopen(filename,"rb")) == NULL)
		return InitFail(0,"LoadBlock FAILED");
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
	if (StaticInf == NULL) return InitFail(hRet,"LoadStaticMap Item Baj");

	StaticInf->LoadStatic(filename);
	if (CritterInf) delete CritterInf;
	CritterInf = new TIanCritter();
	if (CritterInf == NULL) return InitFail(hRet,"LoadCritter Baj");
	hRet = CritterInf->LoadCritters("\\maps\\blank.act",StaticInf->TilesI);
	if (hRet != 0) return InitFail(hRet,"LoadCritter Baj");

	return 0;
}

void CreateIanPalette()
{
int                     i;
	gzFile				   stream;
    
	
	if( (stream = __IOopen( "\\color.pal", "rb" )) != NULL )
    {
		for (i=0; i < 256; i++)
		{
			gzread(stream,&pe[i].r, 1);	  pe[i].r*=4;
			gzread(stream,&pe[i].g, 1);  pe[i].g*=4;	  
			gzread(stream,&pe[i].b, 1);	  pe[i].b*=4;
			if (i==0) { pe[i].r=0;pe[i].g=0;pe[i].b=0;}

		}
		
		gzclose( stream );
    }
	palette::Calculate16bit();
}

void LoadSplashScreen()
{
	SDL_Color				ape[256];
	int						hRet;
	
	int x,y;
	int r,g,b;
	gzFile stream;
	char buf[80];

	sprintf(buf,GetFile("\\color.pal").c_str());
	stream = __IOopen(buf,"rb");

	for (x=0; x<10; x++) gzread(stream,&y,1);
	for (x=0; x<256; x++)
	{
		gzread(stream,&r,1);
		gzread(stream,&g,1);
		gzread(stream,&b,1);
		ape[x].r = 0;
		ape[x].g = 0;
		ape[x].b = 0;
		pe[x].r = r*4;
		pe[x].g = g*4;
		pe[x].b = b*4;
	}
	ape[0].r=0;ape[0].g=0;ape[0].b=0;
	ape[1].r=255;ape[1].g=255;ape[1].b=255;
	
	pe[0].r=0;pe[0].g=0;pe[0].b=0;
	pe[1].r=255;pe[1].g=255;pe[1].b=255;

	palette::Calculate16bit();
 
	return;
}


int InitApplication()
{
    int						 hRet;
	char						buf[100];
	int							i;
	
	srand( (unsigned)time( NULL ) );

   	std::string MasterDat;

	for (i=0; i<75; i++) LINIStr[i] = new TIniStr();
	
	AddToLog(3,"Init> Loading Locations from IanOut.ini");

	SetFile(0,textutil::GetFromProf(".\\IanOut.ini","locations","ian_ed_loc"));
	for (i=0; i<10; i++) {
	   sprintf(buf,"location_%i",i);
	   MasterDat = textutil::GetFromProf(".\\IanOut.ini","locations",buf);
	   AddToLog(4,"Init> Initialising %i. location: %s ",i,MasterDat.c_str());
	   SetFile(i+1,MasterDat);
	}
	SetFile(11,textutil::GetFromProf(".\\IanOut.ini","locations","ianout_dat"));
	SetFile(12,textutil::GetFromProf(".\\IanOut.ini","locations","critter_dat"));
	SetFile(13,textutil::GetFromProf(".\\IanOut.ini","locations","master_dat"));
	SetFile(14,"");
	SetFile(15,"");

	AddToLog(2,"Init> Loading IanOut.ini informations");
	GetMaxX = GetPrivateProfileInt("IanOutEd_options","width",800,".\\IanOut.ini");
	GetMaxY = GetPrivateProfileInt("IanOutEd_options","height",600,".\\IanOut.ini");
	BitDepth = GetPrivateProfileInt("IanOutEd_options","bitdepth",16,".\\IanOut.ini");
	gamma = (double)GetPrivateProfileInt("IanOutEd_options","gamma",100,".\\IanOut.ini")/100;
	usefullscreen = GetPrivateProfileInt("IanOutEd_options","fullscreen",0,".\\IanOut.ini")!=0;

	AddToLog(3,"Init> Informations loaded: X:%i Y:%i Bitdepth:%i",GetMaxX,GetMaxY,BitDepth);
	AddToLog(3,"Init> Fullscreen mode: %i",usefullscreen ? 1 : 0);

	AddToLog(1,"Init> Initalising SDL libraries VIDEO and TIMER");
	SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER) ;

	AddToLog(3,"Init> Changing Window title and icon");
	SDL_WM_SetCaption(TITLE, NAME);
	SDL_WM_SetIcon(SDL_LoadBMP("IanOut.bmp"),NULL);

	AddToLog(1,"Init> Creating Primary Surface");
	g_pDDSBack = SDL_SetVideoMode ( GetMaxX, GetMaxY, BitDepth, usefullscreen ? SDL_FULLSCREEN | SDL_SWSURFACE : SDL_SWSURFACE | SDL_ANYFORMAT) ;
	SetClipRect(0,0,GetMaxX,GetMaxY);
	g_pDDPixelFormat = g_pDDSBack->format;
	SDL_SetClipRect(g_pDDSBack, NULL);

	if (!usefullscreen) { 
		BitDepth = g_pDDSBack->format->BitsPerPixel;
		AddToLog(2,"Init> Using current windows bitdepth : %i",BitDepth);
	}

	AddToLog(0,"Notice> The game is only tested under 16 and 32 bit displays,");
	AddToLog(0,"   so it may crash with other settings!");
	
	AddToLog(3,"Init> Disabling mouse cursor");
	SDL_ShowCursor(SDL_DISABLE);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

    if (usefullscreen) {
		AddToLog(2,"Init> Using exclusive mouse/keyboard input (use F11 to change it during game)");
		SDL_WM_GrabInput(SDL_GRAB_ON);
	} else {
		AddToLog(2,"Init> NOT using exclusive mouse/keyboard input (use F11 to change it during game)");
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}

	AddToLog(3,"Init> Calculating Gamma Ramp");
	SDL_GetGammaRamp(rramp,gramp,bramp);
	SDL_GetGammaRamp(rramp2,gramp2,bramp2);

	AddToLog(3,"Palette> Loading 8bit palette");
	LoadSplashScreen();
	palette::FadeIn();
	CreateIanPalette();

	AnimList = new TList();
	BoundRect.top = 0;
	BoundRect.left = 0;
	BoundRect.bottom = 511;
	BoundRect.right = 511;

	AddToLog(3,"Init> Loading Exit Grid pictures");
	AddToLog(0,"Notice> If the game crashes at this point you should check");
	AddToLog(0,"    the directory settings under IanOut.ini");
	LoadExitGrid();

	AddToLog(2,"Init> Initialising maploader");
	sprintf(buf,"\\maps\\blank.map");
	hRet = LoadLevel(buf);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Mouse Arrow");
	sprintf(buf,"\\art\\intrface\\stdarrow.frm");
	hRet = LoadFRMSingle(&Mouse,  buf,1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Main Menu");
	sprintf(buf,"\\art\\intrface\\mainmenu.frm");
	hRet = LoadFRMSingle(&FullScreen,  buf,1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Some Buttons");
	sprintf(buf,"\\art\\intrface\\menuup.frm");
	hRet = LoadFRMSingle(&MenuUp,  buf,1);
	if (hRet!=0) return hRet;

	sprintf(buf,"\\art\\intrface\\menudown.frm");
	hRet = LoadFRMSingle(&MenuDown,  buf,1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Grids");
	sprintf(buf,"\\art\\intrface\\msef000.frm");
	hRet = LoadFRMSingle(&Select,  buf,1);
	if (hRet!=0) return hRet;

	sprintf(buf,"\\art\\intrface\\msef001.frm");
	hRet = LoadFRMSingle(&Select2,  buf,1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Some More Buttons");
	sprintf(buf,"\\art\\intrface\\tgskloff.frm");
	hRet = LoadFRMSingle(&Buttons[0],  buf,1);
	if (hRet!=0) return hRet;

	sprintf(buf,"\\art\\intrface\\tgsklon.frm");
	hRet = LoadFRMSingle(&Buttons[1],  buf,1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Some Arrows");
	for (i=0; i<8; i++)
	{
		sprintf(buf,"\\art\\intrface\\scr%i.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][0],  buf,1);
		if (hRet!=0) return hRet;
		sprintf(buf,"\\art\\intrface\\scr%ix.frm",i+1);
		hRet = LoadFRMSingle(&MouseScr[i][1],  buf,1);
		if (hRet!=0) return hRet;
	}

	for (i=0 ; i<256; i++)
	{
		pe2[i].b = 0;
		pe2[i].r = 0;
		pe2[i].g = 0;
	}

	palette::SetPaletteBl(65535);

	AddToLog(2,"Init> Loading Font.bmp");
	SDL_Surface* g_pDDSOne2 = SDL_LoadBMP(GetFile("\\Font.bmp").c_str());
	g_pDDSOne = SDL_ConvertSurface(g_pDDSOne2, g_pDDPixelFormat, SDL_SWSURFACE);
	SDL_FreeSurface(g_pDDSOne2);
	SDL_SetColorKey(g_pDDSOne,SDL_SRCCOLORKEY,SDL_MapRGB(g_pDDPixelFormat,147,135,70));
	
	return 0;
}