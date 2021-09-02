// renderer.h

#ifndef _RENDERER_H
#define _RENDERER_H

#include "renderer_opengl.h"

i32 renderer_init();

void renderer_clear(u8 r, u8 g, u8 b);

void renderer_free();

#endif
