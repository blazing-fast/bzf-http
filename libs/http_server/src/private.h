#pragma once
#include "bzf_http_server.h"

void bzf_http_free_base(struct bzf_bytes_immutable_view key, void* data);

void defaultHandler(struct bzf_http_route_output* output);

void printf_bytes_t(const struct bzf_bytes_immutable_view bytes);
