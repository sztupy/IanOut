#include "palette.h"
//#include "done.h"
#include <limits>
#include "engine.h"

int rsz, gsz, bsz; 	//bitsize of field
int rsh, gsh, bsh;	//0’s on left (the shift value)

int GetMaskInfo (DWORD Bitmask, int* lpShift)
{
    int Precision, Shift;

    Precision = Shift = 0;
    //count the zeros on right hand side
    while (!(Bitmask & 0x01L))
    {
        Bitmask >>= 1;
        Shift++;
    }
	
    //count the ones on right hand side
    while (Bitmask & 0x01L)
    {
        Bitmask >>= 1;
        Precision++;
    }
    *lpShift = Shift;
    return Precision;
}

void palette::Calculate16bit()
{

    DDPIXELFORMAT DDpf;
	DWORD r,g,b;

    ZeroMemory (&DDpf, sizeof(DDpf));
    DDpf.dwSize = sizeof(DDpf);
    g_pDDSPrimary->GetPixelFormat(&DDpf);
	
    rsz = GetMaskInfo (DDpf.dwRBitMask, &rsh);
    gsz = GetMaskInfo (DDpf.dwGBitMask, &gsh);
    bsz = GetMaskInfo (DDpf.dwBBitMask, &bsh);

	int i;
	for (i=0; i<256; i++) {
		r = pe[i].peRed;
		g = pe[i].peGreen;
		b = pe[i].peBlue;

		r >>= (8-rsz);	//keep only the MSB bits of component
		g >>= (8-gsz);
		b >>= (8-bsz);

		r <<= rsh;	//SHIFT THEM INTO PLACE
		g <<= gsh;
		b <<= bsh;

		palcal[i] = (DWORD)(r | g | b);
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
		
		if (Red>(int)(DDGammaOld.red[i]*gamma)) Red = (int)(DDGammaOld.red[i]*gamma);
		if (Blue>(int)(DDGammaOld.blue[i]*gamma)) Blue = (int)(DDGammaOld.blue[i]*gamma);
		if (Green>(int)(DDGammaOld.green[i]*gamma)) Green = (int)(DDGammaOld.green[i]*gamma);

		if (Blue>65535) Blue=65535;
		if (Red>65535) Red=65535;
		if (Green>65535) Green=65535;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		DDGammaRamp.blue[i] = Blue;
		DDGammaRamp.red[i] = Red;
		DDGammaRamp.green[i] = Green;
	}
	
	lpDDGammaControl->SetGammaRamp(0, &DDGammaRamp);
}

void palette::FadeOut()
{
	for (int i=65535; i>0; i-=3000)
	{
		SetPaletteBl(i);
	}

}

void palette::FadeIn()
{
	for (int i=0; i<65535; i+=3000)
	{
		SetPaletteBl(i);
	}
}


void palette::SetPaletteNo()
{
	lpDDGammaControl->SetGammaRamp(0, &DDGammaRamp);
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
		
		if (Red<(int)(DDGammaOld.red[i]*gamma)) Red = (int)(DDGammaOld.red[i]*gamma);
		if (Blue<(int)(DDGammaOld.blue[i]*gamma)) Blue = (int)(DDGammaOld.blue[i]*gamma);
		if (Green<(int)(DDGammaOld.green[i]*gamma)) Green = (int)(DDGammaOld.green[i]*gamma);

		if (Blue>65535) Blue=65535;
		if (Red>65535) Red=65535;
		if (Green>65535) Green=65535;
		if (Blue<0) Blue=0;
		if (Red<0) Red=0;
		if (Green<0) Green=0;

		DDGammaRamp.blue[i] = Blue;
		DDGammaRamp.red[i] = Red;
		DDGammaRamp.green[i] = Green;
	}
	
	lpDDGammaControl->SetGammaRamp(0, &DDGammaRamp);
}

void palette::FadeOutWh()
{
	for (int i=65535; i>0; i-=3000)
	{
		SetPaletteWh(i);
	}

}

void palette::FadeInWh()
{
	for (int i=0; i<65535; i+=3000)
	{
		SetPaletteWh(i);
	}
}
