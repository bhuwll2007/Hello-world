/*
 * Encode2Pad_V2.c
 *
 *  Created on: Jan 6, 2020
 *      Author: wanijv2x
 */

#include "Encode2Pad_V2.h"

/****************************************************
 * 函数名称:int EncodeD0(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
 * 功能描述:将UpD0Data结构体的数据按照D0帧协议帧格式组帧，并存放到缓存desbuf中，缓存长度为deslen
 * 输入参数:UpD0Data pUpD0Data：周围车辆信息结构体变量,
 * 输出参数:uint8_t *desbuf：D0帧内容, uint32_t *deslen：D0帧内容长度
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD0_V2(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t srcbuf[1024];
	uint32_t srclen = 0;
	uint8_t ntmpi = 0;
	stcSelVeh_V2 lstcSelVeh_V2;
	memcpy(lstcSelVeh_V2.UEID, pUpD0Data.SlfVehData.UEID,
			sizeof(pUpD0Data.SlfVehData.UEID));

	lstcSelVeh_V2.nSelfLat = (nint32_t) (pUpD0Data.SlfVehData.dSelfLat
			* 10000000);			//自己的坐标的纬度 单位：1e-7度
	lstcSelVeh_V2.nSelfLng = (nint32_t) (pUpD0Data.SlfVehData.dSelfLng
			* 10000000);			//自己的坐标的经度 单位：1e-7度
	lstcSelVeh_V2.nSelfSpeed = pUpD0Data.SlfVehData.nSelfSpeed;	//自己的速度	单位：km/h
	lstcSelVeh_V2.nLimitHighSpeed = pUpD0Data.SlfVehData.nLimitHighSpeed;//当前道路限制的最高速度	无限速时默认200，单位：km/h
	lstcSelVeh_V2.nSelfTrack = pUpD0Data.SlfVehData.nSelfTrack;		//自己的方向	单位：度
	lstcSelVeh_V2.nSpatDistence = pUpD0Data.SlfVehData.nSpatDistence;//距离信号机的距离		单位：米

	stcArdVeh_V2 lstcArdVeh_V2[MAXAROUNDVEHNUM];
	for (ntmpi = 0; ntmpi < pUpD0Data.nAroundVehNum; ntmpi++)
	{
		memcpy(lstcArdVeh_V2[ntmpi].UEID, pUpD0Data.ArdVehData[ntmpi].UEID,
				sizeof(pUpD0Data.ArdVehData[ntmpi].UEID));/** 识别物ID. */
		lstcArdVeh_V2[ntmpi].IsLive = pUpD0Data.ArdVehData[ntmpi].IsLive;
		lstcArdVeh_V2[ntmpi].IsSafe = pUpD0Data.ArdVehData[ntmpi].IsSafe;
		//printf("pUpD0Data.ArdVehData[ntmpi].dDuiLat = %f ",pUpD0Data.ArdVehData[ntmpi].dDuiLat);
		//printf("pUpD0Data.ArdVehData[ntmpi].dDuiLng = %f ",pUpD0Data.ArdVehData[ntmpi].dDuiLng);
		lstcArdVeh_V2[ntmpi].nDuiLat =
				(nint32_t) (pUpD0Data.ArdVehData[ntmpi].dDuiLat * 10000000);
		lstcArdVeh_V2[ntmpi].nDuiLng =
				(nint32_t) (pUpD0Data.ArdVehData[ntmpi].dDuiLng * 10000000);
		lstcArdVeh_V2[ntmpi].nDistance = pUpD0Data.ArdVehData[ntmpi].nDistance;
		lstcArdVeh_V2[ntmpi].nDuiSpeed = pUpD0Data.ArdVehData[ntmpi].nDuiSpeed;
		lstcArdVeh_V2[ntmpi].nDuiTrack = pUpD0Data.ArdVehData[ntmpi].nDuiTrack;
	}
	memcpy(&srcbuf[srclen], lstcSelVeh_V2.UEID, sizeof(stcSelVeh_V2));
	srclen += sizeof(stcSelVeh_V2);
	memcpy(&srcbuf[srclen], &pUpD0Data.nAroundVehNum, sizeof(uint16_t));
	srclen += sizeof(uint16_t);

	for (ntmpi = 0; ntmpi < pUpD0Data.nAroundVehNum; ntmpi++)
	{
		memcpy(&srcbuf[srclen], &lstcArdVeh_V2[ntmpi], sizeof(stcArdVeh_V2));
		srclen += sizeof(stcArdVeh_V2);
	}

	Encode_Frame((uint8_t) 0xD0, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf, srclen,
			desbuf, deslen);
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
int EncodeD3_V2(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen)
{
	uint8_t buf[1000];
	uint32_t buflen = 0;
	tResult_Bsm_V2 lBsm_V2;
	memcpy(lBsm_V2.Id, RV.Id, sizeof(RV.Id));
	lBsm_V2.Latitude = RV.Latitude * 10000000;
	lBsm_V2.Longitude = RV.Longitude * 10000000;
	lBsm_V2.wStr = lRes.wStr;
	lBsm_V2.wPrio = lRes.wPrio;
	lBsm_V2.Distance = lRes.Distance;
	lBsm_V2.Location = lRes.Location;

	memcpy(&buf[buflen], &lBsm_V2, sizeof(tResult_Bsm_V2));
	buflen += sizeof(tResult_Bsm_V2);

	Encode_Frame((uint8_t) 0xD3, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

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
int EncodeD4_V2(tVehData HV, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen)
{
	//数据转换
	tResult_Rsi_V2 lResult_Rsi_V2;

//	lResult_Rsi_V2.wType = lResult_Rsi.wType; /**< 应用类型. */
	lResult_Rsi_V2.wStr = lResult_Rsi.wStr; /**< V2I应用场景. */
//	lResult_Rsi_V2.wPrio = lResult_Rsi.wPrio; /**< 优先级. */
//	lResult_Rsi_V2.WarningType = lResult_Rsi.WarningType; /**< 警告类型. */

///消息类型
	lResult_Rsi_V2.MsgType = lResult_Rsi.MsgType;
	///参考位置（经纬海拔）
	lResult_Rsi_V2.Latitude = (nint32_t) (lResult_Rsi.AlertPos.Latitude
			* 10000000); /**< 纬度值. */
	lResult_Rsi_V2.Longitude = (nint32_t) (lResult_Rsi.AlertPos.Longitude
			* 10000000); /**< 经度值. */
	lResult_Rsi_V2.AlertPosDir = (lResult_Rsi.AlertPathList[0].AlertPosDir); /**< 图标基于路网的角度. */

	///从Rsi的Description中提取出的事件相关信息
	if (lResult_Rsi_V2.MsgType == 81 || lResult_Rsi_V2.MsgType == 82)
		lResult_Rsi_V2.TagVal = (uint16_t) (lResult_Rsi.DscpInfo.TagVal * 100);
	else
		lResult_Rsi_V2.TagVal = (uint16_t) (lResult_Rsi.DscpInfo.TagVal);

	lResult_Rsi_V2.AssisTag = (uint16_t) (lResult_Rsi.DscpInfo.AssisTag);
	lResult_Rsi_V2.AssisVal = (uint16_t) (lResult_Rsi.DscpInfo.AssisVal);
	//if(lResult_Rsi_V2.MsgType == 81) printf("%d  %d  %d\n",lResult_Rsi_V2.TagVal, lResult_Rsi_V2.AssisTag,lResult_Rsi_V2.AssisVal);

	uint8_t buf[1000];
	uint32_t buflen = 0;

	memcpy(&buf[buflen], &lResult_Rsi_V2, sizeof(tResult_Rsi_V2));
	buflen += sizeof(tResult_Rsi_V2);

	//数据组帧
	Encode_Frame((uint8_t) 0xD4, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);
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
int EncodeD5_V2(double Laser_Lat, double Laser_Long, tVehData HV, tVehData *RV,
		uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t buf[10240];
	uint32_t buflen = 0;
	int nTmpi = 0;

	tResult_Rsm_V2 lResult_Spat_V2;
	lResult_Spat_V2.nAnalysisCount = nRVCont; /**< 识别物个数. */
	for (nTmpi = 0; nTmpi < nRVCont; nTmpi++)
	{
		memcpy(lResult_Spat_V2.AnalysisInfoId, RV[nTmpi].Id, sizeof(RV[nTmpi].Id));/** 识别物ID. */
		lResult_Spat_V2.Latitude = (nint32_t) (RV[nTmpi].Latitude * 10000000); /**< 纬度 度. */
		lResult_Spat_V2.Longitude = (nint32_t) (RV[nTmpi].Longitude * 10000000); /**< 经度 度. */
		lResult_Spat_V2.Speed = (uint16_t) (RV[nTmpi].Speed * 3.6); /**< 速度 米每秒. */
		lResult_Spat_V2.Heading = (uint16_t) (RV[nTmpi].Heading);/**< 航向角 度. */
		lResult_Spat_V2.Location = (nint8_t) (RV[nTmpi].BackNode);/**< 航向角 度. */
	}
	memcpy(&buf[buflen], &lResult_Spat_V2.nAnalysisCount,
			sizeof(tResult_Rsm_V2));
	buflen += sizeof(tResult_Rsm_V2);

	Encode_Frame((uint8_t) 0xD5, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

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
int EncodeD6_V2(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen)
{
	tResult_Spat_V2 lSpatRes_V2;

	int i;
	uint8_t buf[1000];
	uint32_t buflen = 0;

	//结构转换 赋值
	lSpatRes_V2.HVLight = HV.Light;
	lSpatRes_V2.Latitude = (nint32_t) (pSpeedGudie_Input.Lat * 10000000); /**< 信号灯纬度.*/
	lSpatRes_V2.Longitude = (nint32_t) (pSpeedGudie_Input.Lng * 10000000); /**< 信号灯经度.*/
//	lSpatRes_V2.wType = Res.wType; /**< 应用类型.*/
	lSpatRes_V2.wStr = Res.wStr; /**< 应用场景.*/
	lSpatRes_V2.wPrio = Res.wPrio; /**< 优先级.*/
//	lSpatRes_V2.Cycle = Res.Cycle; /**< 信号灯周期.*/
//	lSpatRes_V2.Distance = Res.Distance; /**< 到红绿灯距离.*/
	lSpatRes_V2.RelBearing = Res.RelBearing; /**< 红绿灯方位角.*/
	lSpatRes_V2.PhaseCnt = Res.PhaseCnt; /**< 前方相位个数. */
	for (i = 0; i < lSpatRes_V2.PhaseCnt; i++)
	{
		/**< 灯色状态. */
		lSpatRes_V2.PhaseInfo_V2[i].Status = Res.tUpPhaseInfo[i].Status;
		/**< 剩余时间. */
		lSpatRes_V2.PhaseInfo_V2[i].TimeLeft = Res.tUpPhaseInfo[i].TimeLeft;
		/**< 行车方向. */
		lSpatRes_V2.PhaseInfo_V2[i].Direction = Res.tUpPhaseInfo[i].Direction;
		/**< 最高建议时速. */
		lSpatRes_V2.PhaseInfo_V2[i].SuggSd_H =
				Res.tUpPhaseInfo[i].SuggestSpeed_High;
		/**< 最低建议时速. */
		lSpatRes_V2.PhaseInfo_V2[i].SuggSd_L =
				Res.tUpPhaseInfo[i].SuggestSpeed_Low;
		/**< 建议时速. */
		lSpatRes_V2.PhaseInfo_V2[i].SuggSd = Res.tUpPhaseInfo[i].SuggestSpeed;
	}
	//赋值 循环之前
	memcpy(&buf[buflen], &lSpatRes_V2,
			sizeof(uint16_t) + sizeof(uint32_t) * 2 + sizeof(uint8_t) * 5);
	buflen += sizeof(uint16_t) + sizeof(uint32_t) * 2 + sizeof(uint8_t) * 5;

	//赋值 循环数组
	for (i = 0; i < pSpeedGudie_Input.PhaseCnt; i++)
	{
		memcpy(&buf[buflen], &lSpatRes_V2.PhaseInfo_V2[i].Status,
				sizeof(UpPhaseInfo_V2));
		buflen += sizeof(UpPhaseInfo_V2);
	}

	Encode_Frame((uint8_t) 0xD6, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

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
int EncodeD7_V2(tMapInfo MapInfo, uint8_t *desbuf, uint32_t *deslen)
{
	int nTmpi, nTmpj, nTmpk;

	uint8_t srcbuf[10240];
	int srclen = 0;

	//节点个数
	memcpy(&srcbuf[srclen], &MapInfo, sizeof(int));
	srclen += sizeof(int);

	printf("\n* %d *\n", MapInfo.NodesCnt);
	//节点内容循环操作
	for (nTmpi = 0; nTmpi < MapInfo.NodesCnt; nTmpi++)
	{
		//拷贝 节点ID 节点维度 节点经度 上游节点个数
		memcpy(&srcbuf[srclen], &MapInfo.pNodeInfo[nTmpi].ID,
				sizeof(int) * 2 + sizeof(double) * 2);
		srclen += sizeof(int) * 2 + sizeof(double) * 2;
		printf("\n# %d #\n", MapInfo.pNodeInfo[nTmpi].UpNodeCnt);

		//上游节点 内容循环操作
		for (nTmpj = 0; nTmpj < MapInfo.pNodeInfo[nTmpi].UpNodeCnt; nTmpj++)
		{
			//拷贝 上游节点节点ID 上游节点经纬度，车道宽度 和转向关系个数
			memcpy(&srcbuf[srclen], &MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].ID,
					sizeof(int) * 2 + sizeof(double) * 3);
			srclen += sizeof(int) * 2 + sizeof(double) * 3;
			//转向关系 内容循环操作
			for (nTmpk = 0;
					nTmpk < MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].MoveMentCnt;
					nTmpk++)
			{
				//转向关系中的 下游节点ID和经纬度
//				memcpy(&srcbuf[srclen],
//						&MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].pMoveMents[nTmpk].ID,
//						sizeof(int) + sizeof(double) * 2);
//				srclen += sizeof(int) + sizeof(double) * 2;
				memcpy(&srcbuf[srclen],
						&MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].pMoveMents[nTmpk].ID,
						sizeof(tMoveMent));
				srclen += sizeof(tMoveMent);
			}
			//拷贝 限速集合个数
			memcpy(&srcbuf[srclen],
					&MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].SpeedCnt,
					sizeof(int));
			srclen += sizeof(int);
			//限速集合 内容循环拷贝
			for (nTmpk = 0;
					nTmpk < MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].SpeedCnt;
					nTmpk++)
			{
				//限速结构体
				memcpy(&srcbuf[srclen],
						&MapInfo.pNodeInfo[nTmpi].pUpNode[nTmpj].MapSpeedLimits[nTmpk].SpeedLimitType,
						sizeof(tMapSpeedLimitInfo));
				srclen += sizeof(tMapSpeedLimitInfo);
			}
		}
	}
	Encode_Frame((uint8_t) 0xD7, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf,
			(uint32_t) srclen, desbuf, (uint32_t *) deslen);
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
int EncodeD9_V2(UpD9Data pUpD9Data, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t srcbuf[4024];
	uint32_t srclen = 0;
	uint8_t ntmpi = 0;

	memcpy(&srcbuf[srclen], &pUpD9Data.nAroundRsiNum, sizeof(uint16_t));
	srclen += sizeof(uint16_t);
	STCAroundRsiData_V2 lRsiData_V2;
	for (ntmpi = 0; ntmpi < pUpD9Data.nAroundRsiNum; ntmpi++)
	{
		memset(&lRsiData_V2, 0x00, sizeof(STCAroundRsiData_V2));
		lRsiData_V2.RsiType = pUpD9Data.ArdRsiData[ntmpi].RsiType;			//RSI消息TYPE
		lRsiData_V2.IsLive = pUpD9Data.ArdRsiData[ntmpi].IsLive;			//RSI消息存在状态 有RSI消息时默认设置为 20；
																			//<5时 显示端删除即可
																			//等于0时，设备端复用该位置结构体存储新RSI消息
		lRsiData_V2.n32RsiLat1 = pUpD9Data.ArdRsiData[ntmpi].dDuiLat1 * 10000000;			//RSI消息坐标的纬度	 单位：1e7度
		lRsiData_V2.n32RsiLng1 = pUpD9Data.ArdRsiData[ntmpi].dDuiLng1 * 10000000;			//RSI消息坐标的经度	 单位：1e7度
		lRsiData_V2.nDistance = pUpD9Data.ArdRsiData[ntmpi].nDistance;			//本机与RSI消息的车距 单位：米
		lRsiData_V2.nSpeakDis = pUpD9Data.ArdRsiData[ntmpi].nSpeakDis;			//显示端播报的距离
		lRsiData_V2.TagVal = pUpD9Data.ArdRsiData[ntmpi].RsiDscpInfo.TagVal;			//事件值
		lRsiData_V2.uPriority = pUpD9Data.ArdRsiData[ntmpi].uPriority;			//优先级
		lRsiData_V2.WarningType = pUpD9Data.ArdRsiData[ntmpi].WarningType;		//警告类型 原来的事件状态标志位
		lRsiData_V2.RelPostion = pUpD9Data.ArdRsiData[ntmpi].RelPostion;			//相对方位角
		lRsiData_V2.RsiAppType = pUpD9Data.ArdRsiData[ntmpi].RsiA;			//事件类型(V2I应用类型)
		//向buf中拷贝数据
		memcpy(&srcbuf[srclen], &lRsiData_V2.RsiType,
				sizeof(STCAroundRsiData_V2));
		srclen += sizeof(STCAroundRsiData_V2);
	}

	Encode_Frame((uint8_t) 0xD9, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf, srclen,
			desbuf, deslen);
	return 0;
}
