/*
 * V2X_INTERFACE.c
 *
 *  Created on: May 9, 2018
 *      Author: root
 */
#include "V2X_INTERFACE.h"
pthread_mutex_t g_mutex_V2XSendData;

/************************************************************************
 * 初始化V2X通讯_发送模块
 * 函数名称：int V2X_Tx_Init()
 * 函数功能：初始化V2X通讯
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 ************************************************************************/
int V2X_Tx_Init()
{
	int Res = 0;

	//初始化互斥信号量
	pthread_mutex_init(&g_mutex_V2XSendData, NULL);
#if MODE_LTEV_API | MODE_LTEV_IP
	LTE_V2X_CommunInit(&lte_v_option);
	LTE_V2X_ParamInit();
	if(Res){
		goto Error;
	}
#endif
#if MODE_LTEV_RAW
	Res = Raw_init("lmi40");
	if(Res <= 0){
		goto Error;
	}
	else
		Res = 0;
#endif
Error:
	return Res;
}
/************************************************************************
 * 初始化V2X通讯_接收模块
 * 函数名称：int V2X_Rx_Init(pFunRecvData *MsgRx_CallBack)
 * 函数功能：初始化V2X通讯
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 ************************************************************************/
int V2X_Rx_Init(pFunRecvData MsgRx_CallBack)
{
	int Res = 0;
#if MODE_LTEV_API | MODE_LTEV_IP | MODE_LTEV_RAW
	Res = LTE_V2X_Rx_SetDevParam(MsgRx_CallBack);
	if(Res){
		Res = 4;
		goto Error;
	}
#endif

Error:
	return Res;
}

/*********************************************************************
 * 设备参数读取
 * 函数名称：int V2X_GetParam(DevParam_t *pDevParam)
 * 函数功能：用户获取设备当前运行参数，并通过函数输出参数传出；
 * 参数说明：DevParam_t *pDevParam 输出参数，设备当前运行参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int V2X_GetParam(DevParam_t *pDevParam)
{
	int Res = 0;

#if MODE_LTEV_API | MODE_LTEV_IP
	pDevParam->present = DevParam_PR_LTEV;
	memcpy(&(pDevParam->choice.DevParam_LTEV.m_V2XOptions.m_ltevOpts), &lte_v_option.m_ltevOpts, sizeof(LTEV2X_Option));
	if(Res){
		goto Error;
	}
#endif

#if MODE_LTEV_RAW
	if(Res){
			goto Error;
		}
#endif

	Error:
	return Res;
}
/*********************************************************************
 * 设备参数配置
 * 函数名称：int V2X_SetParam(DevParam_t *pDevParam)
 * 函数功能：按照用户定义的设备参数修改配置数据库中的配置参数，并重新配置参数；
 * 参数说明：DevParam_t *pDevParam 输出参数，设备当前运行参数。
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 *********************************************************************/
int V2X_SetParam(DevParam_t *pDevParam)
{
	int Res = 0;

#if MODE_LTEV_API | MODE_LTEV_IP
	pDevParam->present = DevParam_PR_LTEV;
	pDevParam->choice.DevParam_LTEV.m_V2XOptions.m_ltev2x_hdl = lte_v_option.m_ltev2x_hdl;
	memcpy( &lte_v_option, &(pDevParam->choice.DevParam_LTEV.m_V2XOptions),sizeof(LTE_V2X_Options));
	if(Res){
		goto Error;
	}
#endif

#if MODE_LTEV_RAW
	if(Res){
			goto Error;
		}
#endif
	Error:
	return Res;
}
/*********************************************************************
 * 发送指定数据
 * 函数名称：int V2X_SendData(char* p_pSendData, int nDataLen)
 * 函数功能：将指定的数据按照当前的发送参数发送出去
 * 参数说明：char* p_pSendData 发送数据指针，int nDataLen 发送数据长度
 * 函数返回值：>0 成功发送数据的长度（字节数） <0 表示发送失败，返回结果代表错误类型
 *********************************************************************/
int V2X_SendData(int nAid,char* p_pSendData, int nDataLen)
{
	int nRet = 0;
	pthread_mutex_lock(&g_mutex_V2XSendData);

#if MODE_LTEV_API | MODE_LTEV_IP
	nRet = LTE_V2X_Tx_SendData(p_pSendData, nDataLen);
#endif

#if MODE_LTEV_RAW
	nRet = RawSendData(nAid,p_pSendData, nDataLen);
#endif
	pthread_mutex_unlock(&g_mutex_V2XSendData);
	if(nRet > 0);

	return 0;
}
/************************************************************************
 * 停止接收数据
 * 函数名称：int V2X_Close();
 * 函数功能：关闭V2X通讯
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 ************************************************************************/
int V2X_Close()
{
	int Res = 0;

#if MODE_LTEV_API | MODE_LTEV_IP

#endif

#if MODE_LTEV_RAW
	Raw_release();
#endif

	return Res;
}




