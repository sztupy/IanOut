#ifndef __GLOBAL_H
#define __GLOBAL_H

#pragma warning (disable : 4244)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NAME                "IanOut"
#define TITLE               "IanOut: A Post-Nuclear Ian-Playing Game"

#define VERSIONNUM	        "0.80 - Public Beta 3"

#include "../FRMObject/player.h"
#include "../commonutils/ddutil.h"
#include "../commonutils/utils.h"
#include "../commonutils/discutil.h"
#include "resource.h"

HRESULT BlitFRMTo(LPDIRECTDRAWSURFACE7 Target,PFRM Source,int x, int y,DWORD flags,signed char xAl,signed char yAl);

#endif //__GLOBAL_H