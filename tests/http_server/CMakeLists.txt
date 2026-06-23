bzf_add_cmocka_test(test_bzf_http_server_initialize test_bzf_http_server_initialize.c bzf_http_server)

bzf_add_cmocka_test(test_bzf_http_add_route_handler test_bzf_http_add_route_handler.c bzf_http_server
    MOCK_TARGETS bzf_hashmap_mock
)

# Allow tests to reach the internal implementation header for white-box checks.
target_include_directories(test_bzf_http_add_route_handler PRIVATE ${CMAKE_SOURCE_DIR}/libs/http_server/src)
