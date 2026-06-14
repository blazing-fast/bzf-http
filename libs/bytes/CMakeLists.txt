add_library(bzf_bytes STATIC
    src/bzf_bytes.c
    include/bzf_bytes.h
)

target_include_directories(bzf_bytes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
