#include "done.h"

#include "../commonutils/mouse.h"
#include "../commonutils/textutil.h"

void ReleaseAllObjects(void)
{
	int i;
    if (g_pDDSOne != NULL)
    {
        SDL_FreeSurface(g_pDDSOne);
        g_pDDSOne = NULL;
    }
	if (Mouse) delete Mouse;
	if (FullScreen) delete FullScreen;
	if (MenuUp) delete MenuUp;
	if (Select) delete Select;
	if (Select2) delete Select2;
	if (MenuDown) delete MenuDown;
	
	if (MapInf) delete MapInf;
	if (StaticInf) delete StaticInf;
	for (i=0; i<8; i++)
	{
		if (MouseScr[i][0]) delete MouseScr[i][0];
		if (MouseScr[i][1]) delete MouseScr[i][1];
	}
	for (i=0; i<2; i++)
	{
		if (Buttons[i]) delete Buttons[i];
	}
	for (i=0; i<75; i++)
	{
		LINIStr[i]->~TIniStr();
		LINIStr[i] = NULL;
	}

	if (AnimList) delete AnimList;

	SDL_Quit();
}


int 
RestoreAll(void)
{
	return 0;
}