/*
 * stackt_ime.h
 *
 *  Created on: 2018年1月28日
 *      Author: catt
 */

#ifndef _STACKTIME_H_
#define _STACKTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"
#include <inttypes.h>

#define SSID_STIME       11
#define SSNAME_STIME     "StackTime"

enum StackTimeError
{
    STACKTIME_ERROR_MSGID_ILLEGAL = E_RSPS2C_SERVICE_BASE,
    STACKTIME_ERROR_MMAP_STACK,
};

enum StackTimeMsgID
{
    STACKTIME_MSGID_ELAPSED       = 1,
    STACKTIME_MSGID_SUBFRAME_NUM  = 2,
};

typedef struct stack_sfn_s
{
    int32_t sfn;
    int32_t ssfn;
}stack_sfn_t;

typedef struct stack_time_s
{
    int32_t elapsed_s;
    int32_t elapsed_ms;
    int32_t elapsed_us;
}stack_time_t;

#ifdef __cplusplus
}
#endif

#endif /* _STACKTIME_H_ */
