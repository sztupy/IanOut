#include "engine.h"

DWORD						dwFrameCount;
DWORD						dwFrameTime;
DWORD						dwFrames;
DWORD						dwFramesLast;

// Name of our bitmap resource.
char						szBitmap[] = "ALL";

LPDIRECTDRAW7				g_pDD = NULL;			// DirectDraw object
LPDIRECTDRAWSURFACE7		g_pDDSPrimary = NULL;	// DirectDraw primary surface
LPDIRECTDRAWSURFACE7		g_pDDSBack = NULL;		// DirectDraw back surface
LPDIRECTDRAWSURFACE7		g_pDDSBack2 = NULL;
LPDIRECTDRAWSURFACE7		g_pDDSOne = NULL;		// Offscreen surface 1

LPDIRECTDRAWPALETTE			g_pDDPal = NULL;		// The primary surface palette
LPVOID						lpSurface  = NULL;
bool						g_bActive = FALSE;		// Is application active?

DIMOUSESTATE				dims;					// DirectInput mouse state structure
DIMOUSESTATE				olddims;				// DirectInput mouse state structure
IDirectInput*				g_pDI = NULL;
IDirectInputDevice*			g_pMouse = NULL;

int							MousX = 320;
int							MousY = 240;

PALETTEENTRY				pe[256];
PALETTEENTRY				pe2[256];
DWORD						palcal[256];

LPDIRECTDRAWGAMMACONTROL	lpDDGammaControl = NULL;
DDGAMMARAMP					DDGammaRamp;
DDGAMMARAMP					DDGammaOld;

double						gamma;
int							gameSpeed;
HWND						hWnd;

int							GetMaxX;
int							GetMaxY;
int 						BitDepth;