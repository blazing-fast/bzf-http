#include "private.h"
#include "http.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum bzf_handle_request_result http_fetch_and_parse_error_to_request_error(
    const enum http_fetch_and_parse_head_result http_fetch_and_parse_head_result);

enum bzf_handle_request_result bzf_http_handle_request(const struct bzf_http_client client, struct bzf_http_handlers handlers)
{
    enum { HEADERS_INTERNAL_BUFFER_CAPACITY = 20000 };
    bzf_byte_t headers_internal_buffer[HEADERS_INTERNAL_BUFFER_CAPACITY];
    struct bzf_bytes_mutable_buffer headers_raw_buffer = {
        .buffer = headers_internal_buffer, .length = 0, .capacity = HEADERS_INTERNAL_BUFFER_CAPACITY
    };
    struct http_fetch_and_parse_head_output http_fetch_and_parse_head_output;
    const enum bzf_hashmap_initialize_result hashmap_initialize_result = bzf_hashmap_initialize(
        &http_fetch_and_parse_head_output.headers);
    if (hashmap_initialize_result == BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR)
    {
        return BZF_HANDLE_REQUEST_MEMORY_ALLOCATION_ERROR;
    }

    const enum http_fetch_and_parse_head_result http_fetch_headers_res = http_fetch_and_parse_head(
        client.file_descriptor, &headers_raw_buffer, &http_fetch_and_parse_head_output);

    if (http_fetch_headers_res != HTTP_FETCH_AND_PARSE_HEAD_OK)
    {
        printf("lala %d\n", http_fetch_headers_res);
        return http_fetch_and_parse_error_to_request_error(http_fetch_headers_res);
    }

    struct bzf_bytes_immutable_view host_key_immutable_view = {(const bzf_byte_t*)"host", 4};
    void* tmp;
    const enum bzf_hashmap_get_result hashmap_get_host_result = bzf_hashmap_get(
        http_fetch_and_parse_head_output.headers, host_key_immutable_view, &tmp);
    if (hashmap_get_host_result == BZF_HASHMAP_GET_FOUND)
    {
        printf("found\n");
        const struct bzf_bytes_immutable_view* host_value_view = tmp;
        printf_bytes_t(*host_value_view);
    }

    printf_bytes_t(
        bzf_bytes_immutable_view_from_mutable_buffer_and_range(headers_raw_buffer,
                                                               http_fetch_and_parse_head_output.request_line.target));

    printf("\n");

    const struct bzf_bytes_immutable_view target = bzf_bytes_immutable_view_from_mutable_buffer_and_range(
        headers_raw_buffer, http_fetch_and_parse_head_output.request_line.target);
    const enum bzf_hashmap_get_result hashmap_get_target_result = bzf_hashmap_get(
        handlers.configured_routes, target, &tmp);
    struct bzf_bytes_mutable_buffer mutable_buffer;
    struct bzf_http_route_output route_output = {&mutable_buffer};
    if (hashmap_get_target_result == BZF_HASHMAP_GET_FOUND)
    {
        const struct bzf_http_route_handler* handler = tmp;
        handler->handler(&route_output);
    }
    else
    {
        handlers.default_handler(&route_output);
    }
    bzf_http_send_response(client, bzf_bytes_immutable_view_from_mutable_buffer(*route_output.response));
    bzf_hashmap_free(http_fetch_and_parse_head_output.headers, &bzf_http_free_base);

    close(client.file_descriptor);
    return BZF_HANDLE_REQUEST_OK;
}
