//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: Routines for loading bitmap and palettes from resources
//
//
// Copyright (c) 1995-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "ddutil.h"
#include "utils.h"
#include "discutil.h"
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <wingdi.h>

extern int BitDepth;
extern int rsz, gsz, bsz; 	//bitsize of field
extern int rsh, gsh, bsh;	//0’s on left (the shift value)
RECT ClipRect;
unsigned char DarkMask[256][10];

extern "C"
void CalculateEntries()
{

}

extern "C"
void SetClipRect(int left,int top,int right,int bottom)
{
	ClipRect.bottom=bottom-1;
	ClipRect.left=left;
	ClipRect.right=right-1;
	ClipRect.top=top;
}

extern "C"
HRESULT BlitToAlt(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, int cback)
{
	if (cback<0) return LogFail(DDERR_INVALIDPARAMS,"AltDraw");;
	if (cback>9) return LogFail(DDERR_INVALIDPARAMS,"AltDraw");

	DDPIXELFORMAT DDpf;
	long int r,g,b;

    ZeroMemory (&DDpf, sizeof(DDpf));
    DDpf.dwSize = sizeof(DDpf);
    Blit->GetPixelFormat(&DDpf);

    DDSURFACEDESC2				ddsdSource;
	DDSURFACEDESC2				ddsdTarget;
	DWORD						dwTargetPad;
	DWORD						dwSourcePad;
    RECT                        rcRect;
	RECT						rcRectTarget;
	HRESULT                     hRet;
	WORD*						lpbTarget;
	WORD*						lpbSource;

	DWORD*						lpbTarget32;
	DWORD*						lpbSource32;

	WORD						dwSourceTemp;
	DWORD						dwSourceTemp32;

	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;
	int							iWidth;
	int							iHeight;

	int							i;

	cback/=2;

	hRet = DD_OK;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);}
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);}
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};
		
    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

	rcRectTarget.left = BltX; 
	rcRectTarget.top = BltY;
	rcRectTarget.right = BltX+(Sx2-Sx1);
	rcRectTarget.bottom = BltY+(Sy2-Sy1);

	if ((Sx1 != Sx2) && (Sy1 != Sy2))  {

	memset( &ddsdTarget, 0, sizeof ddsdTarget );
	ddsdTarget.dwSize = sizeof ddsdTarget;
	hRet = Blit->Lock( &rcRectTarget, &ddsdTarget, DDLOCK_WAIT, NULL );  

	if (hRet == DD_OK) {
		memset( &ddsdSource, 0, sizeof ddsdSource );
		ddsdSource.dwSize = sizeof ddsdSource;
		hRet = surf->Lock( &rcRect, &ddsdSource, DDLOCK_WAIT, NULL );
		if (hRet == DD_OK) {

			iWidth = Sx2-Sx1;
			iHeight = Sy2-Sy1;

			dwTargetPad = ddsdTarget.lPitch/(BitDepth==32?4:2) - ( iWidth );
			dwSourcePad = ddsdSource.lPitch/(BitDepth==32?4:2) - ( iWidth );

			if (BitDepth==32) {
				lpbTarget32 = ( DWORD* ) ddsdTarget.lpSurface;
				lpbSource32 = ( DWORD* ) ddsdSource.lpSurface;
			} else {
				lpbTarget = ( WORD* ) ddsdTarget.lpSurface;
				lpbSource = ( WORD* ) ddsdSource.lpSurface;
			}

			do {
				// Reset the width.
				i = iWidth;
				while ( i-- > 0 )
				{
					if (BitDepth==32) {
						dwSourceTemp32 = *( ( DWORD* ) lpbSource32 );
						if ( dwSourceTemp32 != 0 )
						{
							r = (dwSourceTemp32 & DDpf.dwRBitMask) >> rsh;
							g = (dwSourceTemp32 & DDpf.dwGBitMask) >> gsh;
							b = (dwSourceTemp32 & DDpf.dwBBitMask) >> bsh;

							r -= (5-cback)<<3;if (r<0) r=0;
							g -= (5-cback)<<3;if (g<0) g=0;
							b -= (5-cback)<<3;if (b<0) b=0;

							r <<= rsh;
							g <<= gsh;
							b <<= bsh;

 							*(  lpbTarget32 ) = (DWORD)((DWORD)r|(DWORD)g|(DWORD)b);
						} 
						lpbTarget32 += 1;
						lpbSource32 += 1;
					} else {
						dwSourceTemp = *( ( WORD* ) lpbSource );
						if ( dwSourceTemp != 0 )
						{
							r = (dwSourceTemp & DDpf.dwRBitMask) >> rsh;
							g = (dwSourceTemp & DDpf.dwGBitMask) >> gsh;
							b = (dwSourceTemp & DDpf.dwBBitMask) >> bsh;

							r -= (5-cback);if (r<0) r=0;
							g -= (5-cback)<<(rsz==gsz?0:1);if (g<0) g=0;
							b -= (5-cback);if (b<0) b=0;

							r <<= rsh;
							g <<= gsh;
							b <<= bsh;
 							*(  lpbTarget ) = (WORD)((WORD)r|(WORD)g|(WORD)b);
						} 
						lpbTarget += 1;
						lpbSource += 1;
					}
				}
				if (BitDepth==32) {
					lpbTarget32 += dwTargetPad;
					lpbSource32 += dwSourcePad;
				} else {
					lpbTarget += dwTargetPad;
					lpbSource += dwSourcePad;
				}
			} 
			while ( --iHeight > 0 );

			// Unlock the source surface.
			hRet = surf->Unlock( &rcRect );
			if (hRet != DD_OK) LogFail(hRet,"Error while Locking");
		} else {
			LogFail(hRet,"Error while unlocking");
		}
	hRet = Blit->Unlock( &rcRectTarget );
	if (hRet != DD_OK) LogFail(hRet,"Error while locking");
	} else {
		LogFail(hRet,"Error while unlocking");
	}
	}
	}
	// Return the result.
	return hRet;
}

