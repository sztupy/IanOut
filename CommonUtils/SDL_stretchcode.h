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

#ifndef _SDL_STRETCHCODE_IMPLEMENTATION_
#define _SDL_STRETCHCODE_IMPLEMENTATION_

/* development header - do not use */

#define SDL_STRETCHCODE_BUFFERSIZE 8192

/**
 * TheRowStretchCode is a shared buffer between Stretch-routines that
 * use no extra buffer-argument. You should call SDL_SetRowStretchCode
 * to fill this buffer and set a "call"-operation for your target
 * cpu to execute this static buffer. That is done for effiency
 * as the RowStretch is often called in a tight loop for each Row
 * in a rectengular stretch and it is best to not use a variable
 * argument with an indirect call or a function call that would
 * build up a callframe and release that callframe later. If you
 * do not need that effiency, use PutRowStretchCode and RunRowStretchCode.
 */
extern unsigned char SDL_TheRowStretchCode[SDL_STRETCHCODE_BUFFERSIZE];

/**
 * The SetRowStretchCode is a wrapper around PutRowStretchCode that
 * uses the Adress and Size of the shared SDL_TheRowStretchCode buffer.
 * The PutRowStretchCode will fill the given buffer with an assembler
 * stream that should be called with SDL_RunRowStretchCode. The
 * assembler stream is usually faster as all the scale decisions are 
 * done in advance of the execution. This helps when a RunCode is
 * done multiple times with the same src_w/dst_w/bpp pair. All the
 * pixel-get and pixel-set calls are unrolled in that buffer. Therefore,
 * the buffer should be big enough - as a rule of thumb use a buffer
 * of size (src_w+dst_w)*5
 *
 * If PutCode or SetCode fails, a null is returned. Otherwise, the
 * start adress of the Assembler is returned, which is the input
 * argument of PutCode.
 */
unsigned char* SDL_SetRowStretchCode(int src_w, int dst_w, int bpp);
/** => SDL_SetRowStretchCode */
unsigned char* SDL_PutRowStretchCode(unsigned char* buffer, int buflen,
				     int src_w, int dst_w, int bpp);
/** => SDL_SetRowStretchCode */
void           SDL_RunRowStretchCode(unsigned char* buffer,
				     unsigned char* src,
				     unsigned char* dst);

/** => SDL_SetRowStretchCode
 * If SDL_SetRowStretchCode fails, this function must be used instead. 
 * This function and its cousins are singular routines that work in 
 * a tight loop to scale a single row. The number specified the 
 * byte-width of each pixel.
 */
void SDL_StretchRow1(Uint8  *src, int src_w, Uint8  *dst, int dst_w);
/** => SDL_SetRowStretchCode */
void SDL_StretchRow2(Uint16 *src, int src_w, Uint16 *dst, int dst_w);
/** => SDL_SetRowStretchCode */
void SDL_StretchRow3(Uint8  *src, int src_w, Uint8  *dst, int dst_w);
/** => SDL_SetRowStretchCode */
void SDL_StretchRow4(Uint32 *src, int src_w, Uint32 *dst, int dst_w);

#endif
