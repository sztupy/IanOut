/*
    SDL_stretch - Stretch Functions For The Simple DirectMedia Layer
    Copyright (C) 2003 Guido Draheim

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Guido Draheim, guidod@gmx.de
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: sdlcode.c $";
#endif

#include "sdl.h"
#include "SDL_stretchcode.h"

#if (defined(WIN32) && !defined(_M_ALPHA) && !defined(_WIN32_WCE)) || \
    defined(i386) && defined(__GNUC__) /* && defined(USE_ASMBLIT) */

/*
 * 0xAC  LOADSB   | 0xAD  LOADSW  | 0x46  INC SI     | 0x66  PREFIX16    
 * 0xAA  STOSB    | 0xAB  STOSW   | 0x91  XCHG AX,CX 
 * 0x51  PUSH CX  | 0x59  POP CX  | 0xC3 RET
 * does XCHG yield a fast register renaming on P >= P3 ?
 */
# define PREP_1BYTE(dst)
# define PREP_2BYTE(dst)
# define PREP_3BYTE(dst)    { *dst++ = 0x51; }
# define PREP_4BYTE(dst)
# define LOAD_1BYTE(dst)    {                *dst++ = 0xAC; }
# define LOAD_2BYTe(dst)    { *dst++ = 0x66; *dst++ = 0xAD; }
# define LOAD_3BYTE(dst)    {                *dst++ = 0xAC; *dst++ = 0x91; \
                              *dst++ = 0x66; *dst++ = 0xAD; }
# define LOAD_4BYTE(dst)    {                *dst++ = 0xAD; }
# define STORE_1BYTE(dst)   {                *dst++ = 0xAA; }
# define STORE_2BYTe(dst)   { *dst++ = 0x66; *dst++ = 0xAB; }
# define STORE_3BYTE(dst)   { *dst++ = 0x91; *dst++ = 0xAA; *dst++ = 0x91; \
                              *dst++ = 0x66; *dst++ = 0xAB; }
# define STORE_4BYTE(dst)   {                *dst++ = 0xAB; }
# define RETURN_1BYTE(dst)  {                *dst++ = 0xC3; }
# define RETURN_2BYTE(dst)  {                *dst++ = 0xC3; }
# define RETURN_3BYTE(dst)  { *dst++ = 0x59; *dst++ = 0xC3; }
# define RETURN_4BYTE(dst)  {                *dst++ = 0xC3; }
# define SKIP_1BYTE(dst)    {                *dst++ = 0x46; }
# define SKIP_2BYTe(dst)    { *dst++ = 0x46; *dst++ = 0x46; }
# define SKIP_3BYTE(dst)    { *dst++ = 0x46; *dst++ = 0x46; *dst++ = 0x46; }
#  ifndef __GNUC__
#  define LOAD_2BYTE(dst)  LOAD_2BYTe(dst)
#  define STORE_2BYTE(dst) STORE_2BYTe(dst)
#  else
#  define LOAD_2BYTE(dst)    { *((unsigned short*)(dst))++ = 0xAD66; }
#  define SKIP_2BYTE(dst)    { *((unsigned short*)(dst))++ = 0x4646; }
#  define STORE_2BYTE(dst)   { *((unsigned short*)(dst))++ = 0xAB66; }
#  endif

#ifndef SKIP_1BYTE
#define SKIP_1BYTE(dst) LOAD_1BYTE(dst)
#endif
#ifndef SKIP_2BYTE
#define SKIP_2BYTE(dst) LOAD_2BYTE(dst)
#endif
#ifndef SKIP_3BYTE
#define SKIP_3BYTE(dst) LOAD_3BYTE(dst)
#endif
#ifndef SKIP_4BYTE
#define SKIP_4BYTE(dst) LOAD_4BYTE(dst)
#endif

/* ASM i386 */
#define USE_ASM_STRETCH 1
#else
#error Need assembly opcodes for this architecture
#endif

#if !defined USE_ASM_STRETCH
unsigned char SDL_TheRowStretchCode[1];

unsigned char* SDL_SetRowStretchCode(int src_w, int dst_w, int bpp)
{
    return 0;
}
unsigned char* SDL_PutRowStretchCode(unsigned char* buf, int buf_len,
				     int src_w, int dst_w, int bpp)
{
    return 0;
}

void           SDL_RunRowStretchCode(unsigned char* buffer,
				     unsigned char* src,
				     unsigned char* dst)
{   
    return; 
}

#else /* we have some ASM for stretching... -------------------------  */

unsigned char SDL_TheRowStretchCode[SDL_STRETCHCODE_BUFFERSIZE];

unsigned char* SDL_SetRowStretchCode (int src_w, int dst_w, int bpp)
{
    return SDL_PutRowStretchCode(SDL_TheRowStretchCode, 
				 sizeof(SDL_TheRowStretchCode),
				 src_w, dst_w, bpp);
}

