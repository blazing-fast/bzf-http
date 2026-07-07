#include "../include/bzf_hashmap.h"
#include <assert.h>
#include <stdlib.h>
#include "bzf_hashmap_impl.h"
#include "bzf_os.h"

enum bzf_hashmap_initialize_result bzf_hashmap_initialize(struct bzf_hashmap **const out) {
    assert(out != NULL);
    struct bzf_hashmap *hashmap = bzf_os_malloc(sizeof(struct bzf_hashmap));
    if (hashmap == NULL){
        return BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR;
    }
    hashmap->capacity = 100;
    hashmap->number_of_elements = 0;
    struct bzf_hashmap_node **elements = bzf_os_calloc(hashmap->capacity, sizeof(struct bzf_hashmap_node *));
    if (elements == NULL) {
        bzf_os_free(hashmap);
        return BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR;
    }
    hashmap->elements = elements;
    *out = hashmap;
    return BZF_HASHMAP_INITIALIZE_OK;
}
