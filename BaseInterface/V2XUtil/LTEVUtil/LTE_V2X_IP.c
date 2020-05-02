#include "../LTEVUtil/LTE_V2X_IP.h"

#include "./../../ConfUtil/Config.h"
char g_acConfFileName_V2X[30] = "/etc/V2X_APP.ini";

int ip_init(LTE_V2X_Options *option)
{
    int host_port = 6666;
    int peer_port = 6666;
    int optval;
    int fd_modem = -1;
    memcpy(option->m_service.host_ip, "192.168.40.199",
           sizeof("192.168.40.199"));
    memcpy(option->m_service.peer_ip, "192.168.40.255",
           sizeof("192.168.40.255"));
    option->m_service.ip_mask = 24;

    /****/
    char achost_port[50];
    char acpeer_port[50];
    memset(achost_port, 0x00, sizeof(achost_port));
    memset(acpeer_port, 0x00, sizeof(acpeer_port));

    ///读取配置文件中GPS的串口号
    int iRetCode = 0;
    iRetCode = GetConfigStringValue(g_acConfFileName_V2X, "LTEVUtil", "host_port",
                                    achost_port);
    if (iRetCode)
    {
        printf("Read LTEV host_port Error ! iRetCode : %d !\n", iRetCode);
        printf("Use the default port number:6666.\n");
    }
    else
    {
        host_port = atoi(achost_port);
    }
    iRetCode = GetConfigStringValue(g_acConfFileName_V2X, "LTEVUtil", "peer_port",
                                    acpeer_port);
    if (iRetCode)
    {
        printf("Read LTEV peer_port Error ! iRetCode : %d !\n", iRetCode);
        printf("Use the default port number:6666.\n");
    }
    else
    {
        peer_port = atoi(acpeer_port);
    }
    /****/
    option->m_service.host_port = host_port;
    option->m_service.peer_port = peer_port;
    fd_modem = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd_modem)
    {
        perror("socket");
        return -1;
    }
    else
        option->m_ltev2x_hdl = fd_modem;
    optval = 1;
    if (0
            != setsockopt(option->m_ltev2x_hdl, SOL_SOCKET, SO_BROADCAST,
                          &optval, sizeof(optval)))
    {
        printf("[ip]: setsockopt BROADCAST error !\n");
        return -1;
    }

    optval = 1;
    if (0
            != setsockopt(option->m_ltev2x_hdl, SOL_SOCKET, SO_NO_CHECK,
                          &optval, sizeof(optval)))
    {
        printf("[ip]: setsockopt SO_NO_CHECK error !\n");
        return -1;
    }

    option->m_service.peer_addr.sin_family = AF_INET;
    option->m_service.peer_addr.sin_port = htons(option->m_service.peer_port);
    option->m_service.peer_addr.sin_addr.s_addr = inet_addr(
                option->m_service.peer_ip);

    option->m_service.host_addr.sin_family = AF_INET;
    option->m_service.host_addr.sin_port = htons(option->m_service.host_port);
    option->m_service.host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(option->m_ltev2x_hdl,
             (struct sockaddr*) &option->m_service.host_addr,
             sizeof(struct sockaddr)) < 0)
    {
        printf("[ip]: host socket bind error !\n");
        return -1;
    }

    return 0;
}
int ip_write(LTE_V2X_Options *option, char *p_pcBuf, int p_nLen)
{
    int iSendLen = 0;
    char tx_buf[4000];
#if PRINTTIME
    printf("1 option->m_ltev2x_hdl = %d\n",option->m_ltev2x_hdl);
#endif
    if (-1 == option->m_ltev2x_hdl)
        return -1;
    LTEV2X_Option* msg = (LTEV2X_Option*) tx_buf;

    memcpy(tx_buf, &(option->m_ltevOpts.ue_id), sizeof(LTEV2X_Option));
    msg->data_len = p_nLen;

    memcpy(&tx_buf[sizeof(LTEV2X_Option)], p_pcBuf, p_nLen);
#if PRINTTIME
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];
    gettimeofday(&tv, NULL); //获取当前时间到 tv
    nowtime = tv.tv_sec;//nowtime 存储了秒级的时间值
    nowtm = localtime(&nowtime);//转换为 tm 数据结构
    //用 strftime 函数将 tv 转换为字符串，但 strftime 函数只能达到秒级精度
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    //将毫秒值追加到 strftime 转换的字符串末尾
    snprintf(buf, sizeof buf, "%s.%06d", tmbuf, tv.tv_usec);
    fprintf(option->pTxLogfp, "-- %s --\n", buf);
#endif

#if 0
    printf("\n\n\n");
    printf("The sent data is list like below------\n");
    int i;
    for (i = 0; i < p_nLen; i++)
    {
        printf("%02X ", p_pcBuf[i]);
    }
    printf("\n");

#endif
    iSendLen = sendto(option->m_ltev2x_hdl, tx_buf,
                      sizeof(LTEV2X_Option) + p_nLen, 0,
                      (struct sockaddr*) &(option->m_service.peer_addr),
                      sizeof(struct sockaddr_in));

    if (iSendLen > 0)
    {
        if (option->pTxLogfp > 0)
        {
            int i = 0;
            for (i = 0; i < iSendLen; i++)
                fprintf(option->pTxLogfp, "%02X ", tx_buf[i]);
            fprintf(option->pTxLogfp, "\n");
        }
    }
    return iSendLen;
#if 0
    int iSendLen = 0;
    int i = 0;

    int payloadbg = sizeof(al_msg_t);
    for (i = 0; i < iSendLen; i++)
    {
        if (i != payloadbg);
        else
            fprintf(option->pTxLogfp, "[ ", tx_buf[i]);

        if (i % 16 == 0)
        {
            printf("\n");
            printf("i = %3d | ", i);
        }
        printf("%02X ", tx_buf[i]);
        fprintf(option->pTxLogfp, "%02X ", tx_buf[i]);
    }
    fprintf(option->pTxLogfp, "]", tx_buf[i]);
#endif
#if 0
    MessageFrame_t *pSendMsg = NULL;
    if (iSendLen > 0)
    {
        // decode the sendbuf and write into log file
        asn_dec_rval_t ret;
        fprintf(option->pTxLogfp, "\n");

        ret = uper_decode(0, &asn_DEF_MessageFrame, (void **) &pSendMsg, p_pcBuf,
                          p_nLen, 0, 0);
        if (ret.code != RC_OK)
        {
            fprintf(option->pTxLogfp, "SendBuf Decode Failed\n ");
        }
        xer_fprint(option->pTxLogfp, &asn_DEF_MessageFrame, pSendMsg);
    }
    ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);

    fprintf(option->pTxLogfp, "\n\n");
    fflush(option->pTxLogfp);
    printf("\n");
#endif
}
int ip_read(LTE_V2X_Options *option, char *p_pPlayLoad)
{
    char rx_buf[LTEV2X_RECV_BUF_LEN] =
    { 0 };
    int recvlen = 0;
    struct sockaddr_in raddr;
    socklen_t addrlen = sizeof(raddr);
    LTEV2X_Option* msg;
#if PRINTTIME
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];
    gettimeofday(&tv, NULL); //获取当前时间到 tv
    nowtime = tv.tv_sec;//nowtime 存储了秒级的时间值
    nowtm = localtime(&nowtime);//转换为 tm 数据结构
    //用 strftime 函数将 tv 转换为字符串，但 strftime 函数只能达到秒级精度
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    //将毫秒值追加到 strftime 转换的字符串末尾
    snprintf(buf, sizeof buf, "%s.%06d", tmbuf, tv.tv_usec);
    fprintf(option->pRxLogfp, "-- %s --\n", buf);
#endif
    if (option->m_ltev2x_hdl < 0)
    {
        printf("option->m_ltev2x_hdl < 0");
        return -1;
    }
    recvlen = recvfrom(option->m_ltev2x_hdl, rx_buf, LTEV2X_RECV_BUF_LEN, 0,
                       (struct sockaddr*) &raddr, &addrlen);
#if 1
    if (recvlen > 0)
    {
        msg = (LTEV2X_Option*) rx_buf;
#if 0
        printf("recvlen is %d, msg->plen is %d,received power: %hhddBm\n",
               recvlen, msg->data_len, msg->tx_power + 30);

        int i;
        for (i = 0; i < recvlen; i++)
        {
            printf("%02X ", rx_buf[i]);
        }
        printf("\n");
#endif
#if 0
        printf("The received data is list like below------\n");
        int i = 0;

        int j = 0;

        int payloadbg = sizeof(al_msg_t);
        for (i = 0; i < recvlen; i++)
        {
            if (i != payloadbg);
            else
                fprintf(option->pRxLogfp, "[ ");

            if (i % 16 == 0)
            {
                printf("\n");
                printf("i = %3d | ", i);
            }
            printf("%02X ", rx_buf[i]);
            fprintf(option->pRxLogfp, "%02X ", rx_buf[i]);
        }
        fprintf(option->pRxLogfp, "]", rx_buf[i]);

        printf("\n");
#endif
    }
#endif

    if (recvlen > 0)
    {
        if (option->pRxLogfp > 0)
        {
            int i = 0;
            for (i = 0; i < recvlen; i++)
                fprintf(option->pRxLogfp, "%02X ", rx_buf[i]);
            fprintf(option->pRxLogfp, "\n");
        }
    }

    memcpy(p_pPlayLoad, rx_buf + sizeof(LTEV2X_Option), msg->data_len);

    return msg->data_len;
}

