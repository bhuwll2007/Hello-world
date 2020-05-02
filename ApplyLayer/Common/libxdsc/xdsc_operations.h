/*
 * xdsc_operations.h
 *
 *  Created on: 2018年9月25日
 *      Author: catt
 */

#ifndef XDSC_OPERATIONS_H_
#define XDSC_OPERATIONS_H_

#include "comm.h"

struct xdsc_session_s;

typedef int  (*op_open)(int ssid, void* flag);
typedef int  (*op_open_inet)(int ssid, void* flag);

typedef void (*op_close)(struct xdsc_session_s* s);
typedef int  (*op_read)(struct xdsc_session_s* s, uint16_t msgid, void* buf, uint16_t len);

typedef int  (*op_sub)(struct xdsc_session_s* s, uint16_t msgid);
typedef int  (*op_unsub)(struct xdsc_session_s* s, uint16_t msgid);

typedef int  (*op_recv)(struct xdsc_session_s* s, void* buf, uint16_t len, int flag);
typedef int  (*op_recv_detach)(struct xdsc_session_s* s, uint16_t type, void* buf, uint16_t len);
typedef int  (*op_recv_msgid)(struct xdsc_session_s* s);

typedef int  (*op_send)(struct xdsc_session_s* s, const void* buf, uint16_t len, int flag);
typedef int  (*op_send_attach)(struct xdsc_session_s* s, uint16_t type, void* buf, uint16_t len);
typedef int  (*op_send_msgid)(struct xdsc_session_s* s, uint16_t msgid);

typedef int  (*op_getopt)(struct xdsc_session_s* s, uint16_t cmd, void* buf, uint16_t len);
typedef int  (*op_setopt)(struct xdsc_session_s* s, uint16_t cmd, const void* buf, uint16_t len);

typedef struct xdsc_operation_s
{
    int ssid;

    op_open open;
    op_open_inet open_inet;
    op_close close;

    op_read read;
    op_sub sub;
    op_unsub unsub;

    op_recv recv;
    op_recv_detach recv_detach;
    op_recv_msgid recv_msgid;

    op_send send;
    op_send_attach send_attach;
    op_send_msgid send_msgid;

    op_getopt getopt;
    op_setopt setopt;
}xdsc_operation_t;

extern void register_operation(xdsc_operation_t* ops);
extern void register_default_operation(xdsc_operation_t* ops);
extern xdsc_operation_t* find_operation(int ssid);


#endif /* XDSC_OPERATIONS_H_ */
