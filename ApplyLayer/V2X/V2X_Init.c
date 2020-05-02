/*
 * V2X_Init.c
 *
 *  Created on: May 31, 2018
 *      Author: root
 */

#include "V2X_Init.h"

/*****************************************************************************
 * 函数名称:V2X_Init
 * 功能描述:初始化V2X收发数据模块
 * 输入参数:tStateStore *pApp 设备功能层参数句柄
 * 输出参数:无
 * 返 回 值:初始化V2X收发数据模块
 * 创建日期:
 *****************************************************************************/
int V2X_Init(tStateStore *pApp)
{
	char* conf_file = "./HostInfo.ini";

	int Res = 0;
	int nRet = 0;
	memset(pApp, 0x00, sizeof(tStateStore));

	pApp->Hdl.pBSMTx = ALLOC_OBJ(struct BsmTx,1);
	pApp->Hdl.pRSMTx = ALLOC_OBJ(struct RsmTx,1);
	pApp->Hdl.pRSITx = ALLOC_OBJ(struct RsiTx,1);
	pApp->Hdl.pMAPTx = ALLOC_OBJ(struct MapTx,1);
	pApp->Hdl.pSPATTx = ALLOC_OBJ(struct SpatTx,1);
	printf("Bsm = %d Rsm = %d Rsi = %d Map = %d Spat = %d\n",sizeof(struct BsmTx),sizeof(struct RsmTx),sizeof(struct RsiTx),sizeof(struct MapTx),sizeof(struct SpatTx));

//Load Config
	HostInfo_LoadConfig(pApp, conf_file);

//Init Communicate modelc
	Res = AppDSMInit_rq();

	Res = AppDSMInit_rs(Res);
	if (Res)
	{
		printf("Init Failed, ErrorCode = %d\n", Res);
	}
	else
	{
		printf("Init Success!!!\n\n");
	}

	sleep(1);
	//Open Message Send Model
	Res = AppSendDSMMsg_rq();
	AppSendDSMMsg_rs(Res, pApp);

#if OLD_SPAT_MODE
	if (Store.HostComInfo.SpatTx.TxEnable == 1)
	{
		StcTcpClient *pTcpClient;
		pTcpClient = calloc(1, sizeof(StcTcpClient));

		int nRet = 0;
		nRet = TcpClt_InitSoc(pTcpClient);
		if (nRet < 0)
		{
			return -1;
		}
		pTcpClient->pCallBackDealData = &FG_SpatTxCallBack_Old;
		nRet = TcpClt_LinkSer(pTcpClient, Store.HostComInfo.SpatTx.ServerIp,
				Store.HostComInfo.SpatTx.ServerPort);
		if (nRet < 0)
		{
			return -2;
		}
		printf("Link TCP Server Success !!! \n\n");
	}
#else
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~pApp->HostComInfo.SpatTx.TxEnable = %d\n",pApp->HostComInfo.SpatTx.TxEnable );
	if (pApp->HostComInfo.SpatTx.TxEnable == 1)
	{
		// Spat连接初始化
		nRet = FG_SpatInit(&Store);
		if (nRet == 0)
		{
			HLOG("Link Spat Server Success !!!");
		}
		else
		{
			HLOG("Link Spat Server Error !!!");
			return nRet;
		}
	}
#endif
	if (pApp->HostComInfo.RsmTx.TxEnable == 1)
	{
		//RSM连接初始化
		nRet = FG_RsmUdpSerInit(5001);
		if (nRet == 0)
		{
			HLOG("Rsm Udp Server Init Success !!!");
		}
		else
		{
			HLOG("Rsm Udp Server Init Error !!!");
			return nRet;
		}
	}

	if(g_tYuTong_RsuBaseInfo.Is_Send2Cloud == 1)
	{
		nRet = FG_YuTongCloudInit(&Store);
		if (nRet > 0)
		{
			HLOG("FG_YuTongCloudInit Tcp Client Init Success !!!");
		}
		else
		{
			HLOG("FG_YuTongCloudInit Tcp Client Init Error !!!");
			//return nRet;
		}
	}

	//如果开启接收数据
	if (Store.HostComInfo.RxEnable == 1)
	{
		Fun_Start_DealMAP(pApp);
		//Fun_Start_DealBSM(pApp);
		//Fun_Start_DealRSI(pApp);
		//Fun_Start_DealRSM(pApp);
		Fun_Start_DealSPAT(pApp);
	}
	return 0;
}

