// pack.h

#ifndef _PACK_H
#define _PACK_H

i32 pack_dir(const char* dest, const char* source);

i32 unpack(const char* path);

i32 read_pack_file(const char* path, const char* pack_file, Buffer* buffer);

#endif
