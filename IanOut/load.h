#ifndef __LOAD_H
#define __LOAD_H

#include "global.h"

extern std::string TempDir;

HRESULT DeleteTempDir();
HRESULT InitializeMap();
HRESULT LoadLocation(int locnum, int entrnum);
HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i);
HRESULT LoadFRMAnim(PFRMAnim *LoadIn, HWND hWnd, char* filename);
void CreateIanPalette();
IDirectDrawPalette *  LoadSplashScreen(LPDIRECTDRAWSURFACE7 g_pDDSB);
HRESULT InitApp(HINSTANCE hInstance, int nCmdShow);

#endif //__LOAD_H