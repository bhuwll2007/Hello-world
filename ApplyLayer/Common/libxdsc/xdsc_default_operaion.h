/*
 * xdsc_comm_operation.h
 *
 *  Created on: 2018年9月25日
 *      Author: catt
 */

#ifndef XDSC_DEFAULT_OPERAION_H_
#define XDSC_DEFAULT_OPERAION_H_


#define XDS_CLIENT_UNIXPATH_DEFAULT   "/tmp/.xdsc/"
#define XDS_CLIENT_ERROR_STR_LEN      256
#define XDSC_CONNECT_RETRY_TIME       3
#define XDSC_INET_SERVER_IP           "192.168.20.199"
#define XDSC_INET_SERVER_MASK         "255.255.255.0"
#define XDSC_INET_SERVER_PORT         5000
//#define XDS_CLIENT_DEBUG

extern void xdsc_default_operation_startup();
extern void xdsc_default_operation_atexit();

#endif /* XDSC_DEFAULT_OPERAION_H_ */
