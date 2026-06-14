#include "../include/bzf_hashmap.h"
#include <assert.h>
#include <stdlib.h>
#include "bzf_hashmap_impl.h"

enum bzf_hashmap_insert_result bzf_hashmap_insert(struct bzf_hashmap *const hashmap, const struct bzf_bytes_immutable_view key,
                                               void *value) {
    assert(hashmap != NULL);
    assert(key.buffer != NULL);
    const size_t bucket_index = bzf_hashmap_hash_string(key) % hashmap->capacity;
    struct bzf_hashmap_node *new_node = malloc(sizeof(struct bzf_hashmap_node));
    if (new_node == NULL) {
        return BZF_HASHMAP_INSERT_ALLOCATION_ERROR;
    }

    new_node->key = key;
    new_node->data = value;
    new_node->next = NULL;

    new_node->next = hashmap->elements[bucket_index];
    hashmap->elements[bucket_index] = new_node;

    hashmap->number_of_elements++;

    return BZF_HASHMAP_INSERT_OK;
}