extern "C"
HRESULT BlitToRo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, DWORD cback)
{
	//if (cback<0) return LogFail(DDERR_INVALIDPARAMS,"RoDraw");;
	//if (cback>9) return LogFail(DDERR_INVALIDPARAMS,"AltDraw");
    DDSURFACEDESC2				ddsdSource;
	DDSURFACEDESC2				ddsdTarget;
	DWORD						dwTargetPad;
	DWORD						dwSourcePad;
    RECT                        rcRect;
	RECT						rcRectTarget;
	HRESULT                     hRet;
	WORD*						lpbTarget;
	WORD*						lpbSource;

	DWORD*						lpbTarget32;
	DWORD*						lpbSource32;

//	BYTE						dwTargetTemp;
	WORD						dwSourceTemp;
	DWORD						dwSourceTemp32;

	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;
	int							iWidth;
	int							iHeight;

	int							i;

	hRet = DD_OK;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);}
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);}
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};
		
    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

	rcRectTarget.left = BltX-1; 
	rcRectTarget.top = BltY-1;
	rcRectTarget.right = BltX+(Sx2-Sx1)+1;
	rcRectTarget.bottom = BltY+(Sy2-Sy1)+1;

	bool top,left,right,bottom;
	top = true; left = true; right = true; bottom = true;
	if (rcRectTarget.left<ClipRect.left) { rcRectTarget.left = ClipRect.left; left = false; }
	if (rcRectTarget.top<ClipRect.top) { rcRectTarget.top = ClipRect.top; top = false; }
	if (rcRectTarget.right>ClipRect.right) { rcRectTarget.right = ClipRect.right;  right = false; }
	if (rcRectTarget.bottom>ClipRect.bottom) { rcRectTarget.bottom = ClipRect.bottom; bottom = false; }
	
	rcRectTarget.left = 0;//ClipRect.left;
	rcRectTarget.top = 0;//ClipRect.top;
	rcRectTarget.right = ClipRect.right;
	rcRectTarget.bottom = ClipRect.bottom;

	if ((Sx1 != Sx2) && (Sy1 != Sy2))  {

	memset( &ddsdTarget, 0, sizeof ddsdTarget );
	ddsdTarget.dwSize = sizeof ddsdTarget;
	hRet = Blit->Lock( &rcRectTarget, &ddsdTarget, DDLOCK_WAIT, NULL );  

	if (hRet == DD_OK) {
		memset( &ddsdSource, 0, sizeof ddsdSource );
		ddsdSource.dwSize = sizeof ddsdSource;
		hRet = surf->Lock( &rcRect, &ddsdSource, DDLOCK_WAIT, NULL );
		if (hRet == DD_OK) {

			iWidth = Sx2-Sx1;
			iHeight = Sy2-Sy1;

			int iWidthT = Sx2-Sx1-(left?1:0)-(right?1:0);
			int iHeightT = Sy2-Sy1-(top?1:0)-(bottom?1:0);

			dwTargetPad = ddsdTarget.lPitch/(BitDepth==32?4:2) - ( iWidthT );
			dwSourcePad = ddsdSource.lPitch/(BitDepth==32?4:2) - ( iWidth );

			if (BitDepth == 32) {
				lpbTarget32 = ( DWORD* )ddsdTarget.lpSurface;
				lpbTarget32 += ((BltY)*ddsdTarget.lPitch/4+BltX);
				lpbSource32 = ( DWORD* ) ddsdSource.lpSurface;
			} else {
				lpbTarget = ( WORD* )ddsdTarget.lpSurface;
				lpbTarget += ((BltY)*ddsdTarget.lPitch/2+BltX);
				lpbSource = ( WORD* ) ddsdSource.lpSurface;
			}

			int ScX, ScY;
			ScX = BltX;
			ScY = BltY;

			bool istop,isleft,isright,isbottom;
			istop = true; isleft = true; isright = false; isbottom = false;

			/*DWORD cback;
			r >>= (8-rsz);
			g >>= (8-gsz);
			b >>= (8-bsz);

			r <<= rsh;
			g <<= gsh;
			b <<= bsh;
			cback = (DWORD)(r | g | b);*/

			do {
				// Reset the width.
				if (iHeight<=0) isbottom = true; else isbottom=false;
				isleft = true;
				i = iWidth;
				while ( i-- > 0 )
				{
					if (i<=0) isright = true; else isright = false;
					if (BitDepth==32) {
						dwSourceTemp32 = *(lpbSource32 );
						if ( dwSourceTemp32 != 0 )
							{
								
								if (((!isleft) && ((*(lpbSource32-1))==0)) || ((isleft) && (left))) {
									*(lpbTarget32-1) = cback;
								}
								if (((!isright) && ((*(lpbSource32+1))==0)) || ((isright) && (right))) {
									*(lpbTarget32+1) = cback;
									
								}
								if (((!istop) && ((*(lpbSource32-ddsdSource.lPitch/4))==0)) || ((istop) && (top))) {
									*(lpbTarget32-ddsdTarget.lPitch/4) = cback;	
								}
								if (((!isbottom) && ((*(lpbSource32+ddsdSource.lPitch/4))==0)) || ((isbottom) && (bottom))) {
									*(lpbTarget32+ddsdTarget.lPitch/4) = cback;	
								}					
							} 
							lpbTarget32 += 1;
							lpbSource32 += 1;
					} else {
						dwSourceTemp = *(lpbSource );
						if ( dwSourceTemp != 0 )
							{
								
								if (((!isleft) && ((*(lpbSource-1))==0)) || ((isleft) && (left))) {
									*(lpbTarget-1) = (WORD)cback;
								}
								if (((!isright) && ((*(lpbSource+1))==0)) || ((isright) && (right))) {
									*(lpbTarget+1) = (WORD)cback;
									
								}
								if (((!istop) && ((*(lpbSource-ddsdSource.lPitch/2))==0)) || ((istop) && (top))) {
									*(lpbTarget-ddsdTarget.lPitch/2) = (WORD)cback;	
								}
								if (((!isbottom) && ((*(lpbSource+ddsdSource.lPitch/2))==0)) || ((isbottom) && (bottom))) {
									*(lpbTarget+ddsdTarget.lPitch/2) = (WORD)cback;	
								}					
							} 
							lpbTarget += 1;
							lpbSource += 1;
					}
					ScX++;
					isleft=false;
				} 
				if (BitDepth==32) {
					lpbTarget32 += dwTargetPad-(left?1:0)-(right?1:0);
					lpbSource32 += dwSourcePad;
				} else {
					lpbTarget += dwTargetPad-(left?1:0)-(right?1:0);
					lpbSource += dwSourcePad;
				}
				istop=false;
				ScX = BltX;
				ScY++;
			} while ( --iHeight > 0 );

			// Unlock the source surface.
			hRet = surf->Unlock( &rcRect );
			if (hRet != DD_OK) LogFail(hRet,"Error while Locking");
		} else {
			LogFail(hRet,"Error while unlocking");
		}
	hRet = Blit->Unlock( &rcRectTarget );
	if (hRet != DD_OK) LogFail(hRet,"Error while locking");
	} else {
		LogFail(hRet,"Error while unlocking");
	}
	}
	}
	// Return the result.
	return hRet;
}

extern "C"
HRESULT BlitTo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf)
{
    DDBLTFX                     ddbltfx;
    RECT                        rcRect;
	//RECT						rcRectTarget;
	HRESULT                     hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	hRet = DD_OK;
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);};
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);};
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};

      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;

	 /*rcRectTarget.left = BltX; 
	rcRectTarget.top = BltY;
	rcRectTarget.right = BltX+(Sx2-Sx1);
	rcRectTarget.bottom = BltY+(Sy2-Sy1);*/
	  if ((Sx1 != Sx2) && (Sy1 != Sy2))
    while (TRUE)
    {
        hRet = Blit->BltFast(BltX,BltY,surf,&rcRect,flags);
		//hRet = Blit->Blt(&rcRectTarget,surf,&rcRect,DDBLT_KEYSRC,&ddbltfx);

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return LogFail(hRet,"Error while drawing");;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return LogFail(hRet,"Error while drawing");;
    }
    if (hRet != DD_OK)
        return LogFail(hRet,"Error while drawing");
	}

	return hRet;
}


extern "C"
HRESULT BlitToStretch(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, int Dx2, int Dy2, DWORD flags, LPDIRECTDRAWSURFACE7 surf)
{
    DDBLTFX                     ddbltfx;
    RECT                        rcRect;
	RECT						rcRectTarget;
	HRESULT                     hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	hRet = DD_OK;
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);};
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);};
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};

      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;

	rcRectTarget.left = BltX; 
	rcRectTarget.top = BltY;
	rcRectTarget.right = BltX+(Dx2-Dx);
	rcRectTarget.bottom = BltY+(Dy2-Dy);
	if ((Sx1 != Sx2) && (Sy1 != Sy2) && (Dx2 > Dx) && (Dy2 > Dy))
    while (TRUE)
    {
        //hRet = Blit->BltFast(BltX,BltY,surf,&rcRect,flags);
		hRet = Blit->Blt(&rcRectTarget,surf,&rcRect,DDBLT_KEYSRC,&ddbltfx);

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return LogFail(hRet,"Error while drawing (stretch)");
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return LogFail(hRet,"Error while drawing (stretch)");
    }
    if (hRet != DD_OK)
        return LogFail(hRet,"Error while drawing (stretch)");
	}

	return hRet;
}

extern "C"
HRESULT BlitToAlpha(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, int alpha)
{
    RECT                        rcRect;
	HRESULT                     hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	HDC							HDCBlit;
	HDC							HDCSurf;

	BLENDFUNCTION				BLDF;

	hRet = DD_OK;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);}
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);}
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};
		
      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;
	if ((Sx1 != Sx2) && (Sy1 != Sy2))
    while (TRUE)
    {
        hRet = Blit->GetDC(&HDCBlit);
		if (hRet != DD_OK) LogFail(hRet,"Error while GetDC (Alpha)");
		if (!FAILED(hRet)) {
		 hRet = surf->GetDC(&HDCSurf);
		 if (hRet != DD_OK) LogFail(hRet,"Error while GetDC (Alpha)");
		 if (!FAILED(hRet)) {
		  iw1 = Sx2-Sx1;
		  iw2 = Sy2-Sy1;
		  BLDF.BlendOp=0;//AC_SRC_OVER;
		  BLDF.BlendFlags=0;
		  BLDF.SourceConstantAlpha=alpha;
		  BLDF.AlphaFormat=0;
		  AlphaBlend(HDCBlit,BltX,BltY,iw1,iw2,HDCSurf,Sx1,Sy1,iw1,iw2,BLDF);
		  
		  hRet = surf->ReleaseDC(HDCSurf);
		  if (hRet != DD_OK) LogFail(hRet,"Error while ReleaseDC (Alpha)");
		 }
		 hRet = Blit->ReleaseDC(HDCBlit);
		 if (hRet != DD_OK) LogFail(hRet,"Error while ReleasDC (Alpha)");
		}

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return LogFail(hRet,"Error while drawing (Alpha)");
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return LogFail(hRet,"Error while drawing (Alpha)");
    }
    if (hRet != DD_OK)
        return LogFail(hRet,"Error while drawing (Alpha)");
	}

	return hRet;
}

extern "C"
HRESULT BlitToMask(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf, HBITMAP mask)
{
    RECT                        rcRect;
	HRESULT                     hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	HDC							HDCBlit;
	HDC							HDCSurf;

	hRet = DD_OK;

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.right) || (Dy>ClipRect.bottom) || (Dx+iw1<ClipRect.left) || (Dy+iw2<ClipRect.top))) 
	{
	if (Dx+iw1>ClipRect.right) {iw1 = iw1-(Dx+iw1-ClipRect.right);}
	if (Dy+iw2>ClipRect.bottom) {iw2 = iw2-(Dy+iw2-ClipRect.bottom);}
		
	Sx2 = Sx1+iw1;
	Sy2 = Sy1+iw2;

	if (Dx<ClipRect.left) { Sx1+=(-Dx+ClipRect.left);BltX=ClipRect.left;};
	if (Dy<ClipRect.top) { Sy1+=(-Dy+ClipRect.top);BltY=ClipRect.top;};
		
      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;
	if ((Sx1 != Sx2) && (Sy1 != Sy2))
    while (TRUE)
    {
        hRet = Blit->GetDC(&HDCBlit);
		if (hRet != DD_OK) LogFail(hRet,"Error while GetDC (Mask)");
		if (!FAILED(hRet)) {
		 hRet = surf->GetDC(&HDCSurf);
		 if (hRet != DD_OK) LogFail(hRet,"Error while GetDC (Mask)");
		 if (!FAILED(hRet)) {
		  iw1 = Sx2-Sx1;
		  iw2 = Sy2-Sy1;

		  /*BOOL error = */MaskBlt(HDCBlit,BltX,BltY,iw1,iw2,HDCSurf,Sx1,Sy1,mask,0,0,SRCCOPY);
          //if (error == 0) BlitTo(Blit,Sx1,Sy1,Sx2,Sy2,Dx,Dy,flags,surf);

		  hRet = surf->ReleaseDC(HDCSurf);
		  if (hRet != DD_OK) LogFail(hRet,"Error while ReleaseDC (Mask)");
		 }
		 hRet = Blit->ReleaseDC(HDCBlit);
		 if (hRet != DD_OK) LogFail(hRet,"Error while ReleaseDC (Mask)");
		}

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return LogFail(hRet,"Error while drawing (Mask)");
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return LogFail(hRet,"Error while drawing (Mask)");
    }
    if (hRet != DD_OK)
        return LogFail(hRet,"Error while drawing (Mask)");
	}

	return hRet;
}


extern "C"
void ClearSurface(LPDIRECTDRAWSURFACE7 Surf, int r, int g, int b)
{
	DDBLTFX                     ddbltfx;
	
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(r,g,b);
    HRESULT hRet = Surf->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	if (hRet != DD_OK) LogFail(hRet,"ClearSurface failed");
}

extern "C"
void ClearRect(LPDIRECTDRAWSURFACE7 Surf, int Sx1, int Sy1, int Sx2, int Sy2, int c)
{
	DDBLTFX                     ddbltfx;
	RECT						rcRect;

    rcRect.left = Sx1;
	rcRect.right = Sx2;
	rcRect.top = Sy1;
	rcRect.bottom = Sy2;
	if ((Sx1 != Sx2) && (Sy1 != Sy2)) {
		ZeroMemory(&ddbltfx, sizeof(ddbltfx));
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = c;
		HRESULT hRet = Surf->Blt(&rcRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		if (hRet != DD_OK) LogFail(hRet,"ClearRect failed");
	}
}

//-----------------------------------------------------------------------------
// Name: DDLoadBitmap()
// Desc: Create a DirectDrawSurface from a bitmap resource.
//-----------------------------------------------------------------------------

extern "C" IDirectDrawSurface7 *
DDLoadBitmap(IDirectDraw7 * pdd, LPCSTR szBitmap, int dx, int dy, HRESULT *hRet, const long caps )
{
    HBITMAP                 hbm;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;
    IDirectDrawSurface7    *pdds;

    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file
    //
	*hRet = DDERR_NOTLOADED;

    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx,
                              dy, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
        return NULL;
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    //
    // Create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = caps;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;
    if (( *hRet = pdd->CreateSurface(&ddsd, &pdds, NULL) ) != DD_OK)
        return NULL;
    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    DeleteObject(hbm);
    return pdds;
}




//-----------------------------------------------------------------------------
// Name: DDReLoadBitmap()
// Desc: Load a bitmap from a file or resource into a directdraw surface.
//       normaly used to re-load a surface after a restore.
//-----------------------------------------------------------------------------
HRESULT
DDReLoadBitmap(IDirectDrawSurface7 * pdds, LPCSTR szBitmap)
{
    HBITMAP                 hbm;
    HRESULT                 hr;

    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file
    //
    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                              0, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;
}




//-----------------------------------------------------------------------------
// Name: DDCopyBitmap()
// Desc: Draw a bitmap into a DirectDrawSurface
//-----------------------------------------------------------------------------
extern "C" HRESULT
DDCopyBitmap(IDirectDrawSurface7 * pdds, HBITMAP hbm, int x, int y,
             int dx, int dy)
{
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;
    //
    // Make sure this surface is restored.
    //
    pdds->Restore();
    //
    // Select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = dx == 0 ? bm.bmWidth : dx;     // Use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;
    //
    // Get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                   dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    return hr;
}




//-----------------------------------------------------------------------------
// Name: DDLoadPalette()
// Desc: Create a DirectDraw palette object from a bitmap resource
//       if the resource does not exist or NULL is passed create a
//       default 332 palette.
//-----------------------------------------------------------------------------
extern "C" IDirectDrawPalette *
DDLoadPalette(IDirectDraw7 * pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette     *ddpal;
    int                     i;
    int                     n;
    int                     fh;
    HRSRC                   h;
    LPBITMAPINFOHEADER      lpbi;
    PALETTEENTRY            ape[256];
    RGBQUAD                *prgb;

    //
    // Build a 332 palette as the default.
    //
    for (i = 0; i < 256; i++)
    {
        ape[i].peRed = (BYTE) (((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE) (((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue = (BYTE) (((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE) 0;
    }
    //
    // Get a pointer to the bitmap resource.
    //
    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER) LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD *) ((BYTE *) lpbi + lpbi->biSize);
        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;
        //
        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for (i = 0; i < n; i++)
        {
            ape[i].peRed = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER        bf;
        BITMAPINFOHEADER        bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);
        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;
        //
        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for (i = 0; i < n; i++)
        {
            BYTE        r = ape[i].peRed;

            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);
    return ddpal;
}




//-----------------------------------------------------------------------------
// Name: DDColorMatch()
// Desc: Convert a RGB color to a pysical color.
//       We do this by leting GDI SetPixel() do the color matching
//       then we lock the memory and see what it got mapped to.
//-----------------------------------------------------------------------------
extern "C" DWORD
DDColorMatch(IDirectDrawSurface7 * pdds, COLORREF rgb)
{
    COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hres;

    //
    //  Use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->ReleaseDC(hdc);
    }
    //
    // Now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask it to bpp
        pdds->Unlock(NULL);
    }
    //
    //  Now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}


//-----------------------------------------------------------------------------
// Name: DDSetColorKey()
// Desc: Set a color key for a surface, given a RGB.
//       If you pass CLR_INVALID as the color key, the pixel
//       in the upper-left corner will be used.
//-----------------------------------------------------------------------------
extern "C" HRESULT
DDSetColorKey(IDirectDrawSurface7 * pdds, COLORREF rgb)
{
    DDCOLORKEY              ddck;

    ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}


