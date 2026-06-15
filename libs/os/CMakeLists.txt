add_library(bzf_os STATIC
        include/bzf_os.h
    src/bzf_os_malloc.c
        src/bzf_os_free.c
        src/bzf_accept.c
        src/bzf_setsockopt.c
)

target_include_directories(bzf_os PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_os PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
