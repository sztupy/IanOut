#include "done.h"

#include "../commonutils/mouse.h"
#include "../commonutils/textutil.h"

#include "../frmobject/freeimage.h"

void ReleaseAllObjects(void)
{
	int i;
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

        g_pDD->Release();
        g_pDD = NULL;
    }
	for (i=0; i<75; i++)
	{
		LINIStr[i]->~TIniStr();
		LINIStr[i] = NULL;
	}

	if (AnimList) delete AnimList;


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