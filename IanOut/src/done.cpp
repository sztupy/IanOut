#include "IanOs.h"

#include "done.h"
#include "load.h"
#include "../commonutils/mouse.h"

#include "../commonutils/engine.h"
#include "../commonutils/textutil.h"
#include "graphic.h"
#include "vars.h"

#include "../frmobject/freeimage.h"

void ReleaseAllObjects(void)
{
	DeleteTempDir();
	int i,i2;
    if (g_pDD != NULL)
    {
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;
        }
        if (g_pDDSOne != NULL)
        {
            g_pDDSOne->Release();
            g_pDDSOne = NULL;
        }
        if (g_pDDPal != NULL)
        {
            g_pDDPal->Release();
            g_pDDPal = NULL;
        }
	
	if (Mouse) delete Mouse; Mouse = NULL;
	if (Mouse2) delete Mouse2; Mouse2 = NULL;
	if (KezeloP) delete KezeloP; KezeloP = NULL;
	if (FullScreen) delete FullScreen; FullScreen = NULL;
	if (FullScreen2) delete FullScreen2; FullScreen2 = NULL;
	if (MenuUp) delete MenuUp; MenuUp = NULL;
	if (Select) delete Select; Select = NULL;
	if (Select2) delete Select2; Select2 = NULL;
	if (MenuDown) delete MenuDown; MenuDown = NULL;
	if (SkillDex) delete SkillDex; SkillDex = NULL;

	if (WaitMouse) delete WaitMouse; WaitMouse = NULL;
	
	DeleteExitGrid();

	if (TimerList) delete TimerList; TimerList = NULL;
	if (TauntList) delete TauntList; TauntList = NULL;

	if (ButtonList) delete ButtonList; ButtonList = NULL;

	if (AmmoBox) delete AmmoBox; AmmoBox = NULL;

	if (MapLocations) delete MapLocations; MapLocations = NULL;

	if (MapLoader) delete MapLoader; MapLoader = NULL;

	GarbageCollect();
	if (AnimList) delete AnimList; AnimList = NULL;

	for (i=0; i<8; i++)
	{
		if (MouseScr[i][0]) delete MouseScr[i][0]; MouseScr[i][0] = NULL;
		if (MouseScr[i][1]) delete MouseScr[i][1]; MouseScr[i][1] = NULL;
	}
	for (i=0; i<20; i++)
	{
		if (Temporary[i]) delete Temporary[i]; Temporary[i] = NULL;
		if (i<5) { if (TempAnim[i]) delete TempAnim[i]; TempAnim[i] = NULL; }
	}
	for (i2=0; i2<25; i2++)
	for (i=0; i<2; i++)
	{
		if (Buttons[i2][i]) delete Buttons[i2][i];Buttons[i2][i] = NULL;
	}
        g_pDD->Release();
        g_pDD = NULL;
    }
	for (i=0; i<75; i++)
	{
		delete LINIStr[i];//->~TIniStr();
		LINIStr[i] = NULL;
	}

	FreeImage_DeInitialise();

	mouse::FreeDirectInput();

	}


HRESULT 
RestoreAll(void)
{
    HRESULT                     hRet;

    hRet = g_pDDSPrimary->Restore();
    if (hRet == DD_OK)
    {
        hRet = g_pDDSOne->Restore();
        if (hRet == DD_OK)
        {
            DDReLoadBitmap(g_pDDSOne, szBitmap);
        }
    }
    return hRet;
}