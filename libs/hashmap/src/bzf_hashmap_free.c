#include "../include/bzf_hashmap.h"
#include <assert.h>
#include <bzf_os.h>
#include <stdlib.h>
#include "bzf_hashmap_impl.h"

void bzf_hashmap_free(struct bzf_hashmap *const hashmap, void (*free_fn)(struct bzf_bytes_immutable_view key, void *val)) {
    assert(hashmap != NULL);
    assert(hashmap->elements != NULL);

    struct bzf_hashmap_node **cur_bucket = hashmap->elements;
    struct bzf_hashmap_node **end_bucket = hashmap->elements + hashmap->capacity;
    while (cur_bucket < end_bucket) {
        struct bzf_hashmap_node *cur_node = *cur_bucket;
        while (cur_node != NULL) {
            if (free_fn != NULL) {
                free_fn(cur_node->key, cur_node->data);
            }
            struct bzf_hashmap_node *next_node = cur_node->next;
            bzf_os_free(cur_node);
            cur_node = next_node;
        }

        ++cur_bucket;
    }
    bzf_os_free(hashmap->elements);
    bzf_os_free(hashmap);
}
