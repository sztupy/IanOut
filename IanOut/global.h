#ifndef __GLOBAL_H
#define __GLOBAL_H

#pragma warning (disable : 4244)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NAME                "IanOut"
#define TITLE               "IanOut: A Post-Nuclear Ian-Playing Game"

#define VERSIONNUM	        "0.8.5 - Public Beta 4"

#include "../FRMObject/player.h"
#include "../commonutils/ddutil.h"
#include "../commonutils/utils.h"
#include "../commonutils/discutil.h"
#include "resource.h"

int BlitFRMTo(SDL_Surface* Target,PFRM Source,int x, int y,Uint32 flags,signed char xAl,signed char yAl);

#endif //__GLOBAL_H