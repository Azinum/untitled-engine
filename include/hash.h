// hash.h

#ifndef _HASH_H
#define _HASH_H

typedef u64 hash_t;

hash_t djb2_hash(u8* data, u32 size);

hash_t sdbm_hash(u8* data, u32 size);

#endif
