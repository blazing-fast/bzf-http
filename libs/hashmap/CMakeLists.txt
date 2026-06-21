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

# Links the real OS wrapper so symbols are resolved at library build time.
target_link_libraries(bzf_hashmap PUBLIC bzf_bytes bzf_os)

# --- Mock objects (test only) ---
# These object files, when linked into a test binary, override the real
# symbols at link time without touching any production library or binary.
add_library(bzf_hashmap_mock OBJECT
    mocks/bzf_hashmap_insert_mock.c
)

# Mocks need the same include paths as the real library.
target_include_directories(bzf_hashmap_mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

# Pull in header paths (bytes, os) without linking archives at this stage.
target_link_libraries(bzf_hashmap_mock PRIVATE bzf_bytes bzf_os)
