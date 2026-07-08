#ifndef BZF_HTTP_SERVER_IMPL_H
#define BZF_HTTP_SERVER_IMPL_H
#include "bzf_http_server.h"
#include "bzf_os.h"

struct bzf_http_server {
    struct bzf_http_handlers handlers;
    int file_descriptor;
};

#endif /* BZF_HTTP_SERVER_IMPL_H */
