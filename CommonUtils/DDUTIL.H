/*==========================================================================
 *
 *  Copyright (C) 1998-1999 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#ifndef __ddutil_h
#define __ddutil_h

#include "sdl.h"

extern SDL_Rect						ClipRect;
extern unsigned char				DarkMask[256][10];

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern void					SetClipRect(int x,int y,int x2,int y2);
extern int					BlitTo(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf);
extern int					BlitToStretch(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, int Dx2, int Dy2, Uint32 flags, SDL_Surface* surf);
extern int					BlitToAlpha(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, int alpha);
extern int					BlitToAlt(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, int cback);
extern int					BlitToRo(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, Uint32 cback);
extern void					ClearRect(SDL_Surface* Blit, int Sx1, int Sy1, int Sx2, int Sy2, int c);
extern void					ClearSurface(SDL_Surface* Blit, int r, int g, int b);
extern void					CalculateEntries();

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif