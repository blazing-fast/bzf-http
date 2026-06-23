#pragma once
#include "bzf_http_server.h"
#include "bzf_os.h"

struct bzf_http_server {
    struct bzf_http_handlers handlers;
    int file_descriptor;
};
