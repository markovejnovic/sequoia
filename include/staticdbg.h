#ifndef _STATICDBG_H_
#define _STATICDBG_H_

#ifdef SEQUOIA_STATIC_DEBUG

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void segfault_handler(int signal) {
    void *array[10];
    size_t size;

    size = backtrace(array, 10);
    fprintf(stderr, "Error: signal %d\n", signal);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

#endif
#endif  // _STATICDBG_H_
