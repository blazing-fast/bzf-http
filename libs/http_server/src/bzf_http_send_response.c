#include "bzf_http_server.h"
#include <errno.h>
#include <unistd.h>

enum bzf_http_send_response_result bzf_http_send_response(const struct bzf_http_client bzf_http_client,
                                                          const struct bzf_bytes_immutable_view to_send)
{
    size_t sent_bytes = 0;

    while (sent_bytes < to_send.length)
    {
        ssize_t n = send(bzf_http_client.file_descriptor, to_send.buffer + sent_bytes, to_send.length - sent_bytes,
                         MSG_NOSIGNAL);
        if (n > 0)
        {
            sent_bytes += (size_t)n;
            continue;
        }
        if (n == 0)
        {
            return BZF_HTTP_SEND_RESPONSE_CLOSED;
        }
        switch (errno)
        {
        case EINTR: continue;
        case EPIPE:
        case ECONNRESET:
#ifdef ETIMEDOUT
        case ETIMEDOUT:
#endif
            return BZF_HTTP_SEND_RESPONSE_CLOSED;
        case EBADF:
        case ENOTSOCK:
        case ENOTCONN:
        case EINVAL:
        case EOPNOTSUPP:
        default:
            return BZF_HTTP_SEND_RESPONSE_ERROR;
        }
    }
    return BZF_HTTP_SEND_RESPONSE_OK;
}
