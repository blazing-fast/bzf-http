#include "../include/bzf_hashmap.h"
#include <assert.h>
#include "bzf_hashmap_impl.h"

enum bzf_hashmap_get_result bzf_hashmap_get(const struct bzf_hashmap *const hashmap, const struct bzf_bytes_immutable_view key,
                                       void **data) {
    assert(hashmap != NULL);
    assert(key.buffer != NULL);
    assert(data != NULL);
    const size_t bucket_index = bzf_hashmap_hash_string(key) % hashmap->capacity;
    const struct bzf_hashmap_node *current_node = hashmap->elements[bucket_index];
    while (current_node != NULL) {
        if (bzf_bytes_equals(current_node->key, key)) {
            *data = current_node->data;
            return BZF_HASHMAP_GET_FOUND;
        }
        current_node = current_node->next;
    }
    return BZF_HASHMAP_GET_NOT_FOUND;
}
