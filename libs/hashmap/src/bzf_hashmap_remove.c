#include "../include/bzf_hashmap.h"
#include <assert.h>
#include <bzf_os.h>
#include <stdlib.h>
#include "bzf_hashmap_impl.h"

enum bzf_hashmap_remove_result bzf_hashmap_remove(const struct bzf_hashmap *const hashmap, const struct bzf_bytes_immutable_view key,
                                               void **data) {
    assert(hashmap != NULL);
    assert(key.buffer != NULL);
    const size_t bucket_index = bzf_hashmap_hash_string(key) % hashmap->capacity;
    struct bzf_hashmap_node *current_node = hashmap->elements[bucket_index];
    struct bzf_hashmap_node *previous_node = NULL;
    while (current_node != NULL) {
        if (bzf_bytes_equals(key, current_node->key)) {
            if (data != NULL) {
                *data = current_node->data;
            }
            if (previous_node == NULL) {
                hashmap->elements[bucket_index] = current_node->next;
            } else {
                previous_node->next = current_node->next;
            }
            bzf_os_free(current_node);
            return BZF_HASHMAP_REMOVE_OK;
        }
        previous_node = current_node;
        current_node = current_node->next;
    }
    return BZF_HASHMAP_REMOVE_NOT_FOUND;
}
