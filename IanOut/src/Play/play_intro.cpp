#include "play.h"
#include "../commonutils/mouse.h"
#include "load.h"
#include "done.h"
#include "../commonutils/textutil.h"
#include "../commonutils/textfont.h"
#include "../commonutils/palette.h"

void play::Intro()
{
/*	int									hRet;

	static int							Fram = -255;
	static double						Lines[25];							
	static double						Linpe[25];
	static int							Lince[25];
	static int							Linti[25];
	int									Frame;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	char								buf[150];
	char								buf2[10];
	int									stringnumb;
	int									i;

	int addx = (GetMaxX-640)/2;
	int addy = (GetMaxY-480)/2;

	GetPrivateProfileString("Intro","text","0",buf,80,GetFile("\\text\\dialog\\intro.sve").c_str());
	stringnumb = atoi(buf);
	ThisTick = SDL_GetTicks();
	
	if (Fram==-255)
	{
	 for (i=0; i<25; i++)
	 {
		 Lines[i]=rand()%540+50;
		 Linpe[i]=((double)(rand()%80-40))/20;
		 Lince[i]=rand()%4+11;
		 Linti[i]=rand()%150;
	 }
	}

	if (Fram<=0) { Fram+=5; } else
	if ((ThisTick - LastTick) > 70)
        {
			Fram++;
            LastTick = ThisTick;
	    }

	for (i=0; i<25; i++) {
		 Lines[i]+=Linpe[i];
		 Linti[i]-=3;
		 if ((Lince[i]<5) || (Lines[i]<50) || (Lines[i]>590))
		 {
		  Lines[i]=rand()%540+50;
		  Linpe[i]=((double)(rand()%80-40))/20;
		  Lince[i]=rand()%4+11;
		  Linti[i]=rand()%150;
		 }
		 
		 if (Linti[i]<0) Lince[i]--;
	}

	Frame = Fram;
	if (Frame<0) Frame=0;
	palette::SetPaletteBl((Fram+255)*256);

	ClearSurface(g_pDDSBack,0,0,0);
    
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 5;
	
	   rcRect.top=addy+50;
	  rcRect.left=addx+50;
	rcRect.bottom=addy+450;
	 rcRect.right=addx+590;

	 ClearRect(g_pDDSBack,0,0,0);
    g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	sprintf(buf2,"text%i",(Frame/50));
	GetPrivateProfileString("Intro",buf2,"",buf,80,GetFile("\\text\\dialog\\intro.sve").c_str());
	textfont::IanOutTextC(addx+320,addy+460,0,buf);

	if ((Frame>=0) && (Frame<=54))
	{
	 if ((strcmp(FullScreen2->fname,"\\art\\intrface\\intro0.frm")))
	 {
      LoadFRMSingle(&FullScreen2,"\\art\\intrface\\intro0.frm",1);
	 }
	 BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen2->FRM->FRM);
	}

	if ((Frame>=50) && (Frame<=325))
	{
	 if ((strcmp(FullScreen->fname,"\\art\\intrface\\intro1.frm")))
	 {
      LoadFRMSingle(&FullScreen,"\\art\\intrface\\intro1.frm",1);
	 }
	 if ((Frame>=25) && (Frame<=54))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen->FRM->FRM,(50*(Frame-50)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen->FRM->FRM);
	 }
	 //BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen->FRM->FRM);
	}

	if ((Frame>=321) && (Frame<=665+50))
	{
	 if ((strcmp(FullScreen2->fname,"\\art\\intrface\\intro2.frm")))
	 {
      LoadFRMSingle(&FullScreen2,"\\art\\intrface\\intro2.frm",1);
	 }
	 if ((Frame>=300) && (Frame<=325))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen2->FRM->FRM,(50*(Frame-321)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen2->FRM->FRM);
	 }
	}

	if ((Frame>=661+50) && (Frame<=875))
    {
	 if ((strcmp(FullScreen->fname,"\\art\\intrface\\intro4.frm")))
     {
      LoadFRMSingle(&FullScreen,"\\art\\intrface\\intro4.frm",1);
     }
     if ((Frame>=700) && (Frame<=665+50))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen->FRM->FRM,(50*(Frame-661-50)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen->FRM->FRM);
	 }
	}

	if ((Frame>=871) && (Frame<=1000))
	{
	 if ((strcmp(FullScreen2->fname,"\\art\\intrface\\intro3.frm")))
	 {
      LoadFRMSingle(&FullScreen2,"\\art\\intrface\\intro3.frm",1);
	 }
	 if ((Frame>=850) && (Frame<=875))
	 {
	  BlitToAlpha(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen2->FRM->FRM,(50*(Frame-871)));
	 } else
	 {
	  BlitTo(g_pDDSBack,0,0,540,400,addx+50,addy+50,0,FullScreen2->FRM->FRM);
	 }
	}

	if ((Frame>=0 ) && (Frame<150)) 
	{textfont::IanOutTextC(addx+100,addy+150,0,"IanOut");}
	if ((Frame>150) && (Frame<300)) 
	{textfont::IanOutTextC(addx+450,addy+400,0,"Program:");
	 textfont::IanOutTextC(addx+450,addy+412,0,"Sztupák Sz. Zsolt");}
	if ((Frame>300) && (Frame<450)) 
	{textfont::IanOutTextC(addx+430,addy+150,0,"Ötlet:");
	 textfont::IanOutTextC(addx+430,addy+162,0,"Czehelszki Levente");
	 textfont::IanOutTextC(addx+430,addy+174,0,"Sztupák Sz. Zsolt");}
	if ((Frame>450) && (Frame<600)) 
	{textfont::IanOutTextC(addx+200,addy+350,0,"Szöveg:");
	 textfont::IanOutTextC(addx+200,addy+362,0,"Czehelszki Levente");}
	if ((Frame>600) && (Frame<750)) 
	{textfont::IanOutTextC(addx+400,addy+300,0,"Grafika:");
	 textfont::IanOutTextC(addx+400,addy+312,0,"Sztupák Sz. Zsolt");
	 textfont::IanOutTextC(addx+400,addy+324,0,"Black Isle Studio");}
	if ((Frame>750) && (Frame<900)) 
	{textfont::IanOutTextC(addx+250,addy+180,0,"Zene:");
	 textfont::IanOutTextC(addx+250,addy+192,0,"Még nincs");}

	textutil::DisplayFrameRate();

	for (i=0; i<10; i++)
	{
	 ZeroMemory(&ddbltfx, sizeof(ddbltfx));
     ddbltfx.dwSize = sizeof(ddbltfx);
     ddbltfx.dwFillColor = RGB(Lince[i],Lince[i],Lince[i]);
	 rcRect.top=addy+50;
	 rcRect.left=addx+(int)Lines[i];
	 rcRect.bottom=addy+450;
	 rcRect.right=addx+(int)Lines[i]+1;
     g_pDDSBack->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	}

	mouse::UpdateInputState();
	if (((Frame/50))>stringnumb) { Fram = -255; GamePos=0; palette::FadeOut(); return;}
	if ((dims.rgbButtons[0] & 0x80)) { Fram = -255; GamePos=0; palette::FadeOut(); return;}
	
    SDL_Flip(g_pDDSBack);
*/	
	GamePos=0;
}