#pragma once
#include "bzf_http_server.h"

void bzf_http_free_base(struct bzf_bytes_immutable_view key, void* data);

void defaultHandler(struct bzf_http_route_output* output);

void printf_bytes_t(const struct bzf_bytes_immutable_view bytes);

enum bzf_handle_request_result
{
    BZF_HANDLE_REQUEST_OK,
    BZF_HANDLE_REQUEST_INVALID,
    BZF_HANDLE_REQUEST_CLOSED,
    BZF_HANDLE_REQUEST_MEMORY_ALLOCATION_ERROR,
    BZF_HANDLE_REQUEST_UNEXPECTED_ERROR,
};
enum bzf_handle_request_result bzf_http_handle_request(const struct bzf_http_client client,
                                                        struct bzf_http_handlers handlers);
