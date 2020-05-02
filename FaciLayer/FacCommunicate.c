#include "FacCommunicate.h"

#include "BsmTx.h"
#include "RsiTx.h"
#include "MapTx.h"
#include "SpatTx.h"
#include "RsmTx.h"
#include "FacMsgDup.h"

/**
 * 函数名 :  AppDSMIint_rq()
 * 功能描述: DSM消息初始化请求，获取相关设置参数
 **/
int AppDSMInit_rq()
{
	return DCR_OPTDSMINIT;
}

/**
 * 函数名 :  AppDSMIint_rs()
 * 功能描述: 系统初始化设置
 **/
int AppDSMInit_rs(int Answer)
{
	int Res = Init_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = Init_ERR_REJECT;
		goto Error;
	}
//	char *nAidInfo = NULL;
	int nAidCnt = 0;

	uint16_t nRxAid[10] = {0};
	sscanf(Store.HostComInfo.RxAidLst, "%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu",
			&nRxAid[0], &nRxAid[1], &nRxAid[2], &nRxAid[3], &nRxAid[4], &nRxAid[5], &nRxAid[6], &nRxAid[7],
			&nRxAid[8], &nRxAid[9]);
	printf("Store.HostComInfo.RxAidLst = %s\n",Store.HostComInfo.RxAidLst);
	while(1)
	{
		if(nRxAid[nAidCnt] == 0)
			break;
		nAidCnt ++;
	}
//	Res = SecLayer_Init(Store.HostComInfo.RxEnable,nRxAid,nAidCnt);
//	if (Res)
//	{
//		Res = Init_ERR_TxINIT_FAIL;
//		goto Error;
//	}

	printf("Store.HostComInfo.RxEnable = %d\n", Store.HostComInfo.RxEnable);
	if (Store.HostComInfo.RxEnable)
	{
		memset(&g_Queue_Recv_bsm, 0x00, sizeof(tQueue_RevLst));
		sem_init(&g_Queue_Recv_bsm.m_blank_number, 0, NUM);
		sem_init(&g_Queue_Recv_bsm.m_product_number, 0, 0);

		memset(&g_Queue_Recv_rsi, 0x00, sizeof(tQueue_RevLst));
		sem_init(&g_Queue_Recv_rsi.m_blank_number, 0, NUM);
		sem_init(&g_Queue_Recv_rsi.m_product_number, 0, 0);

		memset(&g_Queue_Recv_map, 0x00, sizeof(tQueue_RevLst));
		sem_init(&g_Queue_Recv_map.m_blank_number, 0, NUM);
		sem_init(&g_Queue_Recv_map.m_product_number, 0, 0);

		memset(&g_Queue_Recv_spat, 0x00, sizeof(tQueue_RevLst));
		sem_init(&g_Queue_Recv_spat.m_blank_number, 0, NUM);
		sem_init(&g_Queue_Recv_spat.m_product_number, 0, 0);

		memset(&g_Queue_Recv_rsm, 0x00, sizeof(tQueue_RevLst));
		sem_init(&g_Queue_Recv_rsm.m_blank_number, 0, NUM);
		sem_init(&g_Queue_Recv_rsm.m_product_number, 0, 0);
		FaciLayer_Recv();
	}

	if(Store.HostComInfo.RxEnable)
		SecLayer_InitRx(Store.WaveConfig, 1);
	else
		SecLayer_InitRx(Store.WaveConfig, 0);
	Error: return Res;

}

/**
 * 函数名 :  AppSendDSMMsg_rs()
 * 功能描述: 将请求发送的消息类型发送出去
 **/
int AppSendDSMMsg_rq()
{
	return DCR_OPTDSMSEND;
}

int AppSendDSMMsg_rs(int Answer, tStateStore *pStore)
{

	int Res = Send_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = Send_ERR_REJECT;
	}

	/// Thread attributes variable
	pthread_attr_t Attr;
	pthread_attr_init(&Attr);

	//create TxLog
	extern int errno;
	char *path = "./TxLog";
	if ((access(path, F_OK)) == -1)
	{
		if (mkdir(path, 0766) == 0)
		{
			printf("created the directory %s . \n", path);
		}
		else
		{
			printf("can't creat the directoty %s.\n", path);
			printf("errno : %d\n", errno);
			printf("ERR : %s\n", strerror(errno));
		}
	}

	switch (pStore->WaveConfig.AID)
	{

	//AID = 17,车车／车路安全应用模式
	case AID_VEHICLE_SAFETY:
	case 20:
		//Open BSM Message Send
		if (pStore->HostComInfo.BsmTx.TxEnable)
		{
			printf("Open BSM Tx ....\n");

			Res = BsmTx_Open(&(pStore->Hdl.pBSMTx), &Attr);
			if (Res != 0)
			{
				printf("BSMTx Open Failed, Error code is %d !!!\n", Res);
				goto Error;
			}
			else
			{
				printf("BSMTx Open Success !!!\n");
			}
		}

		//Open RSI Message Send
		if (pStore->HostComInfo.RsiTx.TxEnable)
		{
			printf("Open RSI Tx ....\n");

			Res = FG_RsiTxOpen(&(pStore->Hdl.pRSITx), &Attr);
			if (Res != 0)
			{
				printf("RSITx Open Failed, Error code is %d !!!\n", Res);
				goto Error;
			}
			else
			{
				printf("RSITx Open Success !!!\n");
			}
		}

		//Open RSM Message Send
		if (pStore->HostComInfo.RsmTx.TxEnable)
		{
			printf("Open RSM Tx ....\n");

			Res = FG_RsmTxOpen(&(pStore->Hdl.pRSMTx), &Attr);
			if (Res != 0)
			{
				printf("RSMTx Open Failed, Error code is %d !!!\n", Res);
				goto Error;
			}
			else
			{
				printf("RSMTx Open Success !!!\n");
			}
		}

		//Open MAP Message Send
		if (pStore->HostComInfo.MapTx.TxEnable)
		{
			printf("Open MAP Tx ....\n");

			Res = MapTx_Open(&(pStore->Hdl.pMAPTx), &Attr);
			if (Res != 0)
			{
				printf("MAPTx Open Failed, Error code is %d !!!\n", Res);
				goto Error;
			}
			else
			{
				printf("MAPTx Open Success !!!\n");
			}
		}

		//Open SPAT Message Send
		if (pStore->HostComInfo.SpatTx.TxEnable)
		{
			printf("Open Spat Tx ....\n");
			Res = FG_SpatTxOpen(&(pStore->Hdl.pSPATTx), &Attr);
			if (Res != 0)
			{
				printf("SpatTx Open Failed, Error code is %d !!!\n", Res);
				goto Error;
			}
			else
			{
				printf("SpatTx Open Success !!!\n");
			}
		}
		break;

		//AID = 135 V2X业务公告应用
//	case AID_DSRC_SERVICE_ADV:

//		//Open Dsa Msg Tx
//		if (pStore->HostComInfo.m_DsaTx.TxEnable)
//		{
//			printf("Open DSA Tx ....\n");
//
//			Res = DsaTx_Open(&(pStore->Hdl.pDsaTx), &Attr);
//			if (Res != 0)
//			{
//				printf("DsaTx Open Failed, Error code is %d !!!\n", Res);
//				goto Error;
//			}
//			else
//			{
//				printf("DsaTx Open Success !!!\n");
//			}
//		}
//
//		break;
	case AID_NETLAYER_TEST:
		//Get In Test Mode

		break;
	default:
		break;
	}

	Error: return Res;
}

int AppDSMTerminate_rq()
{
	return DCR_OPTDSMCLOSE;
}

int AppDSMTerminate_rs(int Answer)
{
	int Res = Init_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = Init_ERR_REJECT;
		goto Error;
	}

	sleep(1);

	//Close Rx Interface
	Res = V2X_Close();

	Error: return Res;
}

