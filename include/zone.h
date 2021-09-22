// zone.h

#ifndef _ZONE_H
#define _ZONE_H

i32 zone_memory_init(const size_t size, const size_t temp_size);

void zone_print_all(FILE* fp);

i32 zone_validate();

void* zone_malloc(size_t size);

void* zone_calloc(size_t count, size_t size);

void* zone_realloc(void* p, size_t new_size);

size_t zone_free(void* p);

void* zone_temp_malloc(size_t size);

size_t zone_temp_free(void* p);

size_t zone_total_alloc();

void zone_dump(const char* path);

void zone_memory_free();

#endif
