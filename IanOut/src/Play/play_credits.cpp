#include "play.h"

#include "../commonutils/textutil.h"
#include "../commonutils/textfont.h"
#include "done.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "../commonutils/palette.h"


void play::Credits(void)
{
	int hRet;
	std::string					Dumcsi[300];
	int i,c;
	static int							Frame = -20;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	char								buf[80];
	int Stringnumb;
	FILE*	stream;

	Stringnumb=0;
	stream = fopen(GetFile("\\text\\credits.txt").c_str(),"rt");

	while (!(feof(stream)))
	{
	  Stringnumb++;
	  fgets(buf,80,stream);
	  Dumcsi[Stringnumb-1] = buf;

	}
	fclose(stream);

	
	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > 40)
        {
			Frame++;//(ThisTick - LastTick)/40;
            LastTick = ThisTick;
	
        }

    ClearSurface(g_pDDSBack,0,0,0);

	for (i=0; i<Stringnumb; i++)
	{
		if (((i*12+GetMaxY-Frame)<=700) && ((i*12+GetMaxY-Frame)>-15))
		{
			sprintf(buf,"%s",(Dumcsi)[i].c_str());
			c=0;
			switch (buf[0]) {
			case '1':c = 1;break;
			case '2':c = 2;break;
			case '3':c = 3;break;
			case '4':c = 4;break;
			case '5':c = 5;break;
			case 'A':c = 0;break;
			case 'B':c = 1;break;
			case 'C':c = 2;break;
			case 'D':c = 3;break;
			case 'E':c = 4;break;
			case 'F':c = 5;break;
			}
			if ((buf[0]>='A') && (buf[0]<='F'))
			{
			 buf[0]=' ';
			 textfont::IanOutText(0,i*12+GetMaxY-Frame,c,buf);
			} else
			{
			 buf[0]=' ';
			 textfont::IanOutTextC(GetMaxX/2,i*12+GetMaxY-Frame,c,buf);
			}
		}
	}
	textutil::DisplayFrameRate();
	MousX = 200;
	if (dims.buttons[1]) Frame--;
	if (((Stringnumb*12+GetMaxY-Frame)<-15) || (dims.buttons[0])) { Frame = -20; GamePos=0; palette::FadeOut(); /*SetPaletteBl(65535);*/return;}


    SDL_Flip(g_pDDSBack);

	if (Frame<-10) palette::SetPaletteBl(65535);

}
