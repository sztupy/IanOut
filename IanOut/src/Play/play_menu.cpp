#include "play.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"
#include "../commonutils/mouse.h"

void play::MainMenu(void)
{
    HRESULT                     hRet;
	
	static int					Frame = -25;
	static DWORD				ThisTick;
	static DWORD				LastTick = 0;

	int addx = (GetMaxX-640)/2;
	int addy = (GetMaxY-480)/2;
	
	if ((lstrcmpi(FullScreen->fname,"\\art\\intrface\\mainmenu.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"\\art\\intrface\\mainmenu.frm",1);
		
	}

	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3000;
			
        }

	olddims = dims;
	mouse::UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;

	if (MousX>=GetMaxX) { MousX=GetMaxX;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=GetMaxY) { MousY=GetMaxY;}
	if (MousY<=0)   { MousY=0;  }

	ClearSurface(g_pDDSBack,0,0,0);
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
	textfont::IanOutTextR(addx+640,addy+465,4,"Copyright (c) Sztupy corp.");
	
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
			ClearSurface(g_pDDSPrimary,0,0,0);
			MapLoader->InitMainChar(hWnd,g_pDD);
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
			ClearSurface(g_pDDSPrimary,0,0,0);
			PostMessage(hWnd, WM_CLOSE, 0, 0); 
			return;
		}
		
	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
	
    // Flip the surfaces
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
	
}
