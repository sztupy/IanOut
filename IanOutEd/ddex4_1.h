

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>
#include <stdarg.h>
#include <afx.h>
#include <io.h> 
#include <fcntl.h>      
#include "itemobj.h"
#include "resource.h"
#include "ddutil.h"
#include "FRMGraph.h"
#include "utils.h"
#include <time.h>

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "IanEd"
#define TITLE               "IanOut Editor"

#define VERSIONNUM	        "0.1"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
const int					FontMap[] = {
// * *_* * * *!* *"* *#* *$* *%* *&* *'* *(* *)* *** *+* *,* *-* *.* */* *0* *1* *2* *3* *4* *5* *6* *7* *8* *9* *:* *;* *<* *=* *>* *?* *@* *A* *B* *C* *D* *E* *F* *G* *H* *I* *J* *K* *L* *M* *N* *O* *P* *Q* *R* *S* *T* *U* *V* *W* *X* *Y* *Z* *[* *\* *]* *^* *_* *`* *a* *b* *c* *d* *e* *f* *g* *h* *i* *j* *k* *l* *m* *n* *o* *p* *q* *r* *s* *t* *u* *v* *w* *x* *y* *z* *{* *|* *}* *~* *o*
	  0,  6,  10, 17, 26, 34, 44, 53, 58, 64, 70, 77, 84, 88, 96,100,110,118,124,132,140,148,156,164,172,180,188,192,196,205,213,222,230,239,249,258,266,275,283,291,300,309,313,322,331,339,348,357,366,375,384,393,402,410,419,429,438,448,458,467,473,483,489,498,507,512,520,528,535,543,551,558,566,574,578,585,593,597,606,614,622,630,638,645,653,661,669,677,686,694,702,710,716,720,726,735,742
};

const int			    WallMap[][2] = { {0,-120},{0,-120},{0,-120},{0,-120},{-22,-120},{0,0},{0,0},{0,-120},{0,-120},{0,-120},{0,-120},{0,-120},{0,-120},{0,-98},{0,-120},
/*b*/  								   {0,0},{0,-108},{0,-100},{0,-108},{0,-100},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,-108},{0,-100},{0,-108},{0,-117},{0,-76 },{0,-84 },{0,-76 },{0,-84 },
/*b*/								   {0,-76 },{0,-84 },{0,-76 },{0,-84 },{0,-84 },{0,-84 },{0,-84 },{0,-84 },{0,-80}
};

const int			    ItemMap[][2] = { {3,-13},{-30,-95},{-11,-60},{2,-38},{-36,-70},{-11,-63},{0,-95},{-4,-28},{-13,-68},{-18,-88},{-22,-71},{9,-119},{6,-119},{6,-79}
};

const int				Calculate[8] = {6,3,0,5,2,7,4,1};


DWORD                   dwFrameCount;
DWORD                   dwFrameTime;
DWORD                   dwFrames;
DWORD                   dwFramesLast;

// Name of our bitmap resource.
static char                 szBitmap[] = "ALL";

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------


LPDIRECTDRAW7               g_pDD = NULL;			// DirectDraw object
LPDIRECTDRAWSURFACE7        g_pDDSPrimary = NULL;	// DirectDraw primary surface
LPDIRECTDRAWSURFACE7        g_pDDSBack = NULL;		// DirectDraw back surface
LPDIRECTDRAWSURFACE7        g_pDDSOne = NULL;		// Offscreen surface 1

LPDIRECTDRAWPALETTE         g_pDDPal = NULL;		// The primary surface palette
LPVOID						lpSurface  = NULL;
BOOL                        g_bActive = FALSE;		// Is application active?

DIMOUSESTATE				dims;					// DirectInput mouse state structure
DIMOUSESTATE				olddims;				// DirectInput mouse state structure
IDirectInput*				g_pDI = NULL;
IDirectInputDevice*			g_pMouse = NULL;

int							MousX = 320;
int							MousY = 240;

int							TerX = 0; 
int							TerY = 0;

PFRMSingle					FullScreen = NULL;
PFRMSingle					Mouse = NULL;
PFRMSingle					MenuUp = NULL;
PFRMSingle					MenuDown = NULL;
PFRMSingle					Select = NULL;
PFRMSingle					Select2 = NULL;
PFRMSingle					MouseScr[8][2];
PFRMSingle					Buttons[2];

PIanMap						MapInf;
PIanWall					WallInf;
PIanItems					ItemInf;

PALETTEENTRY                pe[256];
PALETTEENTRY                pe2[256];

double						gamma = 1.5;

int							gameSpeed = 40;

HWND                        hWnd;

int							GamePos = 0;

int							wallpos[6][2];
int							current;
BlockType					BlockDat;

TIniStr						*LINIStr[75];

BOOL						dot_plane = FALSE;
BOOL						wall_plane = FALSE;
BOOL						item_plane = FALSE;
BOOL						block_plane = FALSE;

BOOL						show_wall = TRUE;
BOOL						show_item = TRUE;

int							putmode	= 1;
int							Selected = -1;