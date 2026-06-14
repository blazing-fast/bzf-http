add_library(bzf_bytes STATIC
    src/bzf_bytes_equals.c
    src/bzf_bytes_add_offset.c
    src/bzf_bytes_tokenize.c
    src/bzf_bytes_tokenize_once.c
    src/bzf_bytes_tokenize_with_remainder.c
    src/bzf_bytes_split.c
    include/bzf_bytes.h
)

target_include_directories(bzf_bytes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
