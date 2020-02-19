#ifndef MALLOC_MW_H
#define MALLOC_MW_H

/* Copyright (c) 2004, Joerg Wunsch
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

/* $Id: stdlib_private.h 1657 2008-03-24 17:11:08Z arcanum $ */

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>


struct __freelist {
    size_t sz;
    struct __freelist *nx;
};

extern char *__brkval_mw;			/* first location not yet allocated */
extern struct __freelist *__flp_mw; /* freelist pointer (head of freelist) */
extern size_t __malloc_margin_mw;	/* user-changeable before the first malloc() */
extern char *__malloc_heap_start_mw;
extern char *__malloc_heap_end_mw;

extern void *mymalloc(size_t);
extern void myfree(void *);
extern void *myrealloc(void *, size_t);

#define malloc_mw mymalloc
#define free_mw myfree
#define realloc_mw myrealloc

#define __heap_start mymem[0]
#define __heap_end mymem[256]
extern char mymem[];
#define STACK_POINTER() (mymem + 256)

#endif