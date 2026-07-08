#include "bzf_hashmap.h"
#include "bzf_hashmap_mocks.h"

#define bzf_hashmap_hash_string bzf_hashmap_hash_string_real
#include "../src/bzf_hashmap_hash_string.c"
#undef bzf_hashmap_hash_string

static uint32_t default_bzf_hashmap_hash_string(struct bzf_bytes_immutable_view key) {
    return bzf_hashmap_hash_string_real(key);
}

static uint32_t (*mock_bzf_hashmap_hash_string_fn)(struct bzf_bytes_immutable_view) = default_bzf_hashmap_hash_string;

void mock_bzf_hashmap_hash_string_set(uint32_t (*fn)(struct bzf_bytes_immutable_view)) {
    mock_bzf_hashmap_hash_string_fn = fn ? fn : default_bzf_hashmap_hash_string;
}

uint32_t bzf_hashmap_hash_string(struct bzf_bytes_immutable_view key) {
    return mock_bzf_hashmap_hash_string_fn(key);
}
