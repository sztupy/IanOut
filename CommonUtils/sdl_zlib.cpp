#include <SDL_zlib.h>
#include "datextract.h"

/* MSVC can not take a casted variable as an lvalue ! */
#define SDL_RWOPS_ZZIP_DATA(_context) \
             ((_context)->hidden.unknown.data1)
#define SDL_RWOPS_ZZIP_FILE(_context)  (gzFile) \
             ((_context)->hidden.unknown.data1)

static int _zzip_seek(SDL_RWops *context, int offset, int whence)
{
    return gzseek(SDL_RWOPS_ZZIP_FILE(context), offset, whence);
}

static int _zzip_read(SDL_RWops *context, void *ptr, int size, int maxnum)
{
    return gzread(SDL_RWOPS_ZZIP_FILE(context), ptr, size*maxnum);
}

static int _zzip_write(SDL_RWops *context, const void *ptr, int size, int num)
{
    return 0; /* ignored */
}

static int _zzip_close(SDL_RWops *context)
{
    if (! context) return 0; /* may be SDL_RWclose is called by atexit */

    gzclose(SDL_RWOPS_ZZIP_FILE(context));
    SDL_FreeRW (context);
    return 0;
}

SDL_RWops *SDL_RWFromGzip(gzFile file)
{
    register SDL_RWops* rwops;
    
    rwops = SDL_AllocRW ();
    if (! rwops) { errno=ENOMEM; gzclose(file); return 0; }

    SDL_RWOPS_ZZIP_DATA(rwops) = file;
    rwops->read = _zzip_read;
    rwops->write = _zzip_write;
    rwops->seek = _zzip_seek;
    rwops->close = _zzip_close;
    return rwops;
}