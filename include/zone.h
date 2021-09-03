// zone.h

#ifndef _ZONE_H
#define _ZONE_H

i32 zone_memory_init(u32 size);

void* zone_malloc(u32 size);

u32 zone_free(void* p);

void zone_memory_free();

#endif
