#include "../include/bzf_hashmap.h"
#include "bzf_hashmap_impl.h"

uint32_t bzf_hashmap_hash_string(const struct bzf_bytes_immutable_view key) {
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < key.length; i++) {
        hash ^= (bzf_byte_t) key.buffer[i];
        hash *= 16777619u;
    }

    return hash;
}
