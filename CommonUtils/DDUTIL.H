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

#include "ddraw.h"

extern RECT							ClipRect;
extern unsigned char				DarkMask[256][10];
//extern PALETTEENTRY					ConvertEntry[256];

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern IDirectDrawPalette  *DDLoadPalette(IDirectDraw7 *pdd, LPCSTR szBitmap);
extern IDirectDrawSurface7 *DDLoadBitmap(IDirectDraw7 *pdd, LPCSTR szBitmap, int dx, int dy, HRESULT *hRet, const long caps);
extern HRESULT              DDReLoadBitmap(IDirectDrawSurface7 *pdds, LPCSTR szBitmap);
extern HRESULT              DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
extern DWORD                DDColorMatch(IDirectDrawSurface7 *pdds, COLORREF rgb);
extern void					SetClipRect(int left,int top,int right,int bottom);
extern HRESULT              DDSetColorKey(IDirectDrawSurface7 *pdds, COLORREF rgb);
extern HRESULT				BlitTo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf);
extern HRESULT				BlitToStretch(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, int Dx2, int Dy2, DWORD flags, LPDIRECTDRAWSURFACE7 surf);
extern HRESULT				BlitToAlpha(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, int alpha);
extern HRESULT				BlitToAlt(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, int cback);
extern HRESULT				BlitToRo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, DWORD cback);
extern HRESULT				BlitToMask(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, HBITMAP mask);
extern void					ClearRect(LPDIRECTDRAWSURFACE7 Surf, int Sx1, int Sy1, int Sx2, int Sy2, int c);
extern void					ClearSurface(LPDIRECTDRAWSURFACE7 Surf, int r, int g, int b);
extern void					CalculateEntries();

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif