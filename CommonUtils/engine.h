#ifndef __ENGINE_H
#define __ENGINE_H

#include "utils.h"
#include "discutil.h"
#include "ddutil.h"
#include "sdl_mixer.h"

typedef struct MouseState {
	Uint16 x, y;
	Sint16 lX, lY;
	Uint8 buttons[3];
} MouseState;

extern int							dwFrameCount;
extern int							dwFrameTime;
extern int							dwFrames;
extern int							dwFramesLast;

extern int							MousX;
extern int							MousY;

extern SDL_PixelFormat*				g_pDDPixelFormat;
extern SDL_Surface*					g_pDDSBack;
extern SDL_Surface*					g_pDDSBack2;
extern SDL_Surface*					g_pDDSBack3;
extern SDL_Surface*					g_pDDSOne;

extern SDL_Color					pe[256];
extern SDL_Color					pe2[256];
extern Uint32						palcal[256];

extern Uint16						rramp[256];
extern Uint16						gramp[256];
extern Uint16						bramp[256];

extern Uint16						rramp2[256];
extern Uint16						gramp2[256];
extern Uint16						bramp2[256];

extern MouseState					dims;
extern MouseState					olddims;

extern double						gamma;
extern int							gameSpeed;
//extern HWND							hWnd;

extern int							GetMaxX;
extern int							GetMaxY;
extern int							BitDepth;
extern bool							usefullscreen;

extern void*						lpSurface;
extern bool							g_bActive;

extern bool							usingsound;
extern Mix_Music*					MusicData;

#endif //__ENGINE_H