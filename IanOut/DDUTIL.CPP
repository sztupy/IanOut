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
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <wingdi.h>

extern "C"
HRESULT BlitTo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf)
{
    DDBLTFX                     ddbltfx;
    RECT                        rcRect;
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

	if (!((Dx>640) || (Dy>480) || (Dx+iw1<0) || (Dy+iw2<0))) 
	{
		if (Dx+iw1>640) {iw1 = iw1-(Dx+iw1-640);}
		if (Dy+iw2>480) {iw2 = iw2-(Dy+iw2-480);}
		
	  Sx2 = Sx1+iw1;
	  Sy2 = Sy1+iw2;

	  if (Dx<0) { Sx1+=(-Dx);BltX=0;}
	  if (Dy<0) { Sy1+=(-Dy);BltY=0;}
		
      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;


    while (TRUE)
    {
        hRet = Blit->BltFast(BltX,BltY,surf,&rcRect,flags);

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return hRet;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return hRet;
    }
    if (hRet != DD_OK)
        return hRet;
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

	if (!((Dx>640) || (Dy>480) || (Dx+iw1<0) || (Dy+iw2<0))) 
	{
		if (Dx+iw1>640) {iw1 = iw1-(Dx+iw1-640);}
		if (Dy+iw2>480) {iw2 = iw2-(Dy+iw2-480);}
		
	  Sx2 = Sx1+iw1;
	  Sy2 = Sy1+iw2;

	  if (Dx<0) { Sx1+=(-Dx);BltX=0;}
	  if (Dy<0) { Sy1+=(-Dy);BltY=0;}
		
      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;

    while (TRUE)
    {
        hRet = Blit->GetDC(&HDCBlit);
		if (!FAILED(hRet)) {
		 hRet = surf->GetDC(&HDCSurf);
		 if (!FAILED(hRet)) {
		  iw1 = Sx2-Sx1;
		  iw2 = Sy2-Sy1;
		  BLDF.BlendOp=0; //AC_SRC_OVER;
		  BLDF.BlendFlags=0;
		  BLDF.SourceConstantAlpha=alpha;
		  BLDF.AlphaFormat=0;
		  AlphaBlend(HDCBlit,BltX,BltY,iw1,iw2,HDCSurf,Sx1,Sy1,iw1,iw2,BLDF);
		  
		  hRet = surf->ReleaseDC(HDCSurf);
		 }
		 hRet = Blit->ReleaseDC(HDCBlit);
		}

        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
           // hRet = RestoreAll();if (hRet != DD_OK)
                return hRet;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            return hRet;
    }
    if (hRet != DD_OK)
        return hRet;
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
    Surf->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

/*extern "C"
HRESULT BlitTo(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, DWORD flags, LPDIRECTDRAWSURFACE7 surf)
{
    RECT                        rcRect;
	HRESULT                     hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;
	DDSURFACEDESC2			    ddsd,ddsd2;

	hRet = DD_OK;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

	ZeroMemory(&ddsd2, sizeof(ddsd2));
    ddsd2.dwSize = sizeof(ddsd2);

    rcRect.left = Sx1;
    rcRect.top = Sy1;
    rcRect.right = Sx2;
    rcRect.bottom = Sy2;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>640) || (Dy>480) || (Dx+iw1<0) || (Dy+iw2<0))) 
	{
		if (Dx+iw1>640) {iw1 = iw1-(Dx+iw1-640);}
		if (Dy+iw2>480) {iw2 = iw2-(Dy+iw2-480);}
		
	  Sx2 = Sx1+iw1;
	  Sy2 = Sy1+iw2;

	  if (Dx<0) { Sx1+=(-Dx);BltX=0;}
	  if (Dy<0) { Sy1+=(-Dy);BltY=0;}
		
      rcRect.left = Sx1;
      rcRect.top = Sy1;
      rcRect.right = Sx2;
      rcRect.bottom = Sy2;
 
      LPSTR Buf1,Buf2;

      hRet = Blit->Lock(NULL,&ddsd,0,NULL);
	  hRet = surf->Lock(NULL,&ddsd2,0,NULL);

      Buf1 = (LPSTR)ddsd.lpSurface;
	  Buf2 = (LPSTR)ddsd2.lpSurface;

	  int x,y;
	  int y2;

	  y2=Sy1;

	  Buf1=(LPSTR)ddsd.lpSurface+y*ddsd.lPitch+BltX;
	  Buf2=(LPSTR)ddsd2.lpSurface+y2*ddsd2.lPitch+Sx1;

	 iw1 = Sy2-Sy1;
	 iw2 = Sx2-Sx1;

	  for (y=BltY; y<BltY+iw1; y++)
	  {
		  
		  for (x=BltX; x<BltX+iw2; x++)
		  {
			 (*Buf1)=(*Buf2)+1;
			 Buf1++;
			 Buf2++;
		  }
		  Buf1+=(ddsd.lPitch-iw2);
		  Buf2+=(ddsd2.lPitch-iw2);
		  
	  }

	  hRet = surf->Unlock(NULL);
	  hRet = Blit->Unlock(NULL);
	}  
	return hRet;
}*/

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


