#ifndef _SDL_ZLIB_H
#define _SDL_ZLIB_H

#include "datextract.h"
#include <SDL_rwops.h>

#ifdef __cplusplus
extern "C" {
#endif
SDL_RWops *SDL_RWFromGzip(gzFile file);
#ifdef __cplusplus
}
#endif

#endif