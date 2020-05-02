#include "V2X_RecvRSI.h"
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int RTEcpy(tRsiMsg_Arry lRsiInfo_Arry, int nIndex, tRsiMsg *pRsiInfo) {
	pRsiInfo->RSUID = lRsiInfo_Arry.RSUID;
	///消息ID
	pRsiInfo->RSIMsgID = lRsiInfo_Arry.Al_RTEData[nIndex].RTEID;
	///消息类型
	pRsiInfo->MsgType = lRsiInfo_Arry.Al_RTEData[nIndex].MsgType;
	///优先级
	pRsiInfo->Priority = lRsiInfo_Arry.Al_RTEData[nIndex].Priority;
	///消息描述
	memcpy(pRsiInfo->Description, lRsiInfo_Arry.Al_RTEData[nIndex].Description,
			128);
	///消息半径(m)
	pRsiInfo->AlertRadius = lRsiInfo_Arry.Al_RTEData[nIndex].AlertRadius;
	///参考位置
	pRsiInfo->AlertPos = lRsiInfo_Arry.Al_RTEData[nIndex].AlertPos;

	///消息轨迹点个数
	pRsiInfo->AlertPathNbr = lRsiInfo_Arry.Al_RTEData[nIndex].AlertPathNbr;
	///消息点位置
	//RSIPathPt AlertPathList[8];
	int nTmpj = 0;
	for (nTmpj = 0; nTmpj < lRsiInfo_Arry.Al_RTEData[nIndex].AlertPathNbr;
			nTmpj++) {
		pRsiInfo->AlertPathList[nTmpj] =
				lRsiInfo_Arry.Al_RTEData[nIndex].AlertPathList[nTmpj];
	}
	pRsiInfo->AlertPathList[0].AlertPath = pRsiInfo->AlertPos;
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int RTScpy(tRsiMsg_Arry lRsiInfo_Arry, int nIndex, tRsiMsg *pRsiInfo) {
	pRsiInfo->RSUID = lRsiInfo_Arry.RSUID;
	///消息ID
	pRsiInfo->RSIMsgID = lRsiInfo_Arry.Al_RTSData[nIndex].RTSID;
	///消息类型
	pRsiInfo->MsgType = lRsiInfo_Arry.Al_RTSData[nIndex].MsgType;
	///优先级
	pRsiInfo->Priority = lRsiInfo_Arry.Al_RTSData[nIndex].Priority;
	///消息描述
	memcpy(pRsiInfo->Description, lRsiInfo_Arry.Al_RTSData[nIndex].Description,
			128);
	///消息半径(m)
	pRsiInfo->AlertRadius = lRsiInfo_Arry.Al_RTSData[nIndex].AlertRadius;
	///参考位置
	pRsiInfo->AlertPos = lRsiInfo_Arry.Al_RTSData[nIndex].AlertPos;

	///消息轨迹点个数
	pRsiInfo->AlertPathNbr = lRsiInfo_Arry.Al_RTSData[nIndex].AlertPathNbr;
	///消息点位置
	//RSIPathPt AlertPathList[8];
	int nTmpj = 0;
	for (nTmpj = 0; nTmpj < lRsiInfo_Arry.Al_RTSData[nIndex].AlertPathNbr;
			nTmpj++) {
		pRsiInfo->AlertPathList[nTmpj] =
				lRsiInfo_Arry.Al_RTSData[nIndex].AlertPathList[nTmpj];
	}
	pRsiInfo->AlertPathList[0].AlertPath = pRsiInfo->AlertPos;
	return 0;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void *Thread_DealRSI(void * arg) {
	MessageFrame_t *pRSIMsg;

	tRsiMsg lRsiInfo;
	tRsiMsg_Arry lRsiInfo_Arry;
	tVehData HV;
	tResult_Rsi lResult_Rsi;
	uint32_t reslen;
	uint8_t res[1024];

	AppDupRSI_id();
	int nTxAid = 0;
	while (1) {
		usleep(1000);
		sem_wait(&g_Queue_Recv_rsi.m_product_number);

		//获取RSI数据
		pRSIMsg = ALLOC_OBJ(MessageFrame_t, 1);
		GetLstRsiData(&nTxAid, pRSIMsg);
		if (pRSIMsg->present != MessageFrame_PR_rsiFrame) {
			printf("RSI continue;\n");
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRSIMsg);
			continue;
		}
		//获取RSI相关信息
		//GetRsiInfo(pRSIMsg, &lRsiInfo);
		GetRsiInfo(pRSIMsg, &lRsiInfo_Arry);

		ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRSIMsg);

		//获取本车当前信息
		getHvInfo(&HV);

		int nTmpi = 0;
		for (nTmpi = 0;
				nTmpi
						< lRsiInfo_Arry.Al_RTEDataNum
								+ lRsiInfo_Arry.Al_RTSDataNum; nTmpi++) {
			usleep(3 * 1000);
#define RTEMSG 1
#define RTSMSG 2
			int nType = nTmpi - lRsiInfo_Arry.Al_RTEDataNum < 0 ?
			RTEMSG :
																	RTSMSG;
			switch (nType) {
			case RTEMSG:
				RTEcpy(lRsiInfo_Arry, nTmpi, &lRsiInfo);
				break;
			case RTSMSG:
				RTScpy(lRsiInfo_Arry, nTmpi - lRsiInfo_Arry.Al_RTEDataNum,
						&lRsiInfo);
				break;
			default:
				break;
			}
			if (g_IsMapMode == 1) //不使用地图模式
					{
				lResult_Rsi = IVS(&HV, &lRsiInfo);
//					if (lResult_Rsi.dis > 2 * g_V2I_Parameter.IVSWarnDis)
//						lResult_Rsi.RelPos = 44;
			} else //使用地图
			{
				MapMatch_Pojcet(g_MapInfo, &HV);
				lResult_Rsi = IVS_MapMatch(&HV, &lRsiInfo, g_MapInfo);
			}

			//DataBase_Insert_RSI(sqldb, HV, lRsiInfo, lResult_Rsi);
			if (lResult_Rsi.wStr != V2I_STR_DEFAULT) 
			{
				//EncodeD4(HV, lRsiInfo, lResult_Rsi, res, &reslen);
				EncodeD4_YuTong(HV, lRsiInfo, lResult_Rsi, res, &reslen);
				fun_SendData2Pad((nint8_t *) res, reslen);
			}
			AddRsiInfo(&g_UpD9Data, lRsiInfo, lResult_Rsi);
		}
		ClearRsiInfo(&lRsiInfo_Arry);

		usleep(1000);
	}
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Fun_Start_DealRSI(tStateStore *pApp) {
	if (g_ReadHisDataBase.u32HisDataBaseIndex_RSI == 0) {
		pthread_create(&tidp_DealRSI, NULL, Thread_DealRSI, pApp);
	}
	return 0;
}
