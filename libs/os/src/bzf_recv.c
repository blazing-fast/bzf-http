#include <sys/socket.h>

#include "bzf_os.h"

ssize_t bzf_recv(int sockfd, void *buf, size_t len, int flags)
{
    return recv(sockfd, buf, len, flags);
}
