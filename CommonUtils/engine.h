#ifndef __ENGINE_H
#define __ENGINE_H

#include "ddutil.h"
#include "utils.h"
#include "discutil.h"
#include <dinput.h>
#include "utils.h"

extern DWORD						dwFrameCount;
extern DWORD						dwFrameTime;
extern DWORD						dwFrames;
extern DWORD						dwFramesLast;
extern char							szBitmap[];

extern int							MousX;
extern int							MousY;

extern LPDIRECTDRAW7				g_pDD;
extern LPDIRECTDRAWSURFACE7			g_pDDSPrimary;
extern LPDIRECTDRAWSURFACE7			g_pDDSBack;
extern LPDIRECTDRAWSURFACE7			g_pDDSBack2;
extern LPDIRECTDRAWSURFACE7			g_pDDSOne;

extern LPDIRECTDRAWPALETTE			g_pDDPal;

extern DIMOUSESTATE					dims;
extern DIMOUSESTATE					olddims;
extern IDirectInput*				g_pDI;
extern IDirectInputDevice*			g_pMouse;

extern PALETTEENTRY					pe[256];
extern PALETTEENTRY					pe2[256];
extern DWORD						palcal[256];

extern LPDIRECTDRAWGAMMACONTROL		lpDDGammaControl;
extern DDGAMMARAMP					DDGammaRamp;
extern DDGAMMARAMP					DDGammaOld;

extern double						gamma;
extern int							gameSpeed;
extern HWND							hWnd;

extern int							GetMaxX;
extern int							GetMaxY;
extern int							BitDepth;

extern LPVOID						lpSurface;
extern bool							g_bActive;

#endif //__ENGINE_H