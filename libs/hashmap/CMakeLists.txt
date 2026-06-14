add_library(bzf_hashmap STATIC
    src/bzf_hashmap.c
    include/bzf_hashmap.h
    src/bzf_hashmap_impl.h
)

target_include_directories(bzf_hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bzf_hashmap PUBLIC bzf_bytes)
