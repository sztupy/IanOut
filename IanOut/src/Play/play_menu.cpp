#include "play.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"
#include "../commonutils/mouse.h"

SDL_Event event2;

void play::MainMenu(void)
{
    int                     hRet;
	
	static int					Frame = -25;
	static Uint32				ThisTick;
	static Uint32				LastTick = 0;

	int addx = (GetMaxX-640)/2;
	int addy = (GetMaxY-480)/2;
	
	if ((strcmp(FullScreen->fname,"\\art\\intrface\\mainmenu.frm")))
	{
		AddToLog(2,"Load> Loading Main Menu Artwork");
    	LoadFRMSingle(&FullScreen,"\\art\\intrface\\mainmenu.frm",1);
	}

	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3000;
			
        }

	if (MousX>=GetMaxX) { MousX=GetMaxX;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=GetMaxY) { MousY=GetMaxY;}
	if (MousY<=0)   { MousY=0;  }

	//ClearSurface(g_pDDSBack,0,0,0);
	BlitTo(g_pDDSBack,0,0,640,480,addx+0,addy+0,0,FullScreen->FRM->FRM);
	
	ButtonList->DrawButtons(g_pDDSBack,MousX,MousY,mouse::Pressed(6));
    
	textfont::IanOutTextC(addx+123,addy+25,2,"Intro");
	textfont::IanOutTextC(addx+123,addy+66,2,"New Game");
	textfont::IanOutTextC(addx+123,addy+107,2,"Load (x)");
	textfont::IanOutTextC(addx+123,addy+148,2,"Settings (x)");
	textfont::IanOutTextC(addx+123,addy+189,2,"Credits");
	textfont::IanOutTextC(addx+123,addy+230,2,"Exit");

	textutil::DisplayFrameRate();

	textfont::IanOutText(addx+0,addy+465,1,"IanOut - A Post-Nuclear Ian-Playing Game v"VERSIONNUM);
	textfont::IanOutTextR(addx+640,addy+465,4,"SztupY");
	
	if (Frame<65535) palette::SetPaletteBl(Frame);

	int value = ButtonList->GetButton(MousX,MousY,mouse::Pressed(6));

	if (value==1)
		{
			GamePos=3;
			MousX=150;
			Frame = -25;
			palette::FadeOut();
			return;
		}
	if (value==2) 
		{
			GamePos=4;
			Frame=-25;
			palette::FadeOut();
			ClearSurface(g_pDDSBack,0,0,0);
//			ClearSurface(g_pDDSPrimary,0,0,0);
			MapLoader->InitMainChar();
			play::CreateChar();
			palette::FadeIn();
			return;
		}
	if (value==5) 
		{
			GamePos=2;
			Frame=-25;
			palette::FadeOut();
			return;
		}
	if (value==6)
		{
			palette::FadeOut();
			ClearSurface(g_pDDSBack,0,0,0);
			event2.type = SDL_QUIT;
			event2.quit.type = SDL_QUIT;
			SDL_PushEvent(&event2);
			return;
		}
		
	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	
    SDL_UpdateRect(g_pDDSBack,0,0,0,0);
}
