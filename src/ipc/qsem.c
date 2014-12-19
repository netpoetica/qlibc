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
 * @file qsem.c Semaphore APIs.
 *
 * @note
 * @code
 *   [daemon main]
 *   #define MAX_SEMAPHORES (2)
 *
 *   // create semaphores
 *   int semid = qsem_init("/some/file/for/generating/unique/key", 'q', MAX_SEMAPHORES, true);
 *   if(semid < 0) {
 *     printf("ERROR: Can't initialize semaphores.\n");
 *     return -1;
 *   }
 *
 *   // fork childs
 *   (... child forking codes ...)
 *
 *   // at the end of daemon, free semaphores
 *   if(semid >= 0) qsem_free(semid);
 *
 *   [forked child]
 *   // critical section for resource 0
 *   qsem_enter(semid, 0);
 *   (... guaranteed as atomic procedure ...)
 *   qsem_leave(semid, 0);
 *
 *   (... some codes ...)
 *
 *   // critical section for resource 1
 *   qsem_enter(semid, 1);
 *   (... guaranteed as atomic procedure ...)
 *   qsem_leave(semid, 1);
 *
 *   [other program which uses resource 1]
 *   int semid = qsem_getid("/some/file/for/generating/unique/key", 'q');
 *   if(semid < 0) {
 *     printf("ERROR: Can't get semaphore id.\n");
 *     return -1;
 *   }
 *
 *   // critical section for resource 1
 *   qsem_enter(semid, 1);
 *   (... guaranteed as atomic procedure ...)
 *   qsem_leave(semid, 1);
 *
 * @endcode
 */

#ifndef DISABLE_IPC

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "qinternal.h"
#include "ipc/qsem.h"

/**
 * Force to turn on the flag of semaphore.
 *
 * @param semid     semaphore identifier
 * @param semno     semaphore number
 * @param maxwaitms maximum waiting micro-seconds to release
 * @param forceflag status will be stored, it can be NULL if you don't need this information
 *
 * @return true if successful, otherwise returns false
 *
 * @note
 * This will wait the semaphore to be released with in maxwaitms.
 * If it it released by locker normally with in maxwaitms, forceflag will be set to false.
 * But if maximum maxwaitms is exceed and the semaphore is released forcely, forceflag will
 * be set to true.
 */
bool qsem_enter_force(int semid, int semno, int maxwaitms, bool *forceflag) {
    int wait;
    for (wait = 0; wait < maxwaitms; wait += 10) {
        if (qsem_enter_nowait(semid, semno) == true) {
            if (forceflag != NULL)
                *forceflag = false;
            return true;
        }
        usleep(10 * 1000);  // sleep 10ms
    }

    DEBUG("force to unlock semaphore %d-%d", semid, semno);
    while (true) {
        qsem_leave(semid, semno);
        if (qsem_enter_nowait(semid, semno) == true)
            break;
    }

    if (forceflag != NULL)
        *forceflag = true;
    return true;
}

#endif /* DISABLE_IPC */
