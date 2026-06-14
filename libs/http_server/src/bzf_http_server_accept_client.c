#include "bzf_http_server.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/time.h>

enum bzf_http_server_accept_client_result bzf_http_server_accept_client(struct bzf_http_server* http_server, struct bzf_http_client* client,
                                                                        struct sockaddr_in* client_addr)
{
    assert(http_server != NULL);
    assert(client != NULL);
    int *client_fd = &client->file_descriptor;
    *client_fd = -1;
    socklen_t addr_len = client_addr == NULL ? 0 : sizeof(struct sockaddr_in);
    const int accept_client_fd = accept(http_server->file_descriptor, (struct sockaddr*)client_addr, &addr_len);
    if (accept_client_fd < 0)
    {
        return BZF_HTTP_SERVER_ACCEPT_ERROR_WHEN_ACCEPTING;
    }
    *client_fd = accept_client_fd;

    const struct timeval tv = {5, 0};
    if (setsockopt(*client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)))
    {
        fprintf(stderr, "setsockopt failed ! You PROBABLY have a logic error in your code ; %s", strerror(errno));
        return BZF_HTTP_SERVER_ACCEPT_CLIENT_ERROR_WHEN_SETSOCKETOPT;
    }
    printf("client fd : %d\n", *client_fd);

    return BZF_HTTP_SERVER_ACCEPT_CLIENT_OK;
}
