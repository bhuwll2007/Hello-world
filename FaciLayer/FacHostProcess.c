#include "FacHostProcess.h"

DevParam_t g_DevParam;

/*
 获取主机属性信息请求
 */
int AppGetHostInfo_rq()
{
	return DCR_OPTHOSTINFO;
}

/**
 * 函数名 :  AppGetHostInfo_rs()
 * 功能描述: 返回设备的基本属性数据ID， 如查询OBU的GNSS数据、或者RSU的消息发送类型
 * 输入参数: Res 请求结果标识位
 * 输出参数: HostInfo 主机信息结构体
 * 返回值 :0 查询成功， 其他 失败理由
 **/
int AppGetHostInfo_rs(int Answer, tHostInfo *pHostInfo)
{
	int Res = Process_ERR_NONE;

	GpsLocation *pGpsData = NULL;
	//whether Send request accepted
	if (Answer)
	{
		Res = Process_ERR_REJECT;
		goto Error;
	}

	//Copy the Store HostAttriInfo
	memcpy(&pHostInfo->HostAttriInfo, &Store.HostAttriInfo,
			sizeof(tHostAttriInfo));

	//Get GPS Information
	pGpsData = ALLOC_OBJ(GpsLocation, 1);
	Res = GPS_GetData(pGpsData);
	if (Res)
	{
		Res = MSG_ERR_INTERFACE;
		goto Error;
	}
	memcpy(&pHostInfo->GnssInfo, pGpsData, sizeof(GpsLocation));
	Error: if (pGpsData != NULL)
		free(pGpsData);
	return Res;
}

/*
 获取主机通信状态信息请求
 */
int AppGetCommStatus_rq()
{
	return DCR_OPTGETCOMMINFO;
}

/**
 * 函数名 :  AppGetCommStatus_rs(int Res, char *pCommStatus)
 * 功能描述: 获取设备通信状态属性及状态信息
 * 输入参数: Res 请求结果标识位
 * 输出参数: CommStatus 设备通信参数结构体
 * 返回值 :  0 查询成功， 其他 失败理由
 **/
int AppGetCommStatus_rs(int Answer, tCommStatus *pCommStatus)
{
	int Res = Process_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = Process_ERR_REJECT;
		goto Error;
	}
	Res = V2X_GetParam(&g_DevParam);
	if (Res)
	{
		goto Error;
	}
//	//Get Channel parameter
//	Res =  DSRC_V2X_ChnConf_GetParam(&pCommStatus->pCHOpts);
//	if(Res){
//		Res = Process_ERR_GetChnCfg;
//		goto Error;
//	}
//	//Get Tx parameter
//	Res =  DSRC_V2X_Tx_GetParam(&pCommStatus->pTxOpts);
//	if(Res){
//		Res = Process_ERR_GetTxCfg;
//		goto Error;
//	}
//	//Get Rx parameter
//	Res =  DSRC_V2X_Rx_GetParam(&pCommStatus->pRxOpts);
//	if(Res){
//		Res = Process_ERR_GetRxCfg;
//		goto Error;
//	}
//	if (g_DevParam.present == DevParam_PR_DSRC)
//		memcpy(&pCommStatus->pCHOpts, &g_DevParam.choice.DevParam_DSRC.m_CHOpts,
//				sizeof(tCHOpts) + sizeof(tTxOpts) + sizeof(tRxOpts));
//	else if (g_DevParam.present == DevParam_PR_LTEV)
//		memcpy(&pCommStatus->pCHOpts,
//				&g_DevParam.choice.DevParam_LTEV.m_V2XOptions.m_ltevOpts,
//				sizeof(LTEV2X_Option));
	//Get Host Communication State Parameter
	memcpy(&pCommStatus->CommInfo, &Store.HostComInfo, sizeof(tHostComInfo));
	Error: return Res;
}

/*
 配置主机通信参数
 */
int AppSetCommCfg_rq()
{
	return DCR_OPTSETCOMMCFG;
}

/**
 * 函数名 :  AppGetCommStatus_rs()
 * 功能描述: 获取设备通信状态属性及状态信息
 * 输入参数: Res 请求结果标识位； pCommCfg 通信设置参数指针
 * 输出参数:
 * 返回值 :  0 设置成功， 其他 失败理由
 **/
int AppSetCommCfg_rs(int Answer, char *pCommCfg)
{
	int Res = Process_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = Process_ERR_REJECT;
		goto Error;
	}
//	tCommStatus *pCommStatus = (tCommStatus *)pCommCfg;

//	Res =  DSRC_V2X_ChnConf_SetParam(&pCommStatus->pCHOpts);
//	if(Res){
//		Res = Process_ERR_SetChnCfg;
//		goto Error;
//	}
//
//	Res =  DSRC_V2X_Tx_SetParam(&pCommStatus->pTxOpts);
//	if(Res){
//		Res = Process_ERR_SetTxCfg;
//		goto Error;
//	}
//
//	Res =  DSRC_V2X_Rx_SetParam(&pCommStatus->pRxOpts);
//	if(Res){
//		Res = Process_ERR_SetRxCfg;
//		goto Error;
//	}
//	memcpy(&Store.HostComInfo, &pCommStatus->CommInfo, sizeof(tHostComInfo));

	Res = V2X_SetParam(&g_DevParam);
	if (Res)
	{
		goto Error;
	}

	Error: return Res;
}
