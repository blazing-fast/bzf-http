add_library(bzf_http_server STATIC
    src/bzf_http_server.c
    src/http.c
    include/bzf_http_server.h
    src/http.h
)

target_include_directories(bzf_http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_http_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bzf_http_server PUBLIC bzf_bytes bzf_dynamic_array bzf_hashmap)
