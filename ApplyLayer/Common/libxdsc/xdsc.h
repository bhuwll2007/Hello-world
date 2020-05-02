/*
 * xdsc.h
 *
 *  Created on: 2018年1月20日
 *      Author: catt
 */
#ifndef _XDSC_H
#define _XDSC_H

#include "comm.h"
#include "xdsc_buffer.h"
#include "xdsc_operations.h"

#include <unistd.h>

#define MAX_SESSION_NUM        1024

typedef struct xdsc_session_s
{
    int32_t  interact_fd;
    int32_t  monitor_fd;

    uint16_t unique_id;
    uint16_t packet_sn;

    int    ssid;
    int    write_no_wait;

    struct sockaddr_storage interact_addr;
    struct sockaddr_storage monitor_addr;

    pthread_mutex_t interact_lock;
    pthread_mutex_t monitor_lock;

    xdsc_buffer_t interact_buffer;
    xdsc_buffer_t monitor_buffer;

    xdsc_operation_t* ops;
    void* op_priv;
}xdsc_session_t;

extern xdsc_session_t* get_client_session(int hdl);
extern void set_client_session(int hdl, xdsc_session_t* s);

extern xdsc_session_t* alloc_client_session(int hdl);
extern void free_client_session(int hdl);

#endif
