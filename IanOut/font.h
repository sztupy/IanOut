#ifndef __font_h
#define __font_h

#include "ddraw.h"
#include "afx.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern void DisplayNum(int x, int y, int c,unsigned char i,LPDIRECTDRAWSURFACE7 g_pDDSText, LPDIRECTDRAWSURFACE7 g_pDDSOne, LPDIRECTDRAWSURFACE7 g_pDDSBack);
extern void DisplayFrameRate(LPDIRECTDRAWSURFACE7 g_pDDSText, LPDIRECTDRAWSURFACE7 g_pDDSOne, LPDIRECTDRAWSURFACE7 g_pDDSBack );
extern void IanOutText(int x, int y, int co, CString str, LPDIRECTDRAWSURFACE7 g_pDDSText, LPDIRECTDRAWSURFACE7 g_pDDSOne, LPDIRECTDRAWSURFACE7 g_pDDSBack);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif