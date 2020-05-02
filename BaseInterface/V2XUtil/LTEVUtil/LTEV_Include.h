#ifndef __LTEV_INCLUDE_H_
#define __LTEV_INCLUDE_H_
#include "../../DataSet.h"
#include "../LTEVUtil/ltev2x.h"

#define LTEV2X_RECV_BUF_LEN   4096

typedef struct
{
    char host_ip[20];
    char peer_ip[20];

    unsigned int ip_mask;
    unsigned int host_port;
    unsigned int peer_port;

    struct sockaddr_in host_addr;
    struct sockaddr_in peer_addr;
}ip_service;

//LTE-V 设备的综合参数结构体
typedef struct
{
	//设备工作模式0 - ipmode  1 - apimode
    int LTE_V_WorkMode;
    //设备操作句柄
    LTEV2X_HANDLE m_ltev2x_hdl;
    //接收日志文件指针
    FILE *pRxLogfp;
    //发送日志文件指针
    FILE *pTxLogfp;
    //接收功率值
    int nRecvPower;
    //IP 模式下网络参数结构体定义
	ip_service m_service;
    //LTEV 通讯参数结构体定义
    LTEV2X_Option m_ltevOpts;
}LTE_V2X_Options;

extern LTE_V2X_Options lte_v_option;

#endif /*__LTEV_INCLUDE_H_*/
