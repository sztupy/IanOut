#include "io.h"
#include "discutil.h"
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include "sdl.h"

extern "C" int atoi2(const char * str)
{
	if (str) {
		return atoi(str);
	} else return 0;
}

SDL_Event event;

extern "C" int
LogFail(int hRet, const char* szError,...)
{
	char                        szBuff[128];
	char						szBuff2[128];
	char						szBuff3[50];
    va_list                     vl;
	va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
	sprintf(szBuff3,SDL_GetError());
	sprintf(szBuff2,"%s (%s)",szBuff,szBuff3);
    AddToLog(0, "Error> %s", szBuff2);
	va_end(vl);
	
	event.type = SDL_QUIT;
	event.quit.type = SDL_QUIT;
	SDL_PushEvent(&event);
	return -1;
}

extern "C" int
InitFail(int hRet, const char* szError,...)
{
	char                        szBuff[128];
	char						szBuff2[128];
	char						szBuff3[50];
    va_list                     vl;
 
    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
	sprintf(szBuff3,SDL_GetError());
	sprintf(szBuff2,"%s (%s)",szBuff,szBuff3);
    AddToLog(0, "Error> %s", szBuff2);
    
	event.type = SDL_QUIT;
	event.quit.type = SDL_QUIT;
	SDL_PushEvent(&event);

    va_end(vl);
    return -1;
}