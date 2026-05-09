#include "../include/bzf_hashmap.h"

#include <assert.h>
#include <stdlib.h>

#include "bzf_hashmap_impl.h"
#include "utilities.h"

enum bzf_hashmap_initialize_result bzf_hashmap_initialize(struct bzf_hashmap **const out) {
    assert(out != NULL);
    // remember : opaque API, so you need to malloc here!
    struct bzf_hashmap *hashmap = malloc(sizeof(struct bzf_hashmap));
    if (hashmap == NULL){
        return BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR;
    }
    hashmap->capacity = 100;
    hashmap->number_of_elements = 0;
    struct bzf_hashmap_node **elements = calloc(hashmap->capacity, sizeof(struct bzf_hashmap_node *));
    if (elements == NULL) {
        free(hashmap);
        return BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR;
    }
    hashmap->elements = elements;
    *out = hashmap;
    return BZF_HASHMAP_INITIALIZE_OK;
}


uint32_t bzf_hashmap_hash_string(const struct bzf_bytes_immutable_view key) {
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < key.length; i++) {
        hash ^= (bzf_byte_t) key.buffer[i];
        hash *= 16777619u;
    }

    return hash;
}

/**
 * Insert a node in the hashmap
 * @param hashmap the hashmap
 * @param key the key. Will NOT be copied.
 * @param value the value. Will NOT be copied.
 * @return the result
 * @pre hashmap != NULL
 * @pre key.buffer != NULL
 * @pre key.length > 0
 */
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
            free(current_node);
            return BZF_HASHMAP_REMOVE_OK;
        }
        previous_node = current_node;
        current_node = current_node->next;
    }
    return BZF_HASHMAP_REMOVE_NOT_FOUND;
}

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
            free(cur_node);
            cur_node = next_node;
        }

        ++cur_bucket;
    }
    free(hashmap->elements);
    free(hashmap);
}
