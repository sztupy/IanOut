//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "ddutil.h"
#include "utils.h"
#include "discutil.h"
#include "sdl_stretch.h"

extern int BitDepth;
SDL_Rect ClipRect;
unsigned char DarkMask[256][10];

extern "C"
void CalculateEntries()
{

}

extern "C"
void SetClipRect(int x,int y,int x2,int y2)
{
	ClipRect.h=y2-y;
	ClipRect.x=x;
	ClipRect.w=x2-x;
	ClipRect.y=y;
}

extern "C"
int BlitToAlt(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, int cback)
{
	if (cback<0) return 0;//LogFail(0,"AltDraw");;
	if (cback>9) return 0;//LogFail(0,"AltDraw");

	Sint32 r,g,b;

	Uint32						dwTargetPad;
	Uint32						dwSourcePad;
	int		                    hRet;
	Uint16*						lpbTarget;
	Uint16*						lpbSource;

	Uint32*						lpbTarget32;
	Uint32*						lpbSource32;

	Uint16						dwSourceTemp;
	Uint32						dwSourceTemp32;

	int							iWidth;
	int							iHeight;

	SDL_Rect					rcRect;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

//	ClipRect.x+=2;
//	ClipRect.y+=2;

//	ClipRect.w-=2;
//	ClipRect.h-=2;

	int i;

	hRet = 0;

    rcRect.x = Sx1;
    rcRect.y = Sy1;
    rcRect.w = Sx2-Sx1;
    rcRect.h = Sy2-Sy1;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.x+ClipRect.w) || (Dy>ClipRect.y+ClipRect.h) || (Dx+rcRect.w<ClipRect.x) || (Dy+rcRect.h<ClipRect.y))) 
	{
		if (Dx+iw1>ClipRect.x+ClipRect.w) {iw1 = iw1-(Dx+iw1-(ClipRect.x+ClipRect.w));};
		if (Dy+iw2>ClipRect.y+ClipRect.h) {iw2 = iw2-(Dy+iw2-(ClipRect.y+ClipRect.h));};
		
		Sx2 = Sx1+iw1;
		Sy2 = Sy1+iw2;

		if (Dx<ClipRect.x) { Sx1+=(-Dx+ClipRect.x);BltX=ClipRect.x;};
		if (Dy<ClipRect.y) { Sy1+=(-Dy+ClipRect.y);BltY=ClipRect.y;};

		rcRect.x = Sx1;
		rcRect.y = Sy1;
		rcRect.w = Sx2-Sx1;
		rcRect.h = Sy2-Sy1;

		iw1 = Sx2-Sx1; iw2 = Sy2-Sy1;

		if ((rcRect.w != 0) && (rcRect.h != 0)) {
			SDL_LockSurface(Blit);
			SDL_LockSurface(surf);
			iWidth = iw1;
			iHeight = iw2;

			int iWidthT = iw1;
			int iHeightT = iw2;

			dwTargetPad = Blit->pitch/(BitDepth==32?4:2) - ( iWidthT );
			dwSourcePad = surf->pitch/(BitDepth==32?4:2) - ( iWidth );

			if (BitDepth == 32) {
				lpbTarget32 = ( Uint32* )Blit->pixels;
				lpbTarget32 += ((BltY)*Blit->pitch/4+BltX);
				lpbSource32 = ( Uint32* ) surf->pixels;
				lpbSource32 += ((Sy1)*surf->pitch/4+Sx1);
			} else {
				lpbTarget = ( Uint16* )Blit->pixels;
				lpbTarget += ((BltY)*Blit->pitch/2+BltX);
				lpbSource = ( Uint16* ) surf->pixels;
				lpbSource += ((Sy1)*surf->pitch/2+Sx1);
			}

			int ScX, ScY;
			ScX = BltX;
			ScY = BltY;

			do {
				// Reset the width.
				i = iWidth;
				while ( i-- > 0 )
				{
					if (BitDepth==32) {
						dwSourceTemp32 = *( ( Uint32* ) lpbSource32 );
						if ( dwSourceTemp32 != 0 )
						{
							r = (dwSourceTemp32 & Blit->format->Rmask) >> Blit->format->Rshift;
							g = (dwSourceTemp32 & Blit->format->Gmask) >> Blit->format->Gshift;
							b = (dwSourceTemp32 & Blit->format->Bmask) >> Blit->format->Bshift;

							r -= (9-cback)<<3;if (r<0) r=0;
							g -= (9-cback)<<3;if (g<0) g=0;
							b -= (9-cback)<<3;if (b<0) b=0;

							r <<= Blit->format->Rshift;
							g <<= Blit->format->Gshift;
							b <<= Blit->format->Bshift;

 							*(  lpbTarget32 ) = (Uint32)((Uint32)r|(Uint32)g|(Uint32)b);
						} 
						lpbTarget32 += 1;
						lpbSource32 += 1;
					} else {
						dwSourceTemp = *( ( Uint16* ) lpbSource );
						if ( dwSourceTemp != 0 )
						{
							r = (dwSourceTemp & Blit->format->Rmask) >> Blit->format->Rshift;
							g = (dwSourceTemp & Blit->format->Gmask) >> Blit->format->Gshift;
							b = (dwSourceTemp & Blit->format->Bmask) >> Blit->format->Bshift;

							r -= (9-cback);if (r<0) r=0;
							g -= (9-cback)<<(Blit->format->Rloss==Blit->format->Gloss?0:1);if (g<0) g=0;
							b -= (9-cback);if (b<0) b=0;

							r <<= Blit->format->Rshift;
							g <<= Blit->format->Gshift;
							b <<= Blit->format->Bshift;
 							*(  lpbTarget ) = (Uint16)((Uint16)r|(Uint16)g|(Uint16)b);
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
		SDL_UnlockSurface(surf);
		SDL_UnlockSurface(Blit);
	}
	}
	// Return the result.
	return hRet;
}

extern "C"
int BlitToRo(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, Uint32 cback)
{
	//if (cback<0) return LogFail(DDERR_INVALIDPARAMS,"RoDraw");;
	//if (cback>9) return LogFail(DDERR_INVALIDPARAMS,"AltDraw");
	Uint32						dwTargetPad;
	Uint32						dwSourcePad;
	int		                    hRet;
	Uint16*						lpbTarget;
	Uint16*						lpbSource;

	Uint32*						lpbTarget32;
	Uint32*						lpbSource32;

	Uint16						dwSourceTemp;
	Uint32						dwSourceTemp32;

	int							iWidth;
	int							iHeight;

	SDL_Rect					rcRect;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	ClipRect.x+=2;
	ClipRect.y+=2;

	ClipRect.w-=2;
	ClipRect.h-=2;

	int i;

	hRet = 0;

    rcRect.x = Sx1;
    rcRect.y = Sy1;
    rcRect.w = Sx2-Sx1;
    rcRect.h = Sy2-Sy1;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.x+ClipRect.w) || (Dy>ClipRect.y+ClipRect.h) || (Dx+rcRect.w<ClipRect.x) || (Dy+rcRect.h<ClipRect.y))) 
	{
		if (Dx+iw1>ClipRect.x+ClipRect.w) {iw1 = iw1-(Dx+iw1-(ClipRect.x+ClipRect.w));};
		if (Dy+iw2>ClipRect.y+ClipRect.h) {iw2 = iw2-(Dy+iw2-(ClipRect.y+ClipRect.h));};
		
		Sx2 = Sx1+iw1;
		Sy2 = Sy1+iw2;

		if (Dx<ClipRect.x) { Sx1+=(-Dx+ClipRect.x);BltX=ClipRect.x;};
		if (Dy<ClipRect.y) { Sy1+=(-Dy+ClipRect.y);BltY=ClipRect.y;};

		rcRect.x = Sx1;
		rcRect.y = Sy1;
		rcRect.w = Sx2-Sx1;
		rcRect.h = Sy2-Sy1;

		iw1 = Sx2-Sx1; iw2 = Sy2-Sy1;

		if ((rcRect.w != 0) && (rcRect.h != 0)) {
			SDL_LockSurface(Blit);
			SDL_LockSurface(surf);
			iWidth = iw1;
			iHeight = iw2;

			int iWidthT = iw1;
			int iHeightT = iw2;

			dwTargetPad = Blit->pitch/(BitDepth==32?4:2) - ( iWidthT );
			dwSourcePad = surf->pitch/(BitDepth==32?4:2) - ( iWidth );

			if (BitDepth == 32) {
				lpbTarget32 = ( Uint32* )Blit->pixels;
				lpbTarget32 += ((BltY)*Blit->pitch/4+BltX);
				lpbSource32 = ( Uint32* ) surf->pixels;
				lpbSource32 += ((Sy1)*surf->pitch/4+Sx1);
			} else {
				lpbTarget = ( Uint16* )Blit->pixels;
				lpbTarget += ((BltY)*Blit->pitch/2+BltX);
				lpbSource = ( Uint16* ) surf->pixels;
				lpbSource += ((Sy1)*surf->pitch/2+Sx1);
			}

			int ScX, ScY;
			ScX = BltX;
			ScY = BltY;

			bool istop,isleft,isright,isbottom;
			istop = true; isleft = true; isright = false; isbottom = false;

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
								
								if (((!isleft) && ((*(lpbSource32-1))==0)) || ((isleft))) {
									*(lpbTarget32-1) = cback;
								}
								if (((!isright) && ((*(lpbSource32+1))==0)) || ((isright))) {
									*(lpbTarget32+1) = cback;
									
								}
								if (((!istop) && ((*(lpbSource32-surf->pitch/4))==0)) || ((istop))) {
									*(lpbTarget32-Blit->pitch/4) = cback;	
								}
								if (((!isbottom) && ((*(lpbSource32+surf->pitch/4))==0)) || ((isbottom))) {
									*(lpbTarget32+Blit->pitch/4) = cback;	
								}					
							} 
							lpbTarget32 += 1;
							lpbSource32 += 1;
					} else {
						dwSourceTemp = *(lpbSource );
						if ( dwSourceTemp != 0 )
							{
								
								if (((!isleft) && ((*(lpbSource-1))==0)) || ((isleft))) {
									*(lpbTarget-1) = (Uint16)cback;
								}
								if (((!isright) && ((*(lpbSource+1))==0)) || ((isright))) {
									*(lpbTarget+1) = (Uint16)cback;
									
								}
								if (((!istop) && ((*(lpbSource-surf->pitch/2))==0)) || ((istop))) {
									*(lpbTarget-Blit->pitch/2) = (Uint16)cback;	
								}
								if (((!isbottom) && ((*(lpbSource+surf->pitch/2))==0)) || ((isbottom))) {
									*(lpbTarget+Blit->pitch/2) = (Uint16)cback;	
								}					
							} 
							lpbTarget += 1;
							lpbSource += 1;
					}
					ScX++;
					isleft=false;
				} 
				if (BitDepth==32) {
					lpbTarget32 += dwTargetPad;
					lpbSource32 += dwSourcePad;
				} else {
					lpbTarget += dwTargetPad;
					lpbSource += dwSourcePad;
				}
				istop=false;
				ScX = BltX;
				ScY++;
			} while ( --iHeight > 0 );

			SDL_UnlockSurface(surf);
			SDL_UnlockSurface(Blit);
			
		}

	}

	ClipRect.x-=2;
	ClipRect.y-=2;

	ClipRect.w+=2;
	ClipRect.h+=2;

	// Return the result.
	return hRet;
}

extern "C"
int BlitTo(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf)
{
    SDL_Rect					rcRect;
	SDL_Rect					rcDestRect;
	int							hRet;
	int							iw1;
	int							iw2;
	int							BltX;
	int							BltY;

	hRet = 0;

    rcRect.x = Sx1;
    rcRect.y = Sy1;
    rcRect.w = Sx2-Sx1;
    rcRect.h = Sy2-Sy1;

    iw1 = Sx2-Sx1;
	iw2 = Sy2-Sy1;

	BltX = Dx;
	BltY = Dy;

	if (!((Dx>ClipRect.x+ClipRect.w) || (Dy>ClipRect.y+ClipRect.h) || (Dx+rcRect.w<ClipRect.x) || (Dy+rcRect.h<ClipRect.y))) 
	{
		if (Dx+iw1>ClipRect.x+ClipRect.w) {iw1 = iw1-(Dx+iw1-(ClipRect.x+ClipRect.w));};
		if (Dy+iw2>ClipRect.y+ClipRect.h) {iw2 = iw2-(Dy+iw2-(ClipRect.y+ClipRect.h));};
		
		Sx2 = Sx1+iw1;
		Sy2 = Sy1+iw2;

		if (Dx<ClipRect.x) { Sx1+=(-Dx+ClipRect.x);BltX=ClipRect.x;};
		if (Dy<ClipRect.y) { Sy1+=(-Dy+ClipRect.y);BltY=ClipRect.y;};

		rcRect.x = Sx1;
		rcRect.y = Sy1;
		rcRect.w = iw1;
		rcRect.h = iw2;

		rcDestRect.x = BltX;
		rcDestRect.y = BltY;
		rcDestRect.w = iw1;
		rcDestRect.h = iw2;

	if ((rcRect.w != 0) && (rcRect.h != 0)) {
		hRet = SDL_BlitSurface(surf,&rcRect,Blit,&rcDestRect);
	}

	if (hRet != 0)
        return LogFail(hRet,"Error while drawing");
	}

	//SDL_UpdateRect(Blit, rcDestRect.x, rcDestRect.y, rcDestRect.w, rcDestRect.h);

	return hRet;
}


extern "C"
int BlitToStretch(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, int Dx2, int Dy2, Uint32 flags, SDL_Surface* surf/*, SDL_Surface* temp*/)
{
	SDL_Rect					rcRect;
	SDL_Rect					rcRectTarget;
	SDL_Rect					temprect;

    rcRect.x = Sx1;
    rcRect.y = Sy1;
    rcRect.w = Sx2-Sx1;
    rcRect.h = Sy2-Sy1;

	rcRectTarget.x = Dx;
    rcRectTarget.y = Dy;
    rcRectTarget.w = Dx2-Dx;
    rcRectTarget.h = Dy2-Dy;

	SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, rcRectTarget.w,rcRectTarget.h, Blit->format->BitsPerPixel, Blit->format->Rmask, Blit->format->Gmask,Blit->format->Bmask,Blit->format->Amask);

	SDL_LockSurface(temp);
	SDL_LockSurface(surf);

	SDL_StretchSurfaceBlitTo(surf,&rcRect,temp,NULL);

	SDL_UnlockSurface(surf);
	SDL_UnlockSurface(temp);

	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, 0);

	SDL_BlitSurface(temp,NULL,Blit,&rcRectTarget);
    SDL_FreeSurface(temp);
	
	return 0;
}

extern "C"
int	BlitToAlpha(SDL_Surface* Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, SDL_Surface* surf, int alpha)
{
	SDL_Rect					rcRect;
	SDL_Rect					rcRectTarget;
	SDL_Rect					temprect;

    rcRect.x = Sx1;
    rcRect.y = Sy1;
    rcRect.w = Sx2-Sx1;
    rcRect.h = Sy2-Sy1;

	rcRectTarget.x = Dx;
    rcRectTarget.y = Dy;
    rcRectTarget.w = Sx2-Sx1;
    rcRectTarget.h = Sy2-Sy1;

	if (alpha==255) {
		SDL_BlitSurface(surf,&rcRect,Blit,&rcRectTarget);
	} else if (alpha!=0) {
		SDL_SetAlpha(surf,SDL_SRCALPHA,alpha);
		SDL_BlitSurface(surf,&rcRect,Blit,&rcRectTarget);
		SDL_SetAlpha(surf,0,255);
	}
	
	return 0;
}
/*
extern "C"
HRESULT BlitToMask(LPDIRECTDRAWSURFACE7 Blit,int Sx1, int Sy1, int Sx2, int Sy2, int Dx, int Dy, Uint32 flags, LPDIRECTDRAWSURFACE7 surf, HBITMAP mask)
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

		  MaskBlt(HDCBlit,BltX,BltY,iw1,iw2,HDCSurf,Sx1,Sy1,mask,0,0,SRCCOPY);
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
*/

extern "C"
void ClearSurface(SDL_Surface* Surf, int r, int g, int b)
{
	int hRet = SDL_FillRect(Surf, NULL, SDL_MapRGB(Surf->format,r,g,b));

	if (hRet != 0) LogFail(hRet,"ClearSurface failed");

	//SDL_UpdateRect(Surf, 0,0,0,0);
}

extern "C"
void ClearRect(SDL_Surface* Surf, int Sx1, int Sy1, int Sx2, int Sy2, int c)
{
	SDL_Rect rcRect;
	rcRect.x = Sx1;
	rcRect.y = Sy1;
	rcRect.w = Sx2-Sx1;
	rcRect.h = Sy2-Sy1;
	int hRet = SDL_FillRect(Surf, &rcRect, c);
	if (hRet != 0) LogFail(hRet,"ClearSurface failed");
//	SDL_UpdateRect(Surf, rcRect.x,rcRect.y,rcRect.w,rcRect.h);
}