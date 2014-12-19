/******************************************************************************
 * qLibc
 *
 * Copyright (c) 2010-2014 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

/**
 * @file qio.c I/O handling APIs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "qinternal.h"
#include "utilities/qio.h"


/**
 * Test & wait until the file descriptor has readable data.
 *
 * @param fd        file descriptor
 * @param timeoutms wait timeout milliseconds. 0 for no wait,
 *                  -1 for infinite wait
 *
 * @return 1 if readable, 0 on timeout, -1 if an error occurred.
 *
 * @note
 *  The argument timeoutms can be used to set maximum wait time for a socket
 *  descriptor.
 */
int qio_wait_readable(int fd, int timeoutms) {
    struct pollfd fds[1];

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    int pollret = poll(fds, 1, timeoutms);
    if (pollret == 0) {
        errno = ETIMEDOUT;
        return 0;
    } else if (pollret < 0) {
        return -1;
    }

    if (fds[0].revents & POLLIN)
        return 1;
    return -1;
}

/**
 * Test & wait until the file descriptor is ready for writing.
 *
 * @param fd        file descriptor
 * @param timeoutms wait timeout milliseconds. 0 for no wait,
 *                  -1 for infinite wait
 *
 * @return 1 if writable, 0 on timeout, -1 if an error occurred.
 */
int qio_wait_writable(int fd, int timeoutms) {
    struct pollfd fds[1];

    fds[0].fd = fd;
    fds[0].events = POLLOUT;

    int pollret = poll(fds, 1, timeoutms);
    if (pollret == 0) {
        errno = ETIMEDOUT;
        return 0;
    } else if (pollret < 0) {
        return -1;
    }

    if (fds[0].revents & POLLOUT)
        return 1;
    return -1;
}
