// zone.h

#ifndef _ZONE_H
#define _ZONE_H

i32 zone_memory_init(u32 size);

void zone_print_all(FILE* fp);

void* zone_malloc(u32 size);

void* zone_calloc(u32 count, u32 size);

void* zone_realloc(void* p, u32 new_size);

u32 zone_free(void* p);

u32 zone_try_free(void* p);

u32 zone_total_alloc();

void zone_dump(const char* path);

void zone_memory_free();

#endif
