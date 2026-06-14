add_library(bzf_http_server STATIC
    src/printf_bytes_t.c
    src/default_handler.c
    src/bzf_http_server_initialize.c
    src/bzf_http_server_accept_client.c
    src/bzf_http_add_route_handler.c
    src/bzf_http_free_base.c
    src/bzf_http_send_response.c
    src/bzf_http_server_destroy.c
    src/http_fetch_and_parse_error_to_request_error.c
    src/bzf_http_handle_request.c
    src/parse_http_request_line.c
    src/http_fetch_and_parse_head.c
        include/bzf_http_server.h
    src/http.h
)

target_include_directories(bzf_http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_http_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bzf_http_server PUBLIC bzf_bytes bzf_dynamic_array bzf_hashmap)
