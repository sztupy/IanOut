#include "windows.h"

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
#include "../tinyxml/tinyxml.h"

std::string TempDir;

int LoadMusic(std::string filename)
{
	if (usingsound) {
		Mix_HaltMusic();
		AddToLog(3,"Sound> Loading Music file %s",filename.c_str());
		MusicData = Mix_LoadMUS(GetMusicFile(filename).c_str());
		if (!MusicData) {
			return InitFail(0,"Mix_LoadMUS: %s", Mix_GetError());
		}
		AddToLog(3,"Sound> Playing Music");
		if (Mix_PlayMusic(MusicData, -1) == -1)
			return InitFail(0,"Mix_PlayMusic: %s", Mix_GetError());
		Mix_VolumeMusic(MIX_MAX_VOLUME);
	}	
	return 0;
}

int DeleteTempDir()
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 

	bool fFinished = false; 

	hSearch = FindFirstFile((TempDir+"\\maps\\*.*").c_str(), &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		//AddToLog("type1");
		return 0; 
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
				return 0;
			} 
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
		//AddToLog("type4");
		return 0;
	} 
	//AddToLog("type5");
	return 0;
}

//extern long FAR PASCAL WindowProc(HWND  UINT message, WPARAM wParam, LPARAM lParam);

int LoadFRMSingle(PFRMSingle *LoadIn, char* filename, int i)
{
	AddToLog(5,"Load> Loading a new picture: %s",filename);
	int hRet;
    if (*LoadIn) delete (*LoadIn);
	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(filename, i)) != 0)
	   return InitFail(hRet,"%s FAILED",filename); else return 0;
}

int LoadFRMAnim(PFRMAnim *LoadIn, char* filename)
{
	AddToLog(5,"Load> Loading a new animation: %s",filename);
	int hRet;
	if (*LoadIn) delete (*LoadIn);
	*LoadIn = new TFRMAnim();
	if ((hRet = (*LoadIn)->Load(filename,-1)) != 0)
	   return InitFail(hRet,"%s FAILED",filename); else return 0;
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
}

void LoadSplashScreen()
{

	char buf[80];
	int x;
	int amount = atoi(textutil::GetFromXML("\\art\\splash\\splash.dat",".amount").c_str());

	x=(rand()%(amount+1));
	sprintf(buf,"\\art\\splash\\splash%i.frm",x);
	PFRMSingle FRM = NULL;
	LoadFRMSingle(&FRM,buf,1);
	BlitTo(g_pDDSBack,0,0,FRM->FRM->x,FRM->FRM->y,GetMaxX/2-FRM->FRM->x/2,GetMaxY/2-FRM->FRM->y/2,0,FRM->FRM->FRM);
	
    SDL_Flip(g_pDDSBack);
	delete FRM;

	return;
}

