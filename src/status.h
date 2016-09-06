/*
   Copyright (c)2014-2016 Peak6 Investments, LP.  All rights reserved.
   Use of this source code is governed by the LICENSE file.
*/

/* function return codes */

#ifndef STATUS_H
#define STATUS_H

typedef int boolean;

#define FALSE 0
#define TRUE 1

typedef int status;

#define FAILED(x) ((x) < OK)

#define SIG_ERROR_BASE -128
#define ERRNO_ERROR_BASE_1 0
#define ERRNO_ERROR_BASE_2 70
#define LANCASTER_ERROR_BASE -220

#define OK 0
/* #define EOF (LANCASTER_ERROR_BASE - 1) */
#define SYNTAX_ERROR (LANCASTER_ERROR_BASE - 2)
#define BLOCKED (LANCASTER_ERROR_BASE - 3)
#define NOT_FOUND (LANCASTER_ERROR_BASE - 4)
#define NO_MEMORY (LANCASTER_ERROR_BASE - 5)
#define NO_HEARTBEAT (LANCASTER_ERROR_BASE - 6)
#define PROTOCOL_ERROR (LANCASTER_ERROR_BASE - 7)
#define PROTOCOL_TIMEOUT (LANCASTER_ERROR_BASE - 8)
#define WRONG_FILE_VERSION (LANCASTER_ERROR_BASE - 9)
#define WRONG_WIRE_VERSION (LANCASTER_ERROR_BASE - 10)
#define WRONG_DATA_VERSION (LANCASTER_ERROR_BASE - 11)
#define SEQUENCE_OVERFLOW (LANCASTER_ERROR_BASE - 12)
#define UNEXPECTED_SOURCE (LANCASTER_ERROR_BASE - 13)
#define CHANGE_QUEUE_OVERRUN (LANCASTER_ERROR_BASE - 14)
#define BUFFER_TOO_SMALL (LANCASTER_ERROR_BASE - 15)
#define MTU_TOO_SMALL (LANCASTER_ERROR_BASE - 16)
#define INVALID_ADDRESS (LANCASTER_ERROR_BASE - 17)
#define DEADLOCK_DETECTED (LANCASTER_ERROR_BASE - 18)
#define NO_CHANGE_QUEUE (LANCASTER_ERROR_BASE - 19)
#define STORAGE_READ_ONLY (LANCASTER_ERROR_BASE - 20)
#define STORAGE_UNEQUAL (LANCASTER_ERROR_BASE - 21)
#define STORAGE_CORRUPTED (LANCASTER_ERROR_BASE - 22)
#define STORAGE_ORPHANED (LANCASTER_ERROR_BASE - 23)
#define STORAGE_RECREATED (LANCASTER_ERROR_BASE - 24)
#define STORAGE_FULL (LANCASTER_ERROR_BASE - 25)
#define INVALID_CAPACITY (LANCASTER_ERROR_BASE - 26)
#define INVALID_OPEN_FLAGS (LANCASTER_ERROR_BASE - 27)
#define INVALID_RECORD (LANCASTER_ERROR_BASE - 28)
#define INVALID_DEVICE (LANCASTER_ERROR_BASE - 29)
#define INVALID_FORMAT (LANCASTER_ERROR_BASE - 30)
#define INVALID_NUMBER (LANCASTER_ERROR_BASE - 31)

#endif