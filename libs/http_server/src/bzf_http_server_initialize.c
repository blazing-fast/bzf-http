#include "private.h"
#include <assert.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "bzf_os.h"

#define SERVER_PORT 12345

enum bzf_http_server_initialize_result bzf_http_server_initialize(struct bzf_http_server* out)
{
    assert(out != NULL);

    out->file_descriptor = -1;
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }

    const int reuseaddr_opt = 1;
    bzf_setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt));

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    out->file_descriptor = server_fd;
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        fprintf(stderr, "bind failed: %s\n", strerror(errno));
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }

    if (listen(server_fd, 1) < 0)
    {
        fprintf(stderr, "listen failed: %s\n", strerror(errno));
        return BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR;
    }

    out->handlers.default_handler = defaultHandler;
    const enum bzf_hashmap_initialize_result hashmap_initialize_result = bzf_hashmap_initialize(&out->handlers.configured_routes);
    if (hashmap_initialize_result == BZF_HASHMAP_INITIALIZE_ALLOCATION_ERROR)
    {
        return BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR;
    }
    return BZF_HTTP_SERVER_INITIALIZE_OK;
}