int InitApplication()
{
    int		                    hRet;
	char						buf[100];
	int							i;
	
	srand( (unsigned)time( NULL ) );

   	std::string MasterDat;

	for (i=0; i<75; i++) LINIStr[i] = new TIniStr();
	
	AddToLog(3,"Init> Loading Locations from IanOut.ini");

	TempDir = textutil::GetFromProf(".\\IanOut.ini","locations","temporary");
	SetFile(0,TempDir);
	for (i=0; i<10; i++) {
	   sprintf(buf,"location_%i",i);
	   MasterDat = textutil::GetFromProf(".\\IanOut.ini","locations",buf);
	   AddToLog(4,"Init> Initialising %i. location: %s ",i,MasterDat.c_str());
	   SetFile(i+1,MasterDat);
	}
	SetFile(11,textutil::GetFromProf(".\\IanOut.ini","locations","ianout_dat"));
	AddToLog(0,textutil::GetFromProf(".\\IanOut.ini","locations","ianout_dat").c_str());
	SetFile(12,textutil::GetFromProf(".\\IanOut.ini","locations","critter_dat"));
	SetFile(13,textutil::GetFromProf(".\\IanOut.ini","locations","master_dat"));
	SetFile(14,textutil::GetFromProf(".\\IanOut.ini","locations","music"));
	SetFile(15,"");

	if (GetMusicFile("\\01hub") == "") {
		usingsound = false;
		AddToLog(1,"Init> 01Hub not found: Sound is disabled");
	} else {
		usingsound = true;
		AddToLog(1,"Init> 01Hub found: Sound is enabled");
	}

	AddToLog(3,"Init> Erasing Temporary directory %s",TempDir.c_str());
	DeleteTempDir();

	AddToLog(2,"Init> Loading IanOut.ini informations");
	GetMaxX = GetPrivateProfileInt("IanOut_options","width",640,".\\IanOut.ini");
	GetMaxY = GetPrivateProfileInt("IanOut_options","height",480,".\\IanOut.ini");
	BitDepth = GetPrivateProfileInt("IanOut_options","bitdepth",16,".\\IanOut.ini");
	gamma = (double)GetPrivateProfileInt("IanOut_options","gamma",100,".\\IanOut.ini")/100;
	gameSpeed = GetPrivateProfileInt("IanOut_options","gamespeed",40,".\\IanOut.ini");
	usefullscreen = GetPrivateProfileInt("IanOut_options","fullscreen",1,".\\IanOut.ini")!=0;

	graphoptions[0] = GetPrivateProfileInt("IanOut_display","alpharoof",0,".\\IanOut.ini")!=0;
	graphoptions[1] = GetPrivateProfileInt("IanOut_display","alpharoof",0,".\\IanOut.ini")==2;
	graphoptions[2] = GetPrivateProfileInt("IanOut_display","alphaobj",0,".\\IanOut.ini")!=0;
	graphoptions[3] = GetPrivateProfileInt("IanOut_display","alphaobj",0,".\\IanOut.ini")==2;

	AddToLog(3,"Init> Informations loaded: X:%i Y:%i Bitdepth:%i",GetMaxX,GetMaxY,BitDepth);
	AddToLog(3,"Init> Fullscreen mode: %i",usefullscreen ? 1 : 0);

	AddToLog(1,"Init> Initalising SDL libraries VIDEO and TIMER%s",usingsound ? " and AUDIO" : "");
	if (SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER | (usingsound ? SDL_INIT_AUDIO : 0)) ==-1) return InitFail(0,"Error during initalisation");

	AddToLog(3,"Init> Changing Window title and icon");
	SDL_WM_SetCaption(TITLE, NAME);
	SDL_WM_SetIcon(SDL_LoadBMP("IanOut.bmp"),NULL);

	AddToLog(1,"Init> Creating Primary Surface");
	g_pDDSBack = SDL_SetVideoMode ( GetMaxX, GetMaxY, BitDepth, usefullscreen ? SDL_FULLSCREEN | SDL_SWSURFACE : SDL_SWSURFACE | SDL_ANYFORMAT) ;
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

	AddToLog(3,"Init> Calculating Gamma Ramp");
	SDL_GetGammaRamp(rramp,gramp,bramp);
	SDL_GetGammaRamp(rramp2,gramp2,bramp2);

	if (usefullscreen) {
		AddToLog(2,"Init> Using exclusive mouse/keyboard input (use F11 to change it during game)");
		SDL_WM_GrabInput(SDL_GRAB_ON);
	} else {
		AddToLog(2,"Init> NOT using exclusive mouse/keyboard input (use F11 to change it during game)");
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}

	AddToLog(3,"Init> Creating a temporary surface");
	g_pDDSBack2 = SDL_CreateRGBSurface(SDL_SWSURFACE,GetMaxX,GetMaxY,BitDepth,g_pDDPixelFormat->Rmask,g_pDDPixelFormat->Gmask,g_pDDPixelFormat->Bmask,g_pDDPixelFormat->Amask);

	SetClipRect(0,0,GetMaxX,GetMaxY);

	AddToLog(3,"Palette> Loading 8bit palette");
	CreateIanPalette();
	palette::Calculate16bit();

	palette::SetPaletteBl(0);

	AddToLog(1,"Init> Loading Splash Screen");
	LoadSplashScreen();
	palette::FadeIn();

	if (usingsound) {
		AddToLog(1,"Init> Initialising SDL_MIXER");
		if (Mix_OpenAudio(22050,AUDIO_S16SYS,2,4096)==-1) return InitFail(0,"Error during initalisation");
	}

	AddToLog(2,"Init> Initialising maploader");
	AnimList = new TList();
	MapLoader = new TMapLoader(&MapInf,&StaticInf,&CritterInf,&BlockDat);
	AddToLog(3,"Init> Loading Exit Grid pictures");
	AddToLog(0,"Notice> If the game crashes at this point you should check");
	AddToLog(0,"    the directory settings under IanOut.ini");
	LoadExitGrid();

	AddToLog(3,"Init> Loading Mouse Arrow");
	hRet = LoadFRMSingle(&Mouse,  "\\art\\intrface\\stdarrow.frm",1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Mouse Arrow2");
	hRet = LoadFRMSingle(&Mouse2,  "\\art\\intrface\\actarrow.frm",1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Interface");
	hRet = LoadFRMSingle(&KezeloP,  "\\art\\intrface\\iface.frm",1);
	if (hRet!=0) return hRet;
	
	AddToLog(3,"Init> Loading Main Menu");
	hRet = LoadFRMSingle(&FullScreen,  "\\art\\intrface\\mainmenu.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&FullScreen2,  "\\art\\intrface\\mainmenu.frm",1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Some Buttons");
	hRet = LoadFRMSingle(&MenuUp,  "\\art\\intrface\\menuup.frm",1);
	if (hRet!=0) return hRet;
	
	hRet = LoadFRMSingle(&MenuDown,  "\\art\\intrface\\menudown.frm",1);
	if (hRet!=0) return hRet;
	
	AddToLog(3,"Init> Loading Grids");
	hRet = LoadFRMSingle(&Select,  "\\art\\intrface\\msef000.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Select2,  "\\art\\intrface\\msef001.frm",1);
	if (hRet!=0) return hRet;
	
	AddToLog(3,"Init> Loading AmmoBar");
	hRet = LoadFRMSingle(&AmmoBox,  "\\art\\intrface\\ammox.frm",1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Some More Buttons");
	hRet = LoadFRMSingle(&Buttons[0][0],  "\\art\\intrface\\tgskloff.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[0][1],  "\\art\\intrface\\tgsklon.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[1][0],  "\\art\\intrface\\invmaup.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[1][1],  "\\art\\intrface\\invmadn.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[2][0],  "\\art\\intrface\\lilredup.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[2][1],  "\\art\\intrface\\lilreddn.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[3][0],  "\\art\\intrface\\invdnout.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[3][1],  "\\art\\intrface\\invdnin.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[4][0],  "\\art\\intrface\\invupout.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[4][1],  "\\art\\intrface\\invupin.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[5][0],  "\\art\\intrface\\invbutup.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[5][1],  "\\art\\intrface\\invbutdn.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[6][0],  "\\art\\intrface\\sattkbup.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[6][1],  "\\art\\intrface\\sattkbdn.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[7][0],  "\\art\\intrface\\bigredup.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[7][1],  "\\art\\intrface\\bigreddn.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[8][0],  "\\art\\intrface\\endcmbtu.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[8][1],  "\\art\\intrface\\endcmbtd.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[9][0],  "\\art\\intrface\\endturnu.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[9][1],  "\\art\\intrface\\endturnd.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[23][0],  "\\art\\intrface\\hlgrn.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[23][1],  "\\art\\intrface\\hlred.frm",1);
	if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&Buttons[24][0],  "\\art\\intrface\\crossuse.frm",1);
	if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Buttons[24][1],  "\\art\\intrface\\crsshair.frm",1);
	if (hRet!=0) return hRet;

	AddToLog(3,"Init> Loading Generic SkillDex Pic");
	hRet = LoadFRMSingle(&SkillDex, "\\art\\skilldex\\generic.frm",1);
	if (hRet!=0) return hRet;
	
	AddToLog(3,"Init> Initialising Temporary Surfaces");
	for (i=0; i<20; i++)
	{
		hRet = LoadFRMSingle(&Temporary[i],"\\art\\intrface\\blank.frm",1);
		if (hRet!=0) return hRet;

		if (i<5) hRet = LoadFRMAnim(&TempAnim[i],"\\art\\intrface\\blank.frm");
		if (hRet!=0) return hRet;
		
	}

	AddToLog(3,"Init> Loading Actionmenu Pictures");
	hRet = LoadFRMSingle(&Arrows[0],  "\\art\\intrface\\suparrow.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&Arrows[1],  "\\art\\intrface\\sdnarrow.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[0][0],  "\\art\\intrface\\cancelh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[0][1],  "\\art\\intrface\\canceln.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[1][0],  "\\art\\intrface\\droph.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[1][1],  "\\art\\intrface\\dropn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[2][0],  "\\art\\intrface\\invenh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[2][1],  "\\art\\intrface\\invenn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[3][0],  "\\art\\intrface\\lookh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[3][1],  "\\art\\intrface\\lookn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[4][0],  "\\art\\intrface\\pushh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[4][1],  "\\art\\intrface\\pushn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[5][0],  "\\art\\intrface\\rotateh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[5][1],  "\\art\\intrface\\rotaten.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[6][0],  "\\art\\intrface\\skillh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[6][1],  "\\art\\intrface\\skilln.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[7][0],  "\\art\\intrface\\talkh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[7][1],  "\\art\\intrface\\talkn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[8][0],  "\\art\\intrface\\unloadh.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[8][1],  "\\art\\intrface\\unloadn.frm",1);if (hRet!=0) return hRet;

	hRet = LoadFRMSingle(&MenuScr[9][0],  "\\art\\intrface\\usegeth.frm",1);if (hRet!=0) return hRet;
	hRet = LoadFRMSingle(&MenuScr[9][1],  "\\art\\intrface\\usegetn.frm",1);if (hRet!=0) return hRet;
	
	AddToLog(3,"Init> Loading Wait Cursor");
	hRet = LoadFRMAnim(&WaitMouse,  "\\art\\intrface\\wait2.frm");if (hRet!=0) return hRet;

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
	
	date::Year = 2165;
	date::Month = 5;
	date::Day = 25;
	date::Hour = 11;
	date::Minute = 25;
	date::Second = 23;
	
	//ianskill::Nullaz(&IanSkill);
			
/*	MaskBMP = LoadBitmap(hInstance,"MASK");
	if (MaskBMP == NULL) return InitFail( hRet, "MaskBitmap FAILED");
	MaskBMP2 = LoadBitmap(hInstance,"MASK2");
	if (MaskBMP2 == NULL) return InitFail( hRet, "MaskBitmap2 FAILED");*/

	for (i=0 ; i<256; i++)
	{
		pe2[i].b = 0;
		pe2[i].r = 0;
		pe2[i].g = 0;
	}

	AddToLog(3,"Init> Initalising LocationList");

	MapLocations = new TLocationList();

	palette::FadeOut();
	ClearSurface(g_pDDSBack,0,0,0);// ClearSurface(g_pDDSPrimary,0,0,0);
		
	//palette::SetPaletteBl(65535);

	AddToLog(2,"Init> Loading Font.bmp");
	SDL_Surface* g_pDDSOne2 = SDL_LoadBMP(GetFile("\\Font.bmp").c_str());
	g_pDDSOne = SDL_ConvertSurface(g_pDDSOne2, g_pDDPixelFormat, SDL_SWSURFACE);
	SDL_FreeSurface(g_pDDSOne2);
	SDL_SetColorKey(g_pDDSOne,SDL_SRCCOLORKEY,SDL_MapRGB(g_pDDPixelFormat,147,135,70));
		
	palette::Calculate();

	AddToLog(3,"Init> Initialising Timer list");
	TimerList = new TList();
	TauntList = new TList();

	buttons::ChangeButtonList(ButtonList,1);

	LoadMusic("\\07desert");

	return 0;
}

int InitializeMap()
{
	AddToLog(2,"Load> Initialising Map (begin)");
	LoadMusic("\\wind2");
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

	TravelMapX = atoi2(textutil::GetFromXML(GetFile("\\data\\city.dat"),".x").c_str());
	TravelMapY = atoi2(textutil::GetFromXML(GetFile("\\data\\city.dat"),".y").c_str());
	
	i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_loc").c_str());

	sprintf(buf2,"/location-id=%i/name.filename",i);
    sprintf(buf,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/name.actorinf",i);
	sprintf(buf3,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	sprintf(buf2,"/location-id=%i/boundaries.type",i);
	TerepBoundType = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str()) != 0;
	sprintf(buf2,"/location-id=%i/boundaries.y",i);
	TerepBound.y = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.x",i);
	TerepBound.x = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.height",i);
	TerepBound.h = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.width",i);
	TerepBound.w = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	MapLoader->LoadMap(buf,buf3);

	int location = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_loc").c_str());
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),".start_entr").c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.x",location,i2);
	i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/entrance-id=%i.y",location,i2);
	i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	LoadMusic("\\07desert");
	
	PFRMPlayer IanChr = CritterInf->Critters.find(0)->second;

	IanChr->curth = 0;
	IanChr->MoveTo(i,i2);

	TerX = -LocConvertX(IanChr->x,IanChr->y)*16+GetMaxX/2;
	TerY = -LocConvertY(IanChr->x,IanChr->y)*12+GetMaxY/2;

	sprintf(buf2,"/location-id=%i/onenter:",location);
    
	play::TurnBasedMode = false;play::NormalPlay();

	AddToLog(3,"Load> Executiong Init Script");

	IanOs::RunStat(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));

	sprintf(buf2,"/location-id=%i/sound.name",location);
	LoadMusic("\\"+textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));
	AddToLog(2,"Load> Initialising Map (end)");
	return 0;
}

int LoadLocation(int locnum, int entrnum)
{
	AddToLog(2,"Load> Loading Locations (begin)");
	LoadMusic("\\wind2");
	char buf[150],buf2[150],buf3[150];
	int i = locnum; int i2 = entrnum;

	sprintf(buf2,"/location-id=%i/name.filename",i);
    sprintf(buf,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/name.actorinf",i);
	sprintf(buf3,"\\maps\\%s",textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	
	sprintf(buf2,"/location-id=%i/boundaries.type",i);
	TerepBoundType = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str()) != 0;
	sprintf(buf2,"/location-id=%i/boundaries.y",i);
	TerepBound.y = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.x",i);
	TerepBound.x = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.height",i);
	TerepBound.h = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());
	sprintf(buf2,"/location-id=%i/boundaries.width",i);
	TerepBound.w = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2).c_str());

	MapLoader->LoadMap(buf,buf3);

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

	AddToLog(3,"Load> Executiong Init Script");
	IanOs::RunStat(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));

	sprintf(buf2,"/location-id=%i/sound.name",locnum);
	LoadMusic("\\"+textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf2));
	AddToLog(2,"Load> Loading Locations (end)");

	return 0;
}