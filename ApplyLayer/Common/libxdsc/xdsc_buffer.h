/*
 * xdsc_buffer.h
 *
 *  Created on: 2018年1月26日
 *      Author: catt
 */

#ifndef _XDSC_BUFFER
#define _XDSC_BUFFER

#define XDSC_DATA_BUF_LEN      8192

#include "protocol/Protocol.h"

struct xdsc_session_s;

typedef struct xdsc_buffer_s
{
    int option_i;  //option  start
    int payload_i;  //payload start
    int start_i;    // packet start
    int end_i;     //pakcet end

    int read_i;

    int  buflen;
    char data[XDSC_DATA_BUF_LEN];

    void (*reset)(struct xdsc_buffer_s*);
    void (*makedefault)(struct xdsc_buffer_s*, struct xdsc_session_s* cs, OpCode op, uint16_t msgid);
    void (*calccrc)(struct xdsc_buffer_s*);
    int  (*parse)(struct xdsc_buffer_s*, int len);
    int  (*setoption)(struct xdsc_buffer_s*, OptionType ot, char* buffer, int len);
    int  (*getoption)(struct xdsc_buffer_s*, OptionType ot, char** ret);
    int  (*payload)(struct xdsc_buffer_s*, void* buffer, int len);
    int  (*size)(struct xdsc_buffer_s*);
    ProtocolHead* (*head)(struct xdsc_buffer_s*);
}xdsc_buffer_t;

extern void xdsc_buffer_init(xdsc_buffer_t*);
extern void protocol_calccrc(ProtocolHead* ph);
extern void protocol_default_head(ProtocolHead* ph, struct xdsc_session_s* cs, OpCode op, uint16_t msgid);

#endif
