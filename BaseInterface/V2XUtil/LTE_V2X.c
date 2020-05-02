#include "LTE_V2X.h"

//保证LTEV发送数据操作互斥信号标志
#define LTE_V_SEND 5010

LTE_V2X_Options lte_v_option;
/*********************************************************************
 * 信道配置参数读取
 * 函数名称：int LTE_V2X_ChnConf_GetParam(char* p_pChnConfParam)
 * 函数功能：用户获取程序当前的信道配置参数，并通过函数输出参数传出；
 * 参数说明：tCHOpts* pCHOpts 输出参数，程序当前使用的信道配置参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int LTE_V2X_CommunInit(LTE_V2X_Options *lte_v_option)
{
	int ret = 0;
#if MODE_LTEV_API
	lte_v_option->LTE_V_WorkMode = 0;
	ret = api_init(lte_v_option);
#endif
#if MODE_LTEV_IP
	lte_v_option->m_ltevOpts.ue_id = 12;
	lte_v_option->m_ltevOpts.ps_id = 0;
	lte_v_option->m_ltevOpts.tx_power = 20;
	lte_v_option->m_ltevOpts.priority = 7;
	lte_v_option->m_ltevOpts.period = 0;
	lte_v_option->m_ltevOpts.proc_id = 0;
	lte_v_option->m_ltevOpts.data_len = 200;
	lte_v_option->m_ltevOpts.test_mode = 0;
	lte_v_option->m_ltevOpts.test_sfn = 0;
//	lte_v_option->LTE_V_WorkMode = 1;
	ret = ip_init(lte_v_option);
#endif

	return ret;
}
/*********************************************************************
 * 信道配置参数读取
 * 函数名称：int LTE_V2X_ChnConf_GetParam(char* p_pChnConfParam)
 * 函数功能：用户获取程序当前的信道配置参数，并通过函数输出参数传出；
 * 参数说明：tCHOpts* pCHOpts 输出参数，程序当前使用的信道配置参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int LTE_V2X_GetParam(LTEV2X_Option *pLTE_Option)
{
	return 0;
}
/*********************************************************************
 * 信道配置参数配置
 * 函数名称：int LTE_V2X_ChnConf_SetParam(char* p_pChnConfParam)
 * 函数功能：按照用户定义的信道配置参数修改配置数据库中的信道配置参数，并重新读取配置参数至三个全局变量；
 * 参数说明：tCHOpts* pCHOpts 输入参数，用户新定义的信道配置参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int LTE_V2X_SetParam(LTEV2X_Option *pLTE_Option)
{
	return 0;
}

/*********************************************************************
 * 信道配置初始化
 * 函数名称：int LTE_V2X_ParamInit()
 * 函数功能：包括读取配置参数中默认的信道配置参数等
 * 参数说明：无
 * 函数返回值：信道配置初始化状态，0成功，非0失败
 *********************************************************************/
int LTE_V2X_ParamInit()
{
	//初始化配置参数数据库
//	InitConfDb();
	return 0;
}
/*********************************************************************
 * 信道配置参数读取
 * 函数名称：int LTE_V2X_ChnConf_GetParam(char* p_pChnConfParam)
 * 函数功能：用户获取程序当前的信道配置参数，并通过函数输出参数传出；
 * 参数说明：tCHOpts* pCHOpts 输出参数，程序当前使用的信道配置参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
void (*g_FunRecvData)(char *p_cRecvBuf, int *p_nRecvBufSize);
/*********************************************************************
 * 发送指定数据
 * 函数名称：int LTE_V2X_Tx_SendData(char* p_pSendData, int nDataLen)
 * 函数功能：将指定的数据按照当前的发送参数发送出去
 * 参数说明：char* p_pSendData 发送数据指针，int nDataLen 发送数据长度
 * 函数返回值：成功发送数据的长度（字节数）
 *********************************************************************/
int LTE_V2X_Tx_SendData(char* p_pSendData, int nDataLen)
{

#if 1
	struct timeval tv3;
	gettimeofday(&tv3, NULL);
	printf("Time 3: tv.tv_sec = %ld,tv.tv_usec = %ld\n", tv3.tv_sec, tv3.tv_usec);
#endif
	int nSendSize = 0;
#if MODE_LTEV_API
	nSendSize = api_write(&lte_v_option, p_pSendData, nDataLen);
#endif
#if MODE_LTEV_IP
	nSendSize = ip_write(&lte_v_option, p_pSendData, nDataLen);
#endif
	return nSendSize;
}

void *LTEV_Thread_RecvData(void * arg)
{
	char buf[LTEV2X_RECV_BUF_LEN] =
	{ 0 };
//	char *buf = NULL;
	int byteNbr = 0;

	while (1)
	{
#if MODE_LTEV_API
		byteNbr = api_read(&lte_v_option, buf);  // api mode
#endif
#if MODE_LTEV_IP
		byteNbr = ip_read(&lte_v_option, buf);  // ip mode
#endif
#if MODE_LTEV_RAW
		byteNbr = RawRecvData(buf);  // ip mode
#endif
		if (byteNbr > 0)
		{
#if 0
			struct timeval tv0;
			gettimeofday(&tv0, NULL);
			printf("Time 0: tv.tv_sec = %ld,tv.tv_usec = %ld\n", tv0.tv_sec,
					tv0.tv_usec);
#endif
			if (g_FunRecvData != NULL)
				g_FunRecvData(buf, &byteNbr);
		}
		usleep(1000);
	}

	return NULL;
}
/*********************************************************************
 * 开启接收数据
 * 函数名称：int LTE_V2X_Rx_SetDevParam()
 * 函数功能：将当前接收参数配置到设备中，进入接收数据状态，如果接收到数据，则进行数据处理；
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int LTE_V2X_Rx_SetDevParam(
		void (*pFunRecvData)(char *p_cRecvBuf, int *pnRecvBufSize))
{
	pthread_t tidp_OneTouch;
	// Wait for and process all received frames
	pthread_create(&tidp_OneTouch, NULL, LTEV_Thread_RecvData, NULL); //开启接收线程接收tcp传来的数据

	g_FunRecvData = pFunRecvData;
	return 0;
}

