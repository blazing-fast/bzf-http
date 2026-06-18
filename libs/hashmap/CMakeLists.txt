add_library(bzf_hashmap STATIC
    src/bzf_hashmap_initialize.c
    src/bzf_hashmap_hash_string.c
    src/bzf_hashmap_insert.c
    src/bzf_hashmap_get.c
    src/bzf_hashmap_remove.c
    src/bzf_hashmap_free.c
    include/bzf_hashmap.h
    src/bzf_hashmap_impl.h
)

target_include_directories(bzf_hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Only depends on headers, not on the OS implementation.
target_link_libraries(bzf_hashmap PUBLIC bzf_bytes bzf_os_headers)
