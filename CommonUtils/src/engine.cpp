#include "engine.h"

int							dwFrameCount = 0;
int							dwFrameTime = 0;
int							dwFrames = 0;
int							dwFramesLast = 0;

int							MousX = 0;
int							MousY = 0;

SDL_PixelFormat*			g_pDDPixelFormat = NULL;
SDL_Surface*				g_pDDSBack = NULL;
SDL_Surface*				g_pDDSBack2 = NULL;
SDL_Surface*				g_pDDSBack3 = NULL;
SDL_Surface*				g_pDDSOne = NULL;

SDL_Color					pe[256];
SDL_Color					pe2[256];
Uint32						palcal[256];

Uint16						rramp[256];
Uint16						gramp[256];
Uint16						bramp[256];

Uint16						rramp2[256];
Uint16						gramp2[256];
Uint16						bramp2[256];

double						gamma;
int							gameSpeed;

MouseState					dims;
MouseState					olddims;

int							GetMaxX;
int							GetMaxY;
int							BitDepth;
bool						usefullscreen;

void*						lpSurface  = NULL;
bool						g_bActive;

bool						usingsound;
Mix_Music*					MusicData = NULL;