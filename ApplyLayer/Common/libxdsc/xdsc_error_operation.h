/*
 * xdsc_error_operation.h
 *
 *  Created on: 2018年9月26日
 *      Author: catt
 */
#ifndef XDSC_ERROR_OPERAION_H_
#define XDSC_ERROR_OPERAION_H_

#include "comm.h"
#include "xdsc.h"

extern int xdsc_error_open(int serviceID, void* flag);
extern int xdsc_error_open_inet(int serviceID, void* flag);
extern void xdsc_error_close(xdsc_session_t* cs);
extern int xdsc_error_recv(xdsc_session_t* cs, void* buf, uint16_t len, int flag);
extern int xdsc_error_send(xdsc_session_t* cs, const void* buf, uint16_t len, int flag);
extern int xdsc_error_setopt(xdsc_session_t* cs, uint16_t cmd, const void* buf, uint16_t len);
extern int xdsc_error_getopt(xdsc_session_t* cs, uint16_t cmd, void* buf, uint16_t len);
extern int xdsc_error_read(xdsc_session_t* cs, uint16_t msgid, void* buf, uint16_t len);
extern int xdsc_error_recv_msgid(xdsc_session_t* cs);
extern int xdsc_error_send_attach(xdsc_session_t* cs, uint16_t type, void* buf, uint16_t len);
extern int xdsc_error_send_msgid(xdsc_session_t* cs, uint16_t msgid);
extern int xdsc_error_recv_detach(xdsc_session_t* cs, uint16_t type, void* buf, uint16_t len);
extern int xdsc_error_sub(xdsc_session_t* cs, uint16_t msgid);
extern int xdsc_error_unsub(xdsc_session_t* cs, uint16_t msgid);

#endif