unsigned char* SDL_PutRowStretchCode(unsigned char* buf, int buf_len,
				  int src_w, int dst_w, int bpp)
{
    static struct {
	int bpp;
	int src_w;
	int dst_w;
    } last;

    int i = 0; int w = dst_w; int src = 0;
    unsigned char *buf_end = buf + buf_len;
    
    if (last.src_w == src_w && last.dst_w == dst_w && last.bpp == bpp) 
    {   /* so, we do not need to regenerate the code buffer */
	return 0;
    }else{
	last.src_w = src_w; last.dst_w = dst_w; last.bpp = bpp;
    }

    switch (bpp) 
    {
    case 4:
	PREP_4BYTE(buf);
	while(src<src_w) {
	    LOAD_4BYTE(buf);
	draw4:
	    STORE_4BYTE(buf);  
	    --w; i += src_w;
	    if (! w) break;
	    if (buf >= buf_end) goto overflow;
	    if (i < dst_w) goto draw4;
	    i -= dst_w; src++;
	    while (i >= dst_w) { i -= dst_w; src++; SKIP_4BYTE(buf); }
	}; 
	RETURN_4BYTE(buf);
	break;
    case 3:
	PREP_3BYTE(buf);
	while(src<src_w) {
	    LOAD_3BYTE(buf);
	draw3:
	    STORE_3BYTE(buf);
	    --w; i += src_w;
	    if (! w) break;
	    if (buf >= buf_end) goto overflow;
	    if (i < dst_w) goto draw3;
	    i -= dst_w; src++;
	    while (i >= dst_w) { i -= dst_w; src++; SKIP_3BYTE(buf); }
	}; 
	RETURN_3BYTE(buf);
	break;
    case 2:
	PREP_2BYTE(buf);
	while(src<src_w) {
	    LOAD_2BYTE(buf);
	draw2:
	    STORE_2BYTE(buf);
	    --w; i += src_w;
	    if (! w) break;
	    if (buf >= buf_end) goto overflow;
	    if (i < dst_w) goto draw2;
	    i -= dst_w; src++;
	    while (i >= dst_w) { i -= dst_w; src++; SKIP_2BYTE(buf); }
	}; 
	RETURN_2BYTE(buf);
	break;
    case 1:
	PREP_1BYTE(buf);
	while(src<src_w) {
	    LOAD_1BYTE(buf);
	draw1:
	    STORE_1BYTE(buf);
	    --w; i += src_w;
	    if (! w) break;
	    if (buf >= buf_end) goto overflow;
	    if (i < dst_w) goto draw1;
	    i -= dst_w; src++;
	    while (i >= dst_w) { i -= dst_w; src++; SKIP_1BYTE(buf); }
	}; 
	RETURN_1BYTE(buf);
	break;
    default:
	SDL_SetError("ASM stretch of %d bytes isn't supported\n", bpp);
	return 0;
    }
#   if 0 /* debugging - gives a nice visual representation of the code */
    *buf = 0; buf = SDL_TheRowStretchCode; fprintf (stderr,"\n"); 
    while (*buf) { 
	fprintf(stderr,"%c", (*buf++)&0x7F); 
	if (! (((long)buf)&0x3F)) fprintf (stderr, "\n");
    }   fprintf(stderr,"(%iB=(%i->%i))\n", buf-SDL_TheRowStretchCode,
		src_w, dst_w);
#   endif
    return SDL_TheRowStretchCode;

 overflow:
    /* so, we did overflow - too late anyway */
    SDL_SetError("Copy buffer overflow");
    return 0;
}


void           SDL_RunRowStretchCode(unsigned char* code,
				     unsigned char* srcp,
				     unsigned char* dstp)
{   
#	       if defined __GNUC__
		__asm__ __volatile__ ("call *%%eax"
				      :: "S" (dstp), "D" (srcp), "a" (code));
#              elif defined WIN32
		__asm { 
		    push edi ;
		    push esi ;
		    mov edi, dstp ;
		    mov esi, srcp ;
		    call dword ptr code ;
		    pop esi ;
		    pop edi ;
		}
#              else
#              error Need inline assembly for this compiler
#              endif
}

#endif

#define DEFINE_COPY_ROW(name, type)			 \
void name(type *src, int src_w, type *dst, int dst_w)	 \
{							 \
	int i = 0;  			                 \
 	int w = dst_w;					 \
	type pixel = 0;					 \
        type* src_max = src + src_w;                     \
  						         \
	while(src < src_max) {                           \
	    pixel = *src;                                \
	draw:                                            \
	    *dst++ = pixel;                              \
	    --w; i += src_w;                             \
	    if (! w) break;                              \
	    if (i < dst_w) goto draw;                    \
	    do { i -= dst_w; src++; } while (i >= dst_w);\
	}; return;                                       \
}
DEFINE_COPY_ROW(SDL_StretchRow1, Uint8)
DEFINE_COPY_ROW(SDL_StretchRow2, Uint16)
DEFINE_COPY_ROW(SDL_StretchRow4, Uint32)

/* The ASM code has a hard time to handle 24-bpp stretch blits */
void SDL_StretchRow3(Uint8 *src, int src_w, Uint8 *dst, int dst_w)
{
        int i = 0;
	int w = dst_w;
	Uint8 pixel [3];
        Uint8* src_max = src + src_w;

	while(src < src_max) {
	    pixel[0] = *src++;
	    pixel[1] = *src++;
	    pixel[2] = *src++;
	draw:
	    *dst++ = pixel[0];
	    *dst++ = pixel[1];
	    *dst++ = pixel[2];
	    --w ; i += src_w;
	    if (! w) break;                            
	    if (i < dst_w) goto draw; /* draw same pixel again */
	    do { i -= dst_w; src+=3; } while (i >= dst_w);
	}; return;
}
