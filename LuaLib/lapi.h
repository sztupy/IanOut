/*
** $Id: lapi.h,v 1.1 2003/09/17 20:51:26 sztupy Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h


#include "lobject.h"


void luaA_pushobject (lua_State *L, const TObject *o);

#endif
