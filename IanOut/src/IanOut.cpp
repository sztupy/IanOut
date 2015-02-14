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

void RunGame(bool type)
{
	switch (GamePos)
	{
	case 0:
		if (type) play::MainMenu();
		break;
	case 1:
		play::NormalPlay();
		break;
	case 2:
		if (type) play::Credits();
		break;
	case 3:
		if (type) play::Intro();
		break;
	case 4:
		if (type) play::CreateChar();
		break;
	case 5:
		play::Talk();
		break;
	case 6:
		play::Travel();
		break;
	}
}

int main( int argc, char* argv[] )
{
	ClearLogFile();
	AddToLog(0,"------------------------------------------------------------------------------");
	AddToLog(0,"     IanOut - A Post-Nuclear Ian-Playing Game     ");
	AddToLog(0,"     Version: "VERSIONNUM);
	AddToLog(0,"     Copyright (C) Kek eper neoszurrealista tarsulat - SztupY");
	AddToLog(0,"------------------------------------------------------------------------------");
	AddToLog(0,"");
	AddToLog(0,"Global> Output is saved to: log.txt");
	SetLogLevel(GetPrivateProfileInt("IanOut_options","loglevel",0,".\\IanOut.ini"));
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
				if (GamePos != 1) {
					MousX += dims.lX;
					MousY += dims.lY;
				}
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
				switch (event.key.keysym.sym) {
					case SDLK_F10: 
						{
							palette::SetGamma128();
						}
						break;
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
					case SDLK_ESCAPE: 
					case SDLK_BREAK: run = false; break;
					case SDLK_UP:
						if ((Selected>1) && (Selected<=7)) Selected--;
						if ((Selected>8) && (Selected<=25)) Selected--;
						if ((Selected>26) && (Selected<=41)) Selected--;
						if ((Selected>42) && (Selected<=59)) Selected--;
						break;
					case SDLK_DOWN:
						if ((Selected>=1) && (Selected<7)) Selected++;
						if ((Selected>=8) && (Selected<25)) Selected++;
						if ((Selected>=26) && (Selected<41)) Selected++;
						if ((Selected>=42) && (Selected<59)) Selected++;
						break;
					case 'q': date::AddMinute(15);break;
					case 'a': date::AddMinute(-15);break;
					case 'n':
						if (GamePos==1) { CritterInf->Critters.find(0)->second->dir++; if (CritterInf->Critters.find(0)->second->dir>5) CritterInf->Critters.find(0)->second->dir=0; } 
						break;
					case 'm':
						if (GamePos==1) { CritterInf->Critters.find(0)->second->dir--; if (CritterInf->Critters.find(0)->second->dir<0) CritterInf->Critters.find(0)->second->dir=5; } 
						break;
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
						gameSpeed-=5;
						if (gameSpeed<0) gameSpeed=0;
						break;
					case SDLK_KP_DIVIDE:
						gameSpeed+=5;
						if (gameSpeed>100) gameSpeed=100;
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
