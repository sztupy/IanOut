#ifndef __LOAD_H
#define __LOAD_H

#include "global.h"

HRESULT LoadLevel(HWND hWnd, const char* filename);
HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i);
//HRESULT AddAnim(HWND hWnd, const char filename[10]);
void CreateIanPalette();
IDirectDrawPalette *  LoadSplashScreen(LPDIRECTDRAWSURFACE7 g_pDDSB);
HRESULT InitApp(HINSTANCE hInstance, int nCmdShow);

#endif //__LOAD_H