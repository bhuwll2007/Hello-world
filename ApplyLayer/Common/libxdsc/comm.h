/*
 * comm.h
 *
 *  Created on: 2018年9月25日
 *      Author: catt
 */
#ifndef XDSC_COMM_H_
#define XDSC_COMM_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <assert.h>

//#define XDS_CLIENT_DEBUG

#ifdef XDS_CLIENT_DEBUG

#define DEBUG_OUT       stdout
#define D(fmt, ...)     fprintf(DEBUG_OUT, fmt"\n", ##__VA_ARGS__)
#define DD(ptr, len) \
do \
{\
    for(int i = 0; ptr != NULL && i < len ; i++) \
    { \
        fprintf(DEBUG_OUT, "0x%02x ", ((char*)ptr)[i]); \
    } \
    D(""); \
}while(0)

#else

#define D(fmt, ...)
#define DD(ptr, len)

#endif

typedef enum E_XDS_CLIENT_ERROR
{
    E_XDS_SYSTEM_ERROR    = 0x0000,
    E_XDS_SERVER_ERROR    = 0x0100,    /* server error */
    E_XDS_SERVER_STRERROR = 0x0200,    /* server error with string reason */

    E_XDS_CLIENT_ERROR    = 0x0400,
    E_XDS_CLIENT_HOSTIP_ILLEGAL,
    E_XDS_CLIENT_FD_ILLEGAL,
    E_XDS_SERVER_RSP_ILLEGAL,
    E_XDS_CLIENT_READBUF_LACK,
    E_XDS_CLIENT_KEEPALIVE_RSP,
    E_XDS_CLIENT_SEND_ERROR,
    E_XDS_CLIENT_SEND_TOOLARGE,
    E_XDS_CLIENT_USRBUF_ILLEGAL,
    E_XDS_CLIENT_CONNTO_SERVER_FAILURE,
}XDS_CLIENT_ERROR;

extern int errnum();
extern void set_errnum(int errnum);

extern const char* strerr();
extern void set_strerr(char* s, int len);

typedef struct xdsc_error_s
{
    int errnum;
    char* errstr;
}xdsc_error_t;

#define set_serivce_err_array(err, array) \
do{ \
    const int sz = sizeof(array)/sizeof(array[0]); \
    if((int)err <= (int)E_RSPS2C_SERVICE_BASE) \
        set_errnum(err); \
    else if((int)err > (int)(E_RSPS2C_SERVICE_BASE + sz)) \
        set_errnum(err); \
    else { \
        set_errnum((err | E_XDS_SERVER_STRERROR)); \
        char* errstr = array[err - E_RSPS2C_SERVICE_BASE -1].errstr; \
        if(strlen(errstr) == 0) \
            errstr = strerror(errno); \
        set_strerr(errstr, strlen(errstr)); \
    } \
}while(0)

extern void libxdsc_startup();
extern void libxdsc_atexit();

#endif
