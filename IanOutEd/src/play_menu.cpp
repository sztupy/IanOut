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
	//DDBLTFX                     ddbltfx;
	
	static int					Frame = -25;
	static Uint32				ThisTick;
	static Uint32				LastTick = 0;
	//int							i;
	
	if ((strcmp(FullScreen->fname,"\\art\\intrface\\mainmenu.frm")))
	{
    	LoadFRMSingle(&FullScreen,"\\art\\intrface\\mainmenu.frm",1);
	}

	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3;
			
        }

	if (MousX>=GetMaxX) { MousX=GetMaxX;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=GetMaxY) { MousY=GetMaxY;}
	if (MousY<=0)   { MousY=0;  }
	ClearSurface(g_pDDSBack,0,0,0);
	BlitTo(g_pDDSBack,0,0,640,480,0,0,0,FullScreen->FRM->FRM);

	if ((dims.buttons[0]) && mouse::MouseIn(30,20,56,46))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,20,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,20,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,25,2,"Load into memory");
	if ((dims.buttons[0]) && mouse::MouseIn(30,61,56,93))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,61,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,61,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,66,2,"Edit Level");
	if ((dims.buttons[0]) && mouse::MouseIn(30,102,56,134))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,102,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,102,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,107,2,"Edit Objects");
	if ((dims.buttons[0]) && mouse::MouseIn(30,143,56,175))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,143,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,143,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,148,2,"Edit Characters");
	if ((dims.buttons[0]) && mouse::MouseIn(30,184,56,210))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,184,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,184,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,189,2,"File operations");

	if ((dims.buttons[0]) && mouse::MouseIn(30,225,56,250))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,30,225,0,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,30,225,0,MenuUp->FRM->FRM);
	}
	textfont::IanOutTextC(123,230,2,"Exit");

	textutil::DisplayFrameRate();

	textfont::IanOutText(0,465,1,"IanEd - IanOut Level Editor v"VERSIONNUM);
	textfont::IanOutText(455,465,4,"Copyright (c) Sztupy corp.");
	
	if (Frame<(65535/1000)) palette::SetPaletteBl(Frame*1000);

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,20,56,46))
	{ GamePos=2; Frame=-25;palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);palette::SetPaletteBl(65535);Selected=0;return;}

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,61,56,93))
	{ GamePos=1; Frame=-25;palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);palette::SetPaletteBl(65535);Selected=0;return;}

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,225,56,250))
	  { palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);event2.type = SDL_QUIT;
			event2.quit.type = SDL_QUIT;
			SDL_PushEvent(&event2); return;}

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,184,56,210))
	 { GamePos=3; Frame=-25;palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);palette::SetPaletteBl(65535);return;}	  

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,102,56,134))
	{ GamePos=4; Frame=-25;palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);palette::SetPaletteBl(65535);Selected=0;return;}

	if ((olddims.buttons[0]) && !(dims.buttons[0]) && mouse::MouseIn(30,143,56,175))
	{ GamePos=6; Frame=-25;palette::FadeOut(); ClearSurface(g_pDDSBack,0,0,0);palette::SetPaletteBl(65535);Selected=0;return;}

	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	
    // Flip the surfaces
    SDL_Flip(g_pDDSBack);	
}
