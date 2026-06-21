#include "bzf_os.h"
#include <sys/socket.h>

int bzf_listen(const int fd, const int backlog) {
    return listen(fd, backlog);
}
