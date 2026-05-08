#ifndef TASKS_HASHMAP_H
#define TASKS_HASHMAP_H
#include <stdint.h>
#include "bzf_bytes.h"

struct bzf_hashmap_node {
    struct bzf_bytes_immutable_view key;
    void* data;
    struct bzf_hashmap_node* next;
};

struct bzf_hashmap {
    uint_fast32_t number_of_elements;
    uint_fast32_t capacity;
    struct bzf_hashmap_node** elements;
};

enum bzf_hashmap_initialize_result{BZF_HASHMAP_INITIALIZE_OK, BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR} ;


enum bzf_hashmap_initialize_result bzf_hashmap_initialize(struct bzf_hashmap* hashmap);

uint32_t bzf_hashmap_hash_string(struct bzf_bytes_immutable_view key);

enum bzf_hashmap_insert_result{BZF_HASHMAP_INSERT_OK, BZF_HASHMAP_INSERT_ALLOCATION_ERROR};
enum bzf_hashmap_insert_result bzf_hashmap_insert(struct bzf_hashmap* hashmap, struct bzf_bytes_immutable_view key, void* value);


enum bzf_hashmap_get_result{BZF_HASHMAP_GET_FOUND, BZF_HASHMAP_GET_NOT_FOUND};
enum bzf_hashmap_get_result bzf_hashmap_get(const struct bzf_hashmap *const hashmap, const struct bzf_bytes_immutable_view key, void** data);


enum bzf_hashmap_remove_result{BZF_HASHMAP_REMOVE_OK, BZF_HASHMAP_REMOVE_NOT_FOUND};
enum bzf_hashmap_remove_result bzf_hashmap_remove(const struct bzf_hashmap* hashmap, struct bzf_bytes_immutable_view key, void** data);

void bzf_hashmap_free(struct bzf_hashmap *const hashmap, void (*free_fn)(struct bzf_bytes_immutable_view key, void *val));

#endif //TASKS_HASHMAP_H