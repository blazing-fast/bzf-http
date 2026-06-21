#include "bzf_hashmap.h"
#include "bzf_hashmap_mocks.h"
#include "bzf_hashmap_impl.h"
#include "bzf_os.h"

#define bzf_hashmap_insert bzf_hashmap_insert_real
#include "../src/bzf_hashmap_insert.c"
#undef bzf_hashmap_insert

static enum bzf_hashmap_insert_result default_insert(
    struct bzf_hashmap* const hashmap,
    const struct bzf_bytes_immutable_view key,
    void* value)
{
    return bzf_hashmap_insert_real(hashmap, key, value);
}

static enum bzf_hashmap_insert_result (
    *mock_insert_fn)(struct bzf_hashmap* const,
                     const struct bzf_bytes_immutable_view,
                     void*) = default_insert;

void mock_bzf_hashmap_insert_set(
    enum bzf_hashmap_insert_result (
        *fn)(struct bzf_hashmap* const,
             const struct bzf_bytes_immutable_view,
             void*))
{
    mock_insert_fn = fn ? fn : default_insert;
}

enum bzf_hashmap_insert_result bzf_hashmap_insert(
    struct bzf_hashmap* const hashmap,
    const struct bzf_bytes_immutable_view key,
    void* value)
{
    return mock_insert_fn(hashmap, key, value);
}
