#ifndef _BZF_OS_H
#define _BZF_OS_H
#include <stddef.h>

void* bzf_os_malloc(size_t size);
void bzf_os_free(void* ptr);
#endif