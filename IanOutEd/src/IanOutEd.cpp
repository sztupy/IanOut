#include "windows.h"

#include "global.h"

#include "../commonutils/mouse.h"
#include "done.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "play.h"

#include "../commonutils/date.h"
#include "../commonutils/textutil.h"

#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

extern int oldGamePos;
extern bool middlePressed;


void RunGame(bool type)
{
	switch (GamePos) {
		case 0:
			play::MainMenu();
			break;
		case 1:
			play::EditTerep();
			break;
		case 2:
			play::ChooseTerep();
			break;
		case 3:
			play::FileLoadSave();
			break;
		case 4:
			play::EditObjects();
			break;
		case 5:
			play::EditInventory();
			break;
		case 6:
			play::EditCritters();
			break;
	}
}

int main( int argc, char* argv[] )
{
	ClearLogFile();
	AddToLog(0,"------------------------------------------------------------------------------");
	AddToLog(0,"     IanOutEd - A Post-Nuclear Ian-Playing Game - Editor     ");
	AddToLog(0,"     Version: "VERSIONNUM);
	AddToLog(0,"     Copyright (C) Kek eper neoszurrealista tarsulat - Sztupy Corp.");
	AddToLog(0,"------------------------------------------------------------------------------");
	AddToLog(0,"");
	AddToLog(0,"Global> Output is saved to: log.txt");
	SetLogLevel(GetPrivateProfileInt("IanOutEd_options","loglevel",0,".\\IanOut.ini"));
	AddToLog(1,"Init> Initalizing Application");
	InitApplication();
	AddToLog(1,"Init> Initalizing Completed");
	SDL_Event event ;
	bool run = true;
	while (run)
	{
		olddims=dims;
		while ( SDL_PollEvent ( &event ) )
		{
			if ( event.type == SDL_QUIT ) run = false;
			if ( event.type == SDL_ACTIVEEVENT ) {
				AddToLog(5,"Event> Focus changed %i",event.active.gain);
				g_bActive = (event.active.gain==1);
			}
			if ( event.type == SDL_MOUSEMOTION ) {
				dims.lX = event.motion.xrel;
				dims.lY = event.motion.yrel;
				dims.x = event.motion.x;
				dims.y = event.motion.y;
				MousX += dims.lX;
				MousY += dims.lY;
				//AddToLog(5,"Mouse> X:%i Y:%i",MousX,MousY);
			}
			if ( event.type == SDL_MOUSEBUTTONDOWN ) {
				if ( event.button.button == SDL_BUTTON_LEFT) dims.buttons[0] = 1;
				if ( event.button.button == SDL_BUTTON_MIDDLE) dims.buttons[2] = 1;
				if ( event.button.button == SDL_BUTTON_RIGHT) dims.buttons[1] = 1;
			}
			if ( event.type == SDL_MOUSEBUTTONUP ) {
				if ( event.button.button == SDL_BUTTON_LEFT) dims.buttons[0] = 0;
				if ( event.button.button == SDL_BUTTON_MIDDLE) dims.buttons[2] = 0;
				if ( event.button.button == SDL_BUTTON_RIGHT) dims.buttons[1] = 0;
			}
			if ( event.type == SDL_KEYDOWN )
			{
				if (ref_str)
				{
					if (event.key.keysym.sym!=SDLK_BACKSPACE) *ref_str+=event.key.keysym.sym;
					if ((event.key.keysym.sym==SDLK_BACKSPACE) && ((*ref_str).length()!=0))(*ref_str).erase((*ref_str).length()-1,1);
				} 
				if (ref_to) {
				switch (event.key.keysym.sym) {
					case '1': if (ref_to) { *ref_to*=10; *ref_to+=1; };break;
					case '2': if (ref_to) { *ref_to*=10; *ref_to+=2; };break;
					case '3': if (ref_to) { *ref_to*=10; *ref_to+=3; };break;
					case '4': if (ref_to) { *ref_to*=10; *ref_to+=4; };break;
					case '5': if (ref_to) { *ref_to*=10; *ref_to+=5; };break;
					case '6': if (ref_to) { *ref_to*=10; *ref_to+=6; };break;
					case '7': if (ref_to) { *ref_to*=10; *ref_to+=7; };break;
					case '8': if (ref_to) { *ref_to*=10; *ref_to+=8; };break;
					case '9': if (ref_to) { *ref_to*=10; *ref_to+=9; };break;
					case '0': if (ref_to) { *ref_to*=10; };break;
					case '-': if (ref_to) { *ref_to=-*ref_to; };break;
				}
				} else {
					switch (event.key.keysym.sym) {
					case '1': putmode=1;break;
					case '2': putmode=2;break;
					case '3': putmode=3;break;
					case '4': putmode=4;break;
					case '5': putmode=5;break;
					case '6': putmode=6;break;
					case '7': putmode=7;break;
					case '8': putmode=8;break;
					case '9': putmode=9;break;
					case '0': putmode=0;break;
				}
				}
				switch (event.key.keysym.sym) {
					case SDLK_F11: 
						{
							int cgrab = SDL_WM_GrabInput(SDL_GRAB_QUERY);
							if (cgrab == SDL_GRAB_ON) {
								SDL_WM_GrabInput(SDL_GRAB_OFF);
								AddToLog(2,"Input> Exclusive mouse/keybord input disabled");
							} else {
								SDL_WM_GrabInput(SDL_GRAB_ON);
								AddToLog(2,"Input> Exclusive mouse/keybord input enabled");
							}
						}
						break;

					case SDLK_DELETE: if (ref_str){ (*ref_str) = ""; }; if (ref_to) { *ref_to = 0; };break;
					case SDLK_BACKSPACE: if (ref_to) { *ref_to/=10; };break;
					case SDLK_SPACE: (putmode<9) ? putmode++ : putmode=0; break;
					case SDLK_HOME: Selected=0; break;
					case SDLK_UP: if (Selected>0) Selected--; break;
					case SDLK_DOWN: Selected++; break;
					case SDLK_PAGEUP: Selected-=30; if (Selected<0) Selected=0;break;
					case SDLK_PAGEDOWN: Selected+=30;break;
					case SDLK_ESCAPE: middlePressed=true;break;
					case SDLK_BREAK: run = false; break;
					case SDLK_F12: {
						int x=0;
						char buf[80];
						sprintf(buf,"shot%i.bmp",x);
						while (CanOpen(buf)) {
							x++;
							sprintf(buf,"shot%i.bmp",x);
						}
						SDL_SaveBMP(g_pDDSBack,buf);
						std::string xXx;
						xXx = "Screenshot taken: ";
						xXx += buf;
						textutil::AddString(xXx,1);
						break;
					}
					case SDLK_KP_PLUS:
						if (gamma>=1) gamma+=0.1;
						else gamma+=0.05;	
						if (gamma>4) gamma = 4;
						palette::SetPaletteBl(65535);
						break;
					case SDLK_KP_MINUS:
						if (gamma>=1) gamma-=0.1;
						else gamma-=0.05;	
						if (gamma<0.2) gamma = 0.2;
						palette::SetPaletteBl(65535);
						break;
					case SDLK_KP_MULTIPLY:
						BrushSize++; if (BrushSize>9) BrushSize=9;break;
					case SDLK_KP_DIVIDE:
						BrushSize--; if (BrushSize<1) BrushSize=1;break;

				case 'd': if (GamePos==1) dot_plane = !dot_plane;break;
				case 'w': if (GamePos==1) wall_plane = !wall_plane;break;
				case 'i': if (GamePos==1) item_plane = !item_plane; else if (play::InventorySave) { oldGamePos=GamePos; GamePos=5; } break;
				case 'b': if (GamePos==1) block_plane = !block_plane;break;

				case 'z': case 'x': {
						FRMCollection::iterator iter;
						FRMCollection *point = NULL;

						FRMPairCollection::iterator iterP;
						FRMPairCollection *pointP = NULL;

						FRMDeadCollection::iterator iterD;
						FRMDeadCollection *pointD = NULL;
						
						switch (putmode)
						{
							case 0:point=&MapInf->Tiles;break;
							case 1:point=&StaticInf->TilesW;break;
							case 2:point=&StaticInf->TilesS;break;
							case 4:point=&MapInf->Tiles;break;
							case 5:pointP=&StaticInf->TilesI;break;
							case 6:pointP=&StaticInf->TilesO;break;
							case 8:pointD=&StaticInf->TilesD;break;
						}
						if (point) 
						{
 							if (event.key.keysym.sym=='z')
							{
							if (point->count(Selected) != 0)
							{
								iter = point->find(Selected);
								if (iter != point->begin()) {
									iter--;
									Selected = iter->first;
								}
							} else {
								if (point->lower_bound(Selected) != point->end()) {
								iter = point->lower_bound(Selected);
								if (iter != point->begin()) iter--;
								Selected = point->lower_bound(Selected)->first;
								} else
								{
								if (!point->empty()) {
								Selected = point->rbegin()->first;
								}
								}
							}
							}
							if (event.key.keysym.sym=='x')
							{
								if (point->count(Selected) != 0)
							{
								iter = point->find(Selected);
								iter++;
								if (iter != point->end()) {
									Selected = iter->first;
								}
							} else {
								if (point->upper_bound(Selected) != point->end()) {
								Selected = point->upper_bound(Selected)->first;
								}
							}

							}
						}

						if (pointP) 
						{
 							if (event.key.keysym.sym=='z')
							{
							if (pointP->count(Selected) != 0)
							{
								iterP = pointP->find(Selected);
								if (iterP != pointP->begin()) {
									iterP--;
									Selected = iterP->first;
								}
							} else {
								if (pointP->lower_bound(Selected) != pointP->end()) {
								iterP = pointP->lower_bound(Selected);
								if (iterP != pointP->begin()) iterP--;
								Selected = pointP->lower_bound(Selected)->first;
								} else
								{
								if (!pointP->empty()) {
								Selected = pointP->rbegin()->first;
								}
								}
							}
							}
							if (event.key.keysym.sym=='x')
							{
								if (pointP->count(Selected) != 0)
							{
								iterP = pointP->find(Selected);
								iterP++;
								if (iterP != pointP->end()) {
									Selected = iterP->first;
								}
							} else {
								if (pointP->upper_bound(Selected) != pointP->end()) {
								Selected = pointP->upper_bound(Selected)->first;
								}
							}

							}
						}


						if (pointD) 
						{
 							if (event.key.keysym.sym=='z')
							{
							if (pointD->count(Selected) != 0)
							{
								iterD = pointD->find(Selected);
								if (iterD != pointD->begin()) {
									iterD--;
									Selected = iterD->first;
								}
							} else {
								if (pointD->lower_bound(Selected) != pointD->end()) {
								iterD = pointD->lower_bound(Selected);
								if (iterD != pointD->begin()) iterD--;
								Selected = pointD->lower_bound(Selected)->first;
								} else
								{
								if (!pointD->empty()) {
								Selected = pointD->rbegin()->first;
								}
								}
							}
							}
							if (event.key.keysym.sym=='x')
							{
								if (pointD->count(Selected) != 0)
							{
								iterD = pointD->find(Selected);
								iterD++;
								if (iterD != pointD->end()) {
									Selected = iterD->first;
								}
							} else {
								if (pointD->upper_bound(Selected) != pointD->end()) {
								Selected = pointD->upper_bound(Selected)->first;
								}
							}

							}
						}

						}
						break;

				}
			}
		}
		RunGame(true);
	}
	AddToLog(1,"Done> Releasing Objects");
	ReleaseAllObjects();
	AddToLog(1,"Done> Releasing Objects Done");
	AddToLog(0,"Global> Game Quit");
	AddToLog(0,"------------------------------------------------------------------------------");
	AddToLog(0,"");
	return 0;
}