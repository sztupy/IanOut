#include "palette.h"
#include <limits>
#include "engine.h"

#define PAL_ANIM_SPEED 2500

void palette::SetGamma128()
{
	for (int i=0; i<256; i++) {
		rramp2[i]=i*255;
		gramp2[i]=i*255;
		bramp2[i]=i*255;

		rramp[i]=i*255;
		gramp[i]=i*255;
		bramp[i]=i*255;
	}
	SDL_SetGammaRamp(rramp,gramp,bramp);
}

void palette::Calculate16bit()
{
	AddToLog(4,"Palette> Calculating 8bit conversion table");
	int i;
	for (i=0; i<256; i++) {
		palcal[i] = SDL_MapRGB(g_pDDSBack->format,pe[i].r,pe[i].g,pe[i].b);
	}
}

void palette::Calculate()
{
}

void palette::SetPaletteBl(int Frame)
{
	int i;
	int Blue;
	int Green;
	int Red;
	if (Frame>65535) Frame=65535;
	if (Frame<0) Frame=0;
	for ( i = 0; i<=255; i++)
	{
		Blue = Frame;
		Green = Frame;
		Red = Frame;
		
		if (Red>(int)(rramp2[i]*gamma)) Red = (int)(rramp2[i]*gamma);
		if (Blue>(int)(bramp2[i]*gamma)) Blue = (int)(gramp2[i]*gamma);
		if (Green>(int)(gramp2[i]*gamma)) Green = (int)(bramp2[i]*gamma);

		if (Blue>65535) Blue=65535;
		if (Red>65535) Red=65535;
		if (Green>65535) Green=65535;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		bramp[i] = Blue;
		rramp[i] = Red;
		gramp[i] = Green;
	}
	SDL_SetGammaRamp(rramp,gramp,bramp);

}

void palette::FadeOut()
{
	AddToLog(5,"Palette> FadeOut");
	for (int i=65535; i>-PAL_ANIM_SPEED; i-=PAL_ANIM_SPEED)
	{
		SetPaletteBl(i);
	}
	ClearSurface(g_pDDSBack,0,0,0);
	SDL_Flip(g_pDDSBack);

}

void palette::FadeIn()
{
	AddToLog(5,"Palette> FadeIn");
	for (int i=0; i<65535+PAL_ANIM_SPEED; i+=PAL_ANIM_SPEED)
	{
		SetPaletteBl(i);
	}
}


void palette::SetPaletteNo()
{
	SDL_SetGammaRamp(rramp,gramp,bramp);
}


void palette::SetPaletteWh(int Frame)
{
int i;
	int Blue;
	int Green;
	int Red;
	if (Frame>65535) Frame=65535;
	if (Frame<0) Frame=0;
	for ( i = 0; i<=255; i++)
	{
		Blue = 65535-Frame;
		Green = 65535-Frame;
		Red = 65535-Frame;
		
		if (Red<(int)(rramp2[i]*gamma)) Red = (int)(rramp2[i]*gamma);
		if (Blue<(int)(bramp2[i]*gamma)) Blue = (int)(gramp2[i]*gamma);
		if (Green<(int)(gramp2[i]*gamma)) Green = (int)(bramp2[i]*gamma);

		if (Blue>65535) Blue=65535;
		if (Red>65535) Red=65535;
		if (Green>65535) Green=65535;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		bramp[i] = Blue;
		rramp[i] = Red;
		gramp[i] = Green;
	}
	SDL_SetGammaRamp(rramp,gramp,bramp);
}

void palette::FadeOutWh()
{
	AddToLog(5,"Palette> FadeOut to White");
	for (int i=65535; i>-PAL_ANIM_SPEED; i-=PAL_ANIM_SPEED)
	{
		SetPaletteWh(i);
	}
	ClearSurface(g_pDDSBack,255,255,255);
	SDL_Flip(g_pDDSBack);

}

void palette::FadeInWh()
{
	AddToLog(5,"Palette> FadeIn from White");
	for (int i=0; i<65535+PAL_ANIM_SPEED; i+=PAL_ANIM_SPEED)
	{
		SetPaletteWh(i);
	}
}
