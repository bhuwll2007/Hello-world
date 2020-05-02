#include "../LTEVUtil/LTE_V2X_API.h"

int api_init(LTE_V2X_Options *option)
{
    int ret = 0;

    // get config value
    option->m_ltevOpts.ue_id = 211;
    option->m_ltevOpts.ps_id = 0;
    option->m_ltevOpts.tx_power = 20;
    option->m_ltevOpts.priority = 7;
    option->m_ltevOpts.period = 0;
    option->m_ltevOpts.proc_id = 0;
    option->m_ltevOpts.data_len = 200;
    option->m_ltevOpts.test_mode = 0;
    option->m_ltevOpts.test_sfn = 0;

    //open ltev2x
    option->m_ltev2x_hdl = LTEV2X_Open();
    if (-1 == option->m_ltev2x_hdl) {
        printf("open ltev2x error!\n");
        return -1;
    }

    //config ltev2x
    if (0 != LTEV2X_SetOption(option->m_ltev2x_hdl, &option->m_ltevOpts)) {
        printf("set ltev2x error!\n");
        return -1;
    }
    return ret;
}

int api_write(LTE_V2X_Options *option, char *p_pcBuf, int p_nLen)
{
	int iSendLen = 0;
#if PRINTTIME
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];
    gettimeofday(&tv, NULL); //获取当前时间到 tv
    nowtime = tv.tv_sec; //nowtime 存储了秒级的时间值
    nowtm = localtime(&nowtime); //转换为 tm 数据结构
    //用 strftime 函数将 tv 转换为字符串，但 strftime 函数只能达到秒级精度
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    //将毫秒值追加到 strftime 转换的字符串末尾
    snprintf(buf, sizeof buf, "%s.%06d", tmbuf, tv.tv_usec);
    fprintf(option->pTxLogfp, "-- %s --\n", buf);
#endif
#if 0
	printf("\n\n\n");
	printf("The sent data is list like below------\n");
#endif
	iSendLen = LTEV2X_Send(option->m_ltev2x_hdl, (void*)p_pcBuf, p_nLen);
#if 0
	int i = 0;
	fprintf(option->pTxLogfp, "[", p_pcBuf[i]);
	for (i = 0; i < iSendLen; i++)
	{
		if (i % 16 == 0)
		{
			printf("\n");
			printf("i = %3d | ", i);
		}
		printf("%02X ", p_pcBuf[i]);
		fprintf(option->pTxLogfp, "%02X ", p_pcBuf[i]);
	}
	fprintf(option->pTxLogfp, "]", p_pcBuf[i]);
#endif
	if (iSendLen > 0)
	{
		return iSendLen;
	}
#if 0
	MessageFrame_t *pSendMsg = NULL;
	if (iSendLen > 0)
	{
		// decode the sendbuf and write into log file
		asn_dec_rval_t ret;
		fprintf(option->pTxLogfp, "\n");

		ret = uper_decode(0, &asn_DEF_MessageFrame, (void **) &pSendMsg, p_pcBuf,
				p_nLen, 0, 0);
		if (ret.code != RC_OK) {
			fprintf(option->pTxLogfp, "SendBuf Decode Failed\n ");
		}
		xer_fprint(option->pTxLogfp, &asn_DEF_MessageFrame, pSendMsg);
	}
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);


	fprintf(option->pTxLogfp, "\n\n");
	fflush(option->pTxLogfp);

	printf("\n");
#endif
	return iSendLen;
}

int api_read(LTE_V2X_Options *option, char *p_pcBuf)
{
	int recvlen = 0;

    if (option->m_ltev2x_hdl < 0)
    {
    	printf("option->m_ltev2x_hdl < 0");
    	return -1;
    }
#if PRINTTIME
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];
    gettimeofday(&tv, NULL); //获取当前时间到 tv
    nowtime = tv.tv_sec; //nowtime 存储了秒级的时间值
    nowtm = localtime(&nowtime); //转换为 tm 数据结构
    //用 strftime 函数将 tv 转换为字符串，但 strftime 函数只能达到秒级精度
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    //将毫秒值追加到 strftime 转换的字符串末尾
    snprintf(buf, sizeof buf, "%s.%06d", tmbuf, tv.tv_usec);
    //fprintf(option->pRxLogfp, "-- %s --\n", buf);

	printf("\n\n\n");
#endif
	recvlen = LTEV2X_Recv(option->m_ltev2x_hdl, p_pcBuf, LTEV2X_RECV_BUF_LEN, &(option->nRecvPower));
#if 0
	if (recvlen > 0)
	{
		printf("The received data is list like below------\n");
		int i = 0;

		fprintf(option->pRxLogfp, "[ ");
		for (i = 0; i < recvlen; i++)
		{
			if (i % 16 == 0)
			{
				printf("\n");
				printf("i = %3d | ", i);
			}
			printf("%02X ", p_pcBuf[i]);
			fprintf(option->pRxLogfp, "%02X ", p_pcBuf[i]);
		}
		fprintf(option->pRxLogfp, "]");

		printf("\n");
	}
#endif
#if 0

	MessageFrame_t *pSendMsg = NULL;
	if (recvlen > 0)
	{
		// decode the recvbuf and write into log file
		asn_dec_rval_t ret;
		fprintf(option->pRxLogfp, "\n");

		ret = uper_decode(0, &asn_DEF_MessageFrame, (void **) &pSendMsg, p_pcBuf,
				recvlen, 0, 0);
		if (ret.code != RC_OK) {
			printf("RecvBuf Decode Failed\n ");
			fprintf(option->pRxLogfp, "RecvBuf Decode Failed\n ");
			fprintf("Broken encoding at byte %ld\n", (long) ret.consumed);
		}
		xer_fprint(option->pRxLogfp, &asn_DEF_MessageFrame, pSendMsg);
	}
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);
#endif
	fprintf(option->pRxLogfp, "\n\n");
	fflush(option->pRxLogfp);

	return recvlen;
}
