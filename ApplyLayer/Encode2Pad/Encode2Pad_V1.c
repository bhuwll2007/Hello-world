/*
 * Encode2Pad_V1.c
 *
 *  Created on: Jan 6, 2020
 *      Author: wanijv2x
 */
#include "Encode2Pad_V1.h"

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD0_V1(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t srcbuf[1024];
	uint32_t srclen = 0;
	uint8_t ntmpi = 0;

	memcpy(&srcbuf[srclen], &pUpD0Data.SlfVehData, sizeof(STCSelfVehData));
	srclen += sizeof(STCSelfVehData);

	memcpy(&srcbuf[srclen], &pUpD0Data.nAroundVehNum, sizeof(uint16_t));
	srclen += sizeof(uint16_t);

	for (ntmpi = 0; ntmpi < pUpD0Data.nAroundVehNum; ntmpi++)
	{
		memcpy(&srcbuf[srclen], &pUpD0Data.ArdVehData[ntmpi],
				sizeof(STCAroundVehData));
		srclen += sizeof(STCAroundVehData);
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
int EncodeD3_V1(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen)
{
	uint8_t buf[1000];
	uint32_t buflen = 0;

	memcpy(&buf[buflen], &HV, sizeof(tVehData));
	buflen += sizeof(tVehData);
	memcpy(&buf[buflen], &RV, sizeof(tVehData));
	buflen += sizeof(tVehData);
	memcpy(&buf[buflen], &lRes, sizeof(tResult_Bsm));
	buflen += sizeof(tResult_Bsm);

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
int EncodeD4_V1(tVehData HV, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t buf[1000];
	uint32_t buflen = 0;

	//自车状态信息
	memcpy(&buf[buflen], &HV, sizeof(tVehData));
	buflen += sizeof(tVehData);
	//计算结果拷贝
	memcpy(&buf[buflen], &lResult_Rsi, sizeof(tResult_Rsi));
	buflen += sizeof(tResult_Rsi);

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
int EncodeD5_V1(double Laser_Lat, double Laser_Long, tVehData HV, tVehData *RV,
		uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t buf[10240];
	uint32_t buflen = 0;

	int nTmpi = 1;

	//int nTmpNum = 0;
	memcpy(&buf[buflen], &nTmpi, sizeof(uint32_t));
	buflen += sizeof(uint32_t);

	memcpy(&buf[buflen], &Laser_Lat, sizeof(double));
	buflen += sizeof(double);

	memcpy(&buf[buflen], &Laser_Long, sizeof(double));
	buflen += sizeof(double);

	//if(nRVCont > 5) nRVCont = 5;
	memcpy(&buf[buflen], &nRVCont, sizeof(uint32_t));
	buflen += sizeof(uint32_t);

	for (nTmpi = 0; nTmpi < nRVCont; nTmpi++)
	{
		memcpy(&buf[buflen], &RV[nTmpi],
				sizeof(uint8_t) * 4 + sizeof(double) * 5);
		buflen += sizeof(uint8_t) * 4 + sizeof(double) * 5;
		memcpy(&buf[buflen], &RV[nTmpi].VehicleWidth, sizeof(double) * 3);
		buflen += sizeof(double) * 3;
		memcpy(&buf[buflen], &RV[nTmpi].VehicleClass, sizeof(uint16_t));
		buflen += sizeof(uint16_t);
	}
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
int EncodeD6_V1(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input, tResult_Spat Res,
		uint8_t *desbuf, uint32_t *deslen)
{
	int i;
	uint8_t buf[1000];
	uint32_t buflen = 0;

	memcpy(&buf[buflen], &HV, sizeof(tVehData));
	buflen += sizeof(tVehData);

	memcpy(&buf[buflen], &pSpeedGudie_Input.Lng, sizeof(uint32_t) * 5);
	buflen += sizeof(uint32_t) * 5;
	for (i = 0; i < pSpeedGudie_Input.PhaseCnt; i++)
	{
		memcpy(&buf[buflen], &pSpeedGudie_Input.pLightInfo[i],
				sizeof(talgPhaseInfo));
		buflen += sizeof(talgPhaseInfo);
	}

	memcpy(&buf[buflen], &Res.wType, sizeof(int) * 7);
	buflen += sizeof(int) * 7;
	for (i = 0; i < Res.PhaseCnt; i++)
	{
		memcpy(&buf[buflen], &Res.tUpPhaseInfo[i], sizeof(UpPhaseInfo));
		buflen += sizeof(UpPhaseInfo);
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
int EncodeD7_V1(tMapInfo MapInfo, uint8_t *desbuf, uint32_t *deslen)
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
int EncodeD9_V1(UpD9Data pUpD9Data, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t srcbuf[4024];
	uint32_t srclen = 0;
	uint8_t ntmpi = 0;

	memcpy(&srcbuf[srclen], &pUpD9Data.nAroundRsiNum, sizeof(uint16_t));
	srclen += sizeof(uint16_t);

	for (ntmpi = 0; ntmpi < pUpD9Data.nAroundRsiNum; ntmpi++)
	{
		//向buf中拷贝数据
		memcpy(&srcbuf[srclen], &pUpD9Data.ArdRsiData[ntmpi],
				sizeof(STCAroundRsiData));
		srclen += sizeof(STCAroundRsiData);
	}

	Encode_Frame((uint8_t) 0xD9, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf, srclen,
			desbuf, deslen);
	return 0;
}
