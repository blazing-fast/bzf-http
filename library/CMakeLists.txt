add_library(bzf_http_server STATIC
        src/bzf_http_server.c
        src/bzf_hashmap.c
        include/bzf_hashmap.h
        src/utilities.h
        src/utilities.c
        src/http.c
        src/http.h
        src/bzf_dynamic_array.c
        include/bzf_dynamic_array.h
        src/bzf_bytes.c
        include/bzf_bytes.h
        src/bzf_dynamic_array_impl.h
        src/bzf_hashmap_impl.h
)

target_include_directories(bzf_http_server PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private headers (used internally, not exposed)
target_include_directories(bzf_http_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src  # hashmap.h lives here, for example
)
target_compile_options(bzf_http_server PRIVATE
        -Wall
        -Wextra
        -Werror
        -fsanitize=address,undefined
        -g
)

target_link_options(bzf_http_server PRIVATE
        -fsanitize=address,undefined
)