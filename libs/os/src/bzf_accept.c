#include <sys/socket.h>

int bzf_accept(
    const int fd,
    struct sockaddr* addr,
    socklen_t* addr_len)
{
    return accept(fd, addr, addr_len);
}
