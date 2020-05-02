/*
 * SAPT_Alogorithm.c
 *
 *  Created on: Jun 6, 2018
 *      Author: root
 */
#include "RSI_Algorithm.h"
#include "MapMatch.h"
#include "cJSON.h"

#define SPEED_LIMIT_LIST_SIZE 13
#define D(format,...) printf(format, ##__VA_ARGS__)

#define PRINT_DEBUG 0
tRsiDscpInfo GetRsiDscpInfo(char *JsonStr)
{
	tRsiDscpInfo DscpInfo =
	{ 0 };

	cJSON *root = NULL, *ret = NULL;
	root = cJSON_Parse(JsonStr);
	if (!root)
		goto out;
#if 0
	printf("Json Decode Succeed\n");
#endif

	ret = cJSON_GetObjectItem(root, "TagVal");
	if (ret)
	{
		DscpInfo.TagVal = ret->valuedouble;
		//printf("WarnInfo.TagVal:%f\n", DscpInfo.TagVal);
	}

	ret = cJSON_GetObjectItem(root, "AssisTag");
	if (ret)
	{
		DscpInfo.AssisTag = ret->valueint;
		//printf("WarnInfo.AssisTag:%d\n", DscpInfo.AssisTag);
	}

	ret = cJSON_GetObjectItem(root, "AssisVal");
	if (ret)
	{
		DscpInfo.AssisVal = ret->valuedouble;
		//printf("WarnInfo.AssisVal:%f\n", DscpInfo.AssisVal);
	}
	// if (ret)
	// 	cJSON_Delete(ret);
	if (root)
		cJSON_Delete(root);

	out: return DscpInfo;
}

int IsInRoad(double p_dLat1, double p_dLng1, double p_dLat2, double p_dLng2,
		double p_dLat3, double p_dLng3, double Height)
{
	double p_dDis12 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat2, p_dLng2);
	double p_dDis23 = FG_Getdistance(p_dLat2, p_dLng2, p_dLat3, p_dLng3);
	double p_dDis13 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat3, p_dLng3);

	double p = (p_dDis12 + p_dDis13 + p_dDis23) / 2;
	double S = pow(p * (p - p_dDis12) * (p - p_dDis13) * (p - p_dDis23), 0.5);

	double p_dDis = 2 * S / p_dDis12;

	double temp1 = p_dDis12 * p_dDis12 + p_dDis13 * p_dDis13;
	double temp2 = p_dDis12 * p_dDis12 + p_dDis23 * p_dDis23;

	if (p_dDis < Height / 2)
	{
		if ((temp1 < (p_dDis23 * p_dDis23)) || (temp2 < (p_dDis13 * p_dDis13)))
		{
			return 0;
		} else
		{
			return 1;
		}
	} else
	{
		return 0;
	}
}
void ClearRsiInfo(tRsiMsg_Arry *pRsiInfo)
{
	int i = 0;
	int j = 0;
	if (pRsiInfo != NULL)
	{
		if (pRsiInfo->Al_RTEData != NULL)
		{
			for (i = 0; i < pRsiInfo->Al_RTEDataNum; i++)
			{
				if (pRsiInfo->Al_RTEData[i].AlertPathList != NULL)
				{
					for (j = 0; j < pRsiInfo->Al_RTEData[i].AlertPathNbr; j++)
					{
						memset(&pRsiInfo->Al_RTEData[i].AlertPathList[j], 0x00,
								sizeof(RSIPathPt));
					}
					free(pRsiInfo->Al_RTEData[i].AlertPathList);
					pRsiInfo->Al_RTEData[i].AlertPathList = NULL;
				}
			}
			free(pRsiInfo->Al_RTEData);
			pRsiInfo->Al_RTEData = NULL;
		}
		if (pRsiInfo->Al_RTSData != NULL)
		{
			for (i = 0; i < pRsiInfo->Al_RTSDataNum; i++)
			{
				if (pRsiInfo->Al_RTSData[i].AlertPathList != NULL)
				{
					for (j = 0; j < pRsiInfo->Al_RTSData[i].AlertPathNbr; j++)
					{
						memset(&pRsiInfo->Al_RTSData[i].AlertPathList[j], 0x00,
								sizeof(RSIPathPt));
					}
					free(pRsiInfo->Al_RTSData[i].AlertPathList);
					pRsiInfo->Al_RTSData[i].AlertPathList = NULL;
				}
			}
			free(pRsiInfo->Al_RTSData);
			pRsiInfo->Al_RTSData = NULL;
		}
	}
}

int GetRsiInfo(MessageFrame_t *pRsiMsg, tRsiMsg_Arry *pRsiInfo)
{
	int i = 0;

	memset(pRsiInfo, 0, sizeof(tRsiMsg));
	if (pRsiMsg->choice.rsiFrame.id.size > 20)
		return -1;
	memcpy(&pRsiInfo->RSUID, pRsiMsg->choice.rsiFrame.id.buf,
			(pRsiMsg->choice.rsiFrame.id.size >= sizeof(pRsiInfo->RSUID)) ?
					sizeof(pRsiInfo->RSUID) : pRsiMsg->choice.rsiFrame.id.size);

	pRsiInfo->AlertPos.Latitude = pRsiMsg->choice.rsiFrame.refPos.lat
			/ 10000000.0;
	pRsiInfo->AlertPos.Longitude = pRsiMsg->choice.rsiFrame.refPos.Long
			/ 10000000.0;

	if (pRsiMsg->choice.rsiFrame.rtss != NULL)
	{
		int j = 0;
		RTSData_t **RTSData = pRsiMsg->choice.rsiFrame.rtss->list.array;
		pRsiInfo->Al_RTSDataNum = pRsiMsg->choice.rsiFrame.rtss->list.count;
		pRsiInfo->Al_RTSData = ALLOC_OBJ(Al_RTSData_t, pRsiInfo->Al_RTSDataNum);
		for (j = 0; j < pRsiInfo->Al_RTSDataNum; j++)
		{
			///消息ID
			pRsiInfo->Al_RTSData[j].RTSID = RTSData[j]->rtsId;
			///消息类型
			pRsiInfo->Al_RTSData[j].MsgType = RTSData[j]->signType;
			///参考位置
			if (RTSData[j]->signPos == NULL)
			{
				pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
						pRsiInfo->AlertPos.Latitude;
				pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
						pRsiInfo->AlertPos.Longitude;
			} else
			{
				switch (RTSData[j]->signPos->offsetLL.present)
				{
				case PositionOffsetLL_PR_position_LL1:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL1.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL1.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL2:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL2.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL2.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL3:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL3.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL3.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL4:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL4.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL4.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL5:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL5.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL5.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL6:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL6.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTSData[j]->signPos->offsetLL.choice.position_LL6.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LatLon:
					pRsiInfo->Al_RTSData[j].AlertPos.Latitude =
							RTSData[j]->signPos->offsetLL.choice.position_LatLon.lat
									/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPos.Longitude =
							RTSData[j]->signPos->offsetLL.choice.position_LatLon.lon
									/ 10000000.0;
					break;
				default:
					break;
				}
				printf("pRsiInfo->Al_RTSData[%d].AlertPos.Latitude =%f \n", j,
						pRsiInfo->Al_RTSData[j].AlertPos.Latitude);
				printf("pRsiInfo->Al_RTSData[%d].AlertPos.Longitude =%f \n", j,
						pRsiInfo->Al_RTSData[j].AlertPos.Longitude);
			}
			///消息描述
			if (RTSData[j]->description == NULL)
				return -2;
			if ((RTSData[j]->description->present == Description_PR_textString)
					&& (RTSData[j]->description->choice.textString.size > 256))
				return -3;
			//			if ((RTSData[j]->description->present == Description_PR_textString)
			//					&& (RTSData[j]->description->choice.textGB2312->size > 512))
			//				return -4;

			if ((RTSData[j]->description->present == Description_PR_textString))
			{
				memcpy(pRsiInfo->Al_RTSData[j].Description,
						RTSData[j]->description->choice.textString.buf,
						RTSData[j]->description->choice.textString.size);
			}
			//			if ((RTSData[j]->description->present == Description_PR_textString))
			//			{
			//				memcpy(pRsiInfo->Al_RTSData[j].Description,
			//						RTSData[j]->description->choice.textString->buf,
			//						RTSData[j]->description->choice.textGB2312->size);
			//			}

			///优先级
			///消息半径(m)
			struct ReferencePath **pReferencePath =
					RTSData[j]->referencePaths->list.array;
			pRsiInfo->Al_RTSData[j].AlertRadius = pReferencePath[0]->pathRadius;

			///消息轨迹点个数
			pRsiInfo->Al_RTSData[j].AlertPathNbr =
					pReferencePath[0]->activePath.list.count;
			///消息点位置
			pRsiInfo->Al_RTSData[j].AlertPathList = (RSIPathPt *) malloc(
					sizeof(RSIPathPt) * pRsiInfo->Al_RTSData[j].AlertPathNbr);
			for (i = 0; i < pRsiInfo->Al_RTSData[j].AlertPathNbr; i++)
			{
				switch (pReferencePath[0]->activePath.list.array[i]->offsetLL.present)
				{
				case PositionOffsetLL_PR_position_LL1:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL1.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL1.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL2:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL2.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL2.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL3:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL3.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL3.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL4:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL4.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL4.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL5:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL5.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL5.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL6:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL6.lat
											/ 10000000.0;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTSData[j].AlertPos.Longitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL6.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LatLon:
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Latitude =
							(double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LatLon.lat)
									/ 10000000;
					pRsiInfo->Al_RTSData[j].AlertPathList[i].AlertPath.Longitude =
							(double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LatLon.lon)
									/ 10000000;
					break;
				default:
					break;
				}
			}

//			pRsiInfo->Al_RTSData[j].AlertPos.Latitude = pRsiInfo->Al_RTSData[j].AlertPathList[0].AlertPath.Latitude;
//			pRsiInfo->Al_RTSData[j].AlertPos.Longitude = pRsiInfo->Al_RTSData[j].AlertPathList[0].AlertPath.Longitude;
		}
	}
	if (pRsiMsg->choice.rsiFrame.rtes != NULL)
	{
		int j = 0;
		RTEData_t **RTEData = pRsiMsg->choice.rsiFrame.rtes->list.array;
		pRsiInfo->Al_RTEDataNum = pRsiMsg->choice.rsiFrame.rtes->list.count;
		pRsiInfo->Al_RTEData = (Al_RTEData_t *) malloc(
				sizeof(Al_RTEData_t) * pRsiInfo->Al_RTEDataNum);
		for (j = 0; j < pRsiInfo->Al_RTEDataNum; j++)
		{
			///消息ID
			pRsiInfo->Al_RTEData[j].RTEID = RTEData[j]->rteId;
			///消息类型
			pRsiInfo->Al_RTEData[j].MsgType = RTEData[j]->eventType;
			///参考位置
			if (RTEData[j]->eventPos == NULL)
			{
				pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
						pRsiInfo->AlertPos.Latitude;
				pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
						pRsiInfo->AlertPos.Longitude;
			} else
			{
				switch (RTEData[j]->eventPos->offsetLL.present)
				{
				case PositionOffsetLL_PR_position_LL1:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL1.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL1.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL2:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL2.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL2.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL3:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL3.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL3.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL4:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL4.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL4.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL5:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL5.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL5.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL6:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							pRsiInfo->AlertPos.Latitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL6.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							pRsiInfo->AlertPos.Longitude
									+ RTEData[j]->eventPos->offsetLL.choice.position_LL6.lon
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LatLon:
					pRsiInfo->Al_RTEData[j].AlertPos.Latitude =
							RTEData[j]->eventPos->offsetLL.choice.position_LatLon.lat
									/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPos.Longitude =
							RTEData[j]->eventPos->offsetLL.choice.position_LatLon.lon
									/ 10000000.0;
					break;
				default:
					break;
				}
				printf("pRsiInfo->Al_RTEData[%d].AlertPos.Latitude =%f \n", j,
						pRsiInfo->Al_RTEData[j].AlertPos.Latitude);
				printf("pRsiInfo->Al_RTEData[%d].AlertPos.Longitude =%f \n", j,
						pRsiInfo->Al_RTEData[j].AlertPos.Longitude);
			}
			///消息描述
			if (RTEData[j]->description == NULL)
				return -2;
			if ((RTEData[j]->description->present == Description_PR_textString)
					&& (RTEData[j]->description->choice.textString.size > 256))
				return -3;

			if ((RTEData[j]->description->present == Description_PR_textString))
			{
				memcpy(pRsiInfo->Al_RTEData[j].Description,
						RTEData[j]->description->choice.textString.buf,
						RTEData[j]->description->choice.textString.size);
			}

			///优先级
			///消息半径(m)
			struct ReferencePath **pReferencePath =
					RTEData[j]->referencePaths->list.array;
			pRsiInfo->Al_RTEData[j].AlertRadius = pReferencePath[0]->pathRadius;

			///消息轨迹点个数
			pRsiInfo->Al_RTEData[j].AlertPathNbr =
					pReferencePath[0]->activePath.list.count;
			///消息点位置
			pRsiInfo->Al_RTEData[j].AlertPathList = (RSIPathPt *) malloc(
					sizeof(RSIPathPt) * pRsiInfo->Al_RTEData[j].AlertPathNbr);
			for (i = 0; i < pRsiInfo->Al_RTEData[j].AlertPathNbr; i++)
			{
				switch (pReferencePath[0]->activePath.list.array[i]->offsetLL.present)
				{
				case PositionOffsetLL_PR_position_LL1:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL1.lat
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL1.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL2:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL2.lat)
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL2.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL3:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL3.lat)
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL3.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL4:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL4.lat)
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL4.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL5:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL5.lat)
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL5.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LL6:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Latitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL6.lat)
											/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							pRsiInfo->Al_RTEData[j].AlertPos.Longitude
									+ (double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LL6.lon)
											/ 10000000.0;
					break;
				case PositionOffsetLL_PR_position_LatLon:
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Latitude =
							(double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LatLon.lat)
									/ 10000000.0;
					pRsiInfo->Al_RTEData[j].AlertPathList[i].AlertPath.Longitude =
							(double) (pReferencePath[0]->activePath.list.array[i]->offsetLL.choice.position_LatLon.lon)
									/ 10000000.0;
					break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

int GetIVSType(uint32_t MsgType)
{
	int ret = 0;

	switch (MsgType)
	{
	case 1: //交叉路口～～～～～～～～～～～～～～～～～～～～～～～～～～～
	case 2:  // 急转弯 SharpTurn～～～～～～～～～～～～～～～～～～～～～～～～～
	case 7:  //窄路
	case 8:  //注意窄桥～～～～～～～～～～～～～～～～～～
	case 10:  //注意行人～～～～～～～～～～～～～～～～～～
	case 11:  //注意儿童～～～～～～～～～～～～～～～～～～
	case 14:  //注意信号灯
	case 15: // 注意落石 RockFall
	case 17: //易滑
	case 28: //有人看守铁路道口
	case 32: //注意非机动车～～～～～～～～～～～～～～～～～～～～
	case 38: //施工
	case 39: //建议速度

	case 48: //停车让行
	case 80: //禁止鸣喇叭～～～～～～～～～～～～～～～～～～～～
	case 81: //限宽
	case 82: //限高
	case 83: //限重
	case 242: //学校～～～～～～～～～～～～～～～～～～～～～
		ret = 1;
		break;

	case 37:  // 危险路 Danger～～～～～～～～～～～～～～～～～～～～～～～～2
	case 85:  // 限速 SpeedLimit
	case 47:  // 车辆排队
	case 44: //注意合流～～～～～～～～～～～～～～～～～～～～
	case 50: //会车让行～～～～～～～～～～～～～～～～～
		ret = 2;
		break;

	default:
		ret = 0;
		break;
	}

	return ret;
}
int IsUponWarning(tVehData *pHV, uint32_t MsgType, tRsiDscpInfo WarnInfo)
{
	int ret = 0;

	switch (MsgType)
	{
	case 81:
		if (pHV->VehicleWidth > WarnInfo.TagVal)
			ret = 1;
		break;  // 限制宽度（m）
	case 82:
		if (pHV->VehicleHight > WarnInfo.TagVal)
			ret = 1;
		break;  // 限制高度（m）
//	case 85:
//		if (pHV->Speed > (double) WarnInfo.TagVal / 3.6)
//			ret = 1;
//		break;  // 限制速度（km/h）->限速只作为提醒使用

	default:
		break;
	}

	return ret;
}

#define  FORWARD	11
#define  LEFT		22
#define  RIGHT		33
#define  BACK		44

double AngelSet = 5;

uint8_t GetPos(double p_dLatObj, double p_dlngObj, double p_dLatTag,
		double p_dlngTag, double p_dCObj)
{
	///当前车与提醒点间的相对方位
	uint8_t RelPos;

	double PtRelBear = 0;
#if 0
	double dis = 0;
	double AlertRadius = 1;
#endif
	//计算角度
	PtRelBear = FG_GetRelBear(p_dLatObj, p_dlngObj, p_dLatTag, p_dlngTag,
			p_dCObj);

#if 1
	if ((PtRelBear >= 0 && PtRelBear < 90)
			|| (PtRelBear >= 270 && PtRelBear <= 360))
	{
		RelPos = FORWARD;
	} else
	{
		RelPos = BACK;
	}
//	printf("PtRelBear = %f     RelPos = %d ",PtRelBear,RelPos);

#endif
#if 0
	//计算距离
	dis = FG_Getdistance(p_dLatObj, p_dlngObj, p_dLatTag, p_dlngTag);
	double LateralOffset = dis * fabs(sin(FG_Angletoradian(PtRelBear)));

	// 计算自车与提醒点之间的相对方位
	if (LateralOffset < AlertRadius / 10)
	{
		if ((PtRelBear >= 0 && PtRelBear < 90)
				|| (PtRelBear >= 270 && PtRelBear < 360))
		{
			RelPos = FORWARD;
		}
		else
		{
			RelPos = BACK;
		}
	}
	else
	{
		if (PtRelBear <= AngelSet || PtRelBear >= (360 - AngelSet))
		{
			RelPos = FORWARD;
		}
		else if (PtRelBear >= AngelSet && PtRelBear < 90)
		{
			RelPos = RIGHT;
		}
		else if (PtRelBear >= 270 && PtRelBear < (360 - AngelSet))
		{
			RelPos = LEFT;
		}
		else
		{
			RelPos = BACK;
		}
	}
#endif
	return RelPos;
}

tResult_Rsi IVS(tVehData *pHV, tRsiMsg *pRsiUsage)
{
	tResult_Rsi CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };

	int i = 0, IsInzone = 0;
//	double WarnDis = 50.0;
	double PtRelBear = 0;
	CResult.WarningType = NoWarning;
	CResult.wStr = 0, CResult.wStr |= IVS_Warning;
	CResult.wType = Type_RSI;

	CResult.AlertPos = pRsiUsage->AlertPos;
	CResult.MsgType = pRsiUsage->MsgType;
	CResult.RelPos = 0;
	CResult.SpeakDis = g_V2I_Parameter.IVSSpeakDis;
	memset(&CResult.DscpInfo, 0, sizeof(tRsiDscpInfo));
#if 0
	printf("-----------------------------------------------------");
	printf("pRsiUsage->AlertRadius:%f\n", pRsiUsage->AlertRadius);
	printf("-----------------------------------------------------");
#endif
	CResult.DscpInfo = GetRsiDscpInfo(pRsiUsage->Description);
	int IVSType = GetIVSType(pRsiUsage->MsgType);
//	D("IVSType:%d\n", IVSType);
	switch (IVSType)
	{
	case 1:  // 事件点
		CResult.AlertPathList[0].AlertPath = pRsiUsage->AlertPos; // 关联点为参考位置的经纬度
		CResult.AlertPathList[1].AlertPath = pRsiUsage->AlertPos;
		CResult.dis = FG_Getdistance(pHV->Latitude, pHV->Longitude,
				pRsiUsage->AlertPos.Latitude, pRsiUsage->AlertPos.Longitude);
		PtRelBear = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
				pRsiUsage->AlertPos.Latitude, pRsiUsage->AlertPos.Longitude,
				pHV->Heading);

		double LateralOffset = CResult.dis
				* fabs(sin(FG_Angletoradian(PtRelBear)));

		// 计算自车与提醒点之间的相对方位
		if (LateralOffset < pRsiUsage->AlertRadius / 10)
		{
			if ((PtRelBear >= 0 && PtRelBear < 90)
					|| (PtRelBear >= 270 && PtRelBear < 360))
			{
				CResult.RelPos = FORWARD;
			} else
			{
				CResult.RelPos = BACK;
			}
		} else
		{
			if (PtRelBear <= AngelSet || PtRelBear >= (360 - AngelSet))
			{
				CResult.RelPos = FORWARD;
			} else if (PtRelBear >= AngelSet && PtRelBear < 90)
			{
				CResult.RelPos = RIGHT;
			} else if (PtRelBear >= 270 && PtRelBear < (360 - AngelSet))
			{
				CResult.RelPos = LEFT;
			} else
			{
				CResult.RelPos = BACK;
			}
		}
		// 进入提醒距离范围内
		if ((g_V2I_Parameter.IVSWarnDis
				+ pHV->Speed * g_Spat_Parameter.WarnCalcTime) > CResult.dis)
		{
			// 根据相对方位确定警告类型
			if (CResult.RelPos != BACK && LateralOffset < 100)
			{
				CResult.WarningType = PreWarning;
				CResult.wPrio = Lower_V2I_PRIO + 1;
				if (IsUponWarning(pHV, pRsiUsage->MsgType, CResult.DscpInfo))
				{
					// 超限
					CResult.WarningType |= UponWarning;
					CResult.wPrio = Lower_V2I_PRIO + 0;
				}

				CResult.DscpInfo.AssisVal = 1;

			} else
			{
				CResult.dis = 0;
				CResult.WarningType = NoWarning;

				CResult.DscpInfo.AssisVal = 0;
			}
		}
		break;
	case 2:  // 路段
		// 轮询路段，检查自车是否在某一路段上
		CResult.dis = 0;
		CResult.AlertPathList[0].AlertPath =
				pRsiUsage->AlertPathList[i].AlertPath;
		CResult.AlertPathList[1].AlertPath =
				pRsiUsage->AlertPathList[i + 1].AlertPath;
		for (i = 0; i < pRsiUsage->AlertPathNbr - 1; i++)
		{
#if 0
			D("%d: %f, %f, %f, %f\n", i, pRsiUsage->AlertPathList[i].AlertPath.Latitude,pRsiUsage->AlertPathList[i].AlertPath.Longitude,
					pRsiUsage->AlertPathList[i+1].AlertPath.Latitude,pRsiUsage->AlertPathList[i+1].AlertPath.Longitude);
#endif
			IsInzone = IsInRoad(pRsiUsage->AlertPathList[i].AlertPath.Latitude,
					pRsiUsage->AlertPathList[i].AlertPath.Longitude,
					pRsiUsage->AlertPathList[i + 1].AlertPath.Latitude,
					pRsiUsage->AlertPathList[i + 1].AlertPath.Longitude,
					pHV->Latitude, pHV->Longitude, pRsiUsage->AlertRadius / 10);
#if 0
			D("IsInzone:%d\n", IsInzone);
#endif
			PtRelBear = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
					pRsiUsage->AlertPathList[i + 1].AlertPath.Latitude,
					pRsiUsage->AlertPathList[i + 1].AlertPath.Longitude,
					pHV->Heading);

			// 计算到前方道路节点的方向
			if ((PtRelBear >= 0 && PtRelBear < 90)
					|| (PtRelBear >= 270 && PtRelBear < 360))
			{
				CResult.RelPos = FORWARD;
			} else
			{
				CResult.RelPos = BACK;
			}

			if (IsInzone)		//在路段内
			{
				/*CResult.dis = FG_Getdistance(pHV->Latitude, pHV->Longitude,
				 pRsiUsage->AlertPathList[i + 1].AlertPath.Latitude,
				 pRsiUsage->AlertPathList[i + 1].AlertPath.Longitude);*/
				CResult.dis = 0;
				// 根据相对方位确定事件类型
				if (CResult.RelPos == FORWARD)
				{
					CResult.WarningType = InsideWarning;
					CResult.wPrio = Lower_V2I_PRIO + 1;

					if (IsUponWarning(pHV, pRsiUsage->MsgType,
							CResult.DscpInfo))
					{
						// 超限
						CResult.WarningType |= UponWarning;
						CResult.wPrio = Lower_V2I_PRIO + 0;
					}
					// 确定在某路段上，跳出路段轮询循环
					break;
				} else
				{
					CResult.WarningType = NoWarning;
				}
			}
		}

		// 若自车不在任一路段上，就针对提醒点位置进行预报
		if (!IsInzone)
		{
			double DistoRoadMid = 0;
			CResult.dis = FG_Getdistance(pHV->Latitude,
					pHV->Longitude,  // 自车与提醒点的距离
					pRsiUsage->AlertPos.Latitude,
					pRsiUsage->AlertPos.Longitude);
			CResult.PathLen = FG_Getdistance(
					pRsiUsage->AlertPathList[0].AlertPath.Latitude, // 得到路段起始段的长度
					pRsiUsage->AlertPathList[0].AlertPath.Longitude,
					pRsiUsage->AlertPathList[1].AlertPath.Latitude,
					pRsiUsage->AlertPathList[1].AlertPath.Longitude);
			DistoRoadMid = FG_Getdistance(pHV->Latitude, pHV->Longitude,
					(pRsiUsage->AlertPathList[0].AlertPath.Latitude
							+ pRsiUsage->AlertPathList[1].AlertPath.Latitude)
							/ 2,
					(pRsiUsage->AlertPathList[0].AlertPath.Longitude
							+ pRsiUsage->AlertPathList[1].AlertPath.Longitude)
							/ 2);
			PtRelBear = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
					(pRsiUsage->AlertPathList[0].AlertPath.Latitude
							+ pRsiUsage->AlertPathList[1].AlertPath.Latitude)
							/ 2,
					(pRsiUsage->AlertPathList[0].AlertPath.Longitude
							+ pRsiUsage->AlertPathList[1].AlertPath.Longitude)
							/ 2, pHV->Heading);
			double LateralOffset = DistoRoadMid
					* fabs(sin(FG_Angletoradian(PtRelBear)));
			CResult.RelPos = 0;

			// 计算自车与提醒点之间的相对方位
			if (LateralOffset < pRsiUsage->AlertRadius / 10)
			{
				if ((PtRelBear >= 0 && PtRelBear < 90)
						|| (PtRelBear >= 270 && PtRelBear < 360))
				{
					CResult.RelPos = FORWARD;
				} else
				{
					CResult.RelPos = BACK;
				}
			} else
			{
				if ((PtRelBear < AngelSet) || (PtRelBear >= (360 - AngelSet)))
				{
					CResult.RelPos = FORWARD;
				} else if (PtRelBear >= AngelSet && PtRelBear < 90)
				{
					CResult.RelPos = RIGHT;
				} else if (PtRelBear >= 270 && PtRelBear < (360 - AngelSet))
				{
					CResult.RelPos = LEFT;
				} else
				{
					CResult.RelPos = BACK;
				}
			}

			// 进入提醒距离范围内
			if ((g_V2I_Parameter.IVSWarnDis
					+ pHV->Speed * g_Spat_Parameter.WarnCalcTime) > CResult.dis)
			{
				// 根据相对方位确定警告类型
				if (CResult.RelPos != BACK)
				{
					CResult.WarningType = PreWarning;
					CResult.wPrio = Lower_V2I_PRIO + 1;
					if (IsUponWarning(pHV, pRsiUsage->MsgType,
							CResult.DscpInfo))
					{
						// 超限
						CResult.WarningType |= UponWarning;
						CResult.wPrio = Lower_V2I_PRIO + 0;
					}
				} else
				{
					CResult.WarningType = NoWarning;
				}
			}
		}

		break;

	default:
		break;
	}

	if (CResult.WarningType > 0)
		CResult.SpeakDis = (int) (CResult.dis / 10) * 10;
	return CResult;
}

int ChangeSLForm(SpeedLimitInfo *SpeedLimitList, uint32_t *SpeedLimitType)
{
	int ret = 0;

	// 将限速信息转换为以位表示的形式
	*SpeedLimitType = 0;
	int i = 0;
	for (i = 0; i < SPEED_LIMIT_LIST_SIZE; i++)
	{
		if (SpeedLimitList[i].IsActive)
		{
			*SpeedLimitType |= (1 << i);
		}
	}

	// 当前路段存在限速信息，返回1
	if (*SpeedLimitType)
		ret = 1;

	return ret;
}

int GetHVSL(tResult_SL *CResult, SpeedLimitInfo *SpeedLimitList,
		uint32_t *SpeedLimitType)
{
	int ret = SpeedNoLimit;

	// 根据自车类型在限速类型列表中选择对应的选项进行匹配
	// 匹配最低限速
	if (*SpeedLimitType & (1 << SpeedLimitType_vehicleMinSpeed))
	{
//		slusage->VehSpeedLimitLow = SpeedLimit_Low;
		CResult->VehSpeedLimitLow =
				(double) SpeedLimitList[SpeedLimitType_vehicleMinSpeed].Speed
						* 3.6;
		ret |= SpeedLowLimit;
	} else
		CResult->VehSpeedLimitLow = 0;

	// 匹配最高限速
	if (*SpeedLimitType & (1 << SpeedLimitType_vehicleMaxSpeed))
	{
//		slusage->VehSpeedLimitHigh = SpeedLimit_High;
		CResult->VehSpeedLimitHigh =
				(double) SpeedLimitList[SpeedLimitType_vehicleMaxSpeed].Speed
						* 3.6;
		ret |= SpeedHighLimit;
	} else
		CResult->VehSpeedLimitHigh = 0;
#if 0

	printf("=================================== %d ================================\n", CResult->VehSpeedLimitLow);
	printf("=================================== %d ================================\n", CResult->VehSpeedLimitHigh);
#endif
	return ret;
}

tResult_SL MapSL(tVehData *pHV, tMapInfo *pMapInfo)
{
	tResult_SL CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
#if 0
	int i = 0, j = 0, k = 0, ret = 0;
	SpeedLimitInfo SpeedLimitList[SPEED_LIMIT_LIST_SIZE] =
	{
		{	0}};

	CResult.WarningType = NoWarning;
	CResult.wStr = 0, CResult.wStr |= IVS_Warning;
	CResult.wType = Type_RSI;

	memset(CResult.AlertPathList, 0, 2 * sizeof(RSIPathPt));
	uint32_t SpeedLimitType = 0;
	//根据地图匹配结果，在地图信息中找到下游转向关系
	for (i = 0; i < pMapInfo->NodesCnt; i++)
	{
		//找到车辆在地图中前方节点
		if (pHV->ForwardNode == pMapInfo->pNodeInfo[i].ID)
		{
			for (j = 0; j < pMapInfo->pNodeInfo[i].UpNodeCnt; j++)
			{
				//找前方节点的上游节点
				if (pHV->BackNode == pMapInfo->pNodeInfo[i].pUpNode[j].ID)
				{
					for (k = 0; k < pMapInfo->pNodeInfo[i].pUpNode[j].SpeedCnt;
							k++)
					{
						SpeedLimitList[pMapInfo->pNodeInfo[i].pUpNode[j].MapSpeedLimits[k].SpeedLimitType].IsActive =
						1;
						SpeedLimitList[pMapInfo->pNodeInfo[i].pUpNode[j].MapSpeedLimits[k].SpeedLimitType].Speed =
						pMapInfo->pNodeInfo[i].pUpNode[j].MapSpeedLimits[k].SpeedLimitValue;
					}
					goto found;
				}
			}
		}
	}

	found:

	ret = ChangeSLForm(SpeedLimitList, &SpeedLimitType);
	if (ret)
	{
		CResult.WarningType = InsideWarning;
		CResult.wPrio = Lower_V2I_PRIO + 1;

		CResult.AlertPathList[0].AlertPath.Latitude =
		pMapInfo->pNodeInfo[i].pUpNode[j].Lat;
		CResult.AlertPathList[0].AlertPath.Longitude =
		pMapInfo->pNodeInfo[i].pUpNode[j].Lng;
		CResult.AlertPathList[1].AlertPath.Latitude =
		pMapInfo->pNodeInfo[i].Lat;
		CResult.AlertPathList[1].AlertPath.Longitude =
		pMapInfo->pNodeInfo[i].Lng;
	}

	ret = GetHVSL(&CResult, SpeedLimitList, &SpeedLimitType);
#if 0
	printf("\t\t\t\t rrr:%d\n", ret);
#endif
	// 在道路本身限速的基础上根据RSI消息中的限速信息更新限速范围
	switch (ret)
	{
		case SpeedLowLimit:
		if (pHV->Speed < (double) CResult.VehSpeedLimitLow / 3.6)
		{
			CResult.WarningType |= UponWarning;
			CResult.wPrio = Lower_V2I_PRIO + 0;
		}

		break;
		case SpeedHighLimit:
		if (pHV->Speed > (double) CResult.VehSpeedLimitHigh / 3.6)
		{
			CResult.WarningType |= UponWarning;
			CResult.wPrio = Lower_V2I_PRIO + 0;
		}

		break;
		case SpeedAllLimit:
		if (/*(pHV->Speed < (double)CResult.sl.VehSpeedLimitLow / 3.6)
				 ||*/(pHV->Speed > (double) CResult.VehSpeedLimitHigh / 3.6))
		{
			CResult.WarningType |= UponWarning;
			CResult.wPrio = Lower_V2I_PRIO + 0;
		}

		break;
		case SpeedNoLimit:
		CResult.wStr = STR_DEFAULT;
		break;
		default:
		break;
	}
#endif
	return CResult;
}

tResult_SL RsiSL(tVehData *pHV, tRsiMsg *pRsiUsage)
{
	tResult_SL CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
	return CResult;
}

tResult_SL SL(tVehData *pHV, tRsiMsg *pRsiUsage, tMapInfo *pMapInfo)
{
	tResult_SL CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
	tResult_SL CResult1 =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
	tResult_SL CResult2 =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };

	CResult1 = RsiSL(pHV, pRsiUsage);

	printf("\n\nCResult1:%d\n", CResult1.WarningType);
	CResult2 = MapSL(pHV, pMapInfo);
	printf("CResult2:%d\n\n\n", CResult2.WarningType);

	printf("\t\t\t\tCResult2.spdhigh:%d\n", CResult2.VehSpeedLimitHigh);
	printf("\t\t\t\tCResult2.spdlow:%d\n", CResult2.VehSpeedLimitLow);
	//综合限速结果并返回
	CResult.wType = Type_RSI;
	CResult.WarningType = NoWarning;
	CResult.wStr = 0, CResult.wStr |= IVS_Warning;

	//综合限速结果
	CResult.WarningType =
			(CResult1.WarningType > CResult2.WarningType) ?
					CResult1.WarningType : CResult2.WarningType;

	if (CResult1.VehSpeedLimitHigh != 0 && CResult2.VehSpeedLimitHigh != 0)
		CResult.VehSpeedLimitHigh =
				(CResult1.VehSpeedLimitHigh < CResult2.VehSpeedLimitHigh) ?
						CResult1.VehSpeedLimitHigh : CResult2.VehSpeedLimitHigh;
	else if (CResult1.VehSpeedLimitHigh != 0)
		CResult.VehSpeedLimitHigh = CResult1.VehSpeedLimitHigh;
	else if (CResult2.VehSpeedLimitHigh != 0)
		CResult.VehSpeedLimitHigh = CResult2.VehSpeedLimitHigh;
	else
		CResult.VehSpeedLimitHigh = 0;

	if (CResult1.VehSpeedLimitLow != 0 && CResult2.VehSpeedLimitLow != 0)
		CResult.VehSpeedLimitLow =
				(CResult1.VehSpeedLimitLow > CResult2.VehSpeedLimitLow) ?
						CResult1.VehSpeedLimitLow : CResult2.VehSpeedLimitLow;
	else if (CResult1.VehSpeedLimitLow != 0)
		CResult.VehSpeedLimitLow = CResult1.VehSpeedLimitLow;
	else if (CResult2.VehSpeedLimitLow != 0)
		CResult.VehSpeedLimitLow = CResult2.VehSpeedLimitLow;
	else
		CResult.VehSpeedLimitLow = 0;

	return CResult;
}

tResult_Rsi IVS_MapMatch(tVehData *pHV, tRsiMsg *pRsiUsage, tMapInfo pMapInfo)
{
	tResult_Rsi CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };

	uint8_t RsiRelated = 0;
	int i = 0, j = 0;
	uint32_t DownNode = 10000, UpNode = 10000;
	RSIPos3D DownNodePos =
	{ 0 }, UpNodePos =
	{ 0 };
	RSIPos3D RoadPt[2] =
	{
	{ 0 } };
	uint8_t RoadPtCnt = 0;

	//计算RSI事件方向角
	double EventHeading = FG_GetTrueBear(
			pRsiUsage->AlertPathList[0].AlertPath.Latitude,
			pRsiUsage->AlertPathList[0].AlertPath.Longitude,
			pRsiUsage->AlertPathList[1].AlertPath.Latitude,
			pRsiUsage->AlertPathList[1].AlertPath.Longitude);

	tVehData RsiMapInfo =
	{ .Heading = EventHeading, .Latitude = pRsiUsage->AlertPos.Latitude,
			.Longitude = pRsiUsage->AlertPos.Longitude };

	// 先执行车内标牌算法
	CResult = IVS(pHV, pRsiUsage);

	//本车没有匹配到道路上，则不执行事件道路匹配算法
	if ((pHV->BackNode == 10000) || (pHV->ForwardNode == 10000))
	{
#if PRINT_DEBUG
		if ((CResult.MsgType == 2))
		{
			printf(
					"-2CResult.MsgType = %d CResult.WarningType = %d CResult.RelPos = %d\n",
					CResult.MsgType, CResult.WarningType, CResult.RelPos);
			printf("-2RsiMapInfo.BackNode %d,RsiMapInfo.ForwardNode %d\n",
					RsiMapInfo.BackNode, RsiMapInfo.ForwardNode);
			printf("-2pHV.BackNode %d,pHV.ForwardNode %d\n", pHV->BackNode,
					pHV->ForwardNode);
		}
#endif
		return CResult;
	}

	// 再判断事件对于当前车的位置是否有效
	if (MapMatch_Pojcet(pMapInfo, &RsiMapInfo) == 0)  // 成功将Rsi事件匹配到某个路段上
	{
		//	printf("Rsi (%d ---> %d) == ",RsiMapInfo.BackNode,RsiMapInfo.ForwardNode);
		// 检索事件所在路段两节点的坐标
		for (i = 0; i < pMapInfo.NodesCnt; i++)
		{
			//找到需判定的下游节点在地图中的位置
			if (RsiMapInfo.ForwardNode == pMapInfo.pNodeInfo[i].ID) // RoadPt下标0中始终存储RsiMapInfo.ForwardNode的坐标
			{
				RoadPt[0].Latitude = pMapInfo.pNodeInfo[i].nodePos.Latitude;
				RoadPt[0].Longitude = pMapInfo.pNodeInfo[i].nodePos.Longitude;
				RoadPtCnt++;
			}
			if (RsiMapInfo.BackNode == pMapInfo.pNodeInfo[i].ID)
			{
				RoadPt[1].Latitude = pMapInfo.pNodeInfo[i].nodePos.Latitude;
				RoadPt[1].Longitude = pMapInfo.pNodeInfo[i].nodePos.Longitude;
				RoadPtCnt++;
			}

			if (RoadPtCnt >= 2)
				break;
		}

#if 1
		// 根据道路的方向计算显示端图标需要旋转的角度
		if (RoadPtCnt >= 2) // 找到了2个节点的坐标
		{
			// 计算两节点连线相对北方的夹角(0~360)
			CResult.AlertPathList[0].AlertPosDir = FG_GetTrueBear(
					RoadPt[0].Latitude, RoadPt[0].Longitude, RoadPt[1].Latitude,
					RoadPt[1].Longitude);
			// 计算显示端图标应旋转的角度（>180表示需逆时针旋转360-x度，小于180表示需顺时针旋转x度）
			if ((CResult.AlertPathList[0].AlertPosDir > 315
					&& CResult.AlertPathList[0].AlertPosDir <= 360)
					|| (CResult.AlertPathList[0].AlertPosDir >= 0
							&& CResult.AlertPathList[0].AlertPosDir < 45)
					|| (CResult.AlertPathList[0].AlertPosDir > 135
							&& CResult.AlertPathList[0].AlertPosDir < 225)) // 南北方向的路
			{
				if ((CResult.AlertPathList[0].AlertPosDir > 135
						&& CResult.AlertPathList[0].AlertPosDir < 180)
						|| (CResult.AlertPathList[0].AlertPosDir > 315
								&& CResult.AlertPathList[0].AlertPosDir < 360)) // 图标需要逆时针方向旋转
				{
					CResult.AlertPathList[0].AlertPosDir =
							(CResult.AlertPathList[0].AlertPosDir < 180) ?
									(CResult.AlertPathList[0].AlertPosDir + 180) :
									CResult.AlertPathList[0].AlertPosDir;
				} else if ((CResult.AlertPathList[0].AlertPosDir > 0
						&& CResult.AlertPathList[0].AlertPosDir < 45)
						|| (CResult.AlertPathList[0].AlertPosDir > 180
								&& CResult.AlertPathList[0].AlertPosDir < 225)) // 图标需要顺时针方向旋转
				{
					CResult.AlertPathList[0].AlertPosDir =
							(CResult.AlertPathList[0].AlertPosDir > 180) ?
									(CResult.AlertPathList[0].AlertPosDir - 180) :
									CResult.AlertPathList[0].AlertPosDir;
				}
			} else  // 东西方向的路
			{
				if ((CResult.AlertPathList[0].AlertPosDir >= 45
						&& CResult.AlertPathList[0].AlertPosDir < 90)
						|| (CResult.AlertPathList[0].AlertPosDir >= 225
								&& CResult.AlertPathList[0].AlertPosDir < 270)) // 图标需要逆时针方向旋转
				{
					CResult.AlertPathList[0].AlertPosDir =
							(CResult.AlertPathList[0].AlertPosDir < 180) ?
									(CResult.AlertPathList[0].AlertPosDir + 180) :
									CResult.AlertPathList[0].AlertPosDir;
					CResult.AlertPathList[0].AlertPosDir += 90;

				} else /*if((CResult.AlertPathList[0].AlertPosDir > 90 && CResult.AlertPathList[0].AlertPosDir < 135) ||
				 (CResult.AlertPathList[0].AlertPosDir > 180 && CResult.AlertPathList[0].AlertPosDir < 225))*/ // 图标需要顺时针方向旋转
				{
					CResult.AlertPathList[0].AlertPosDir =
							(CResult.AlertPathList[0].AlertPosDir > 180) ?
									(CResult.AlertPathList[0].AlertPosDir - 180) :
									CResult.AlertPathList[0].AlertPosDir;
					CResult.AlertPathList[0].AlertPosDir -= 90;

				}
			}
		} else  // 没找到全部两个节点的坐标
		{
			// 显示端需要显示的图标旋转角度为0
			CResult.AlertPathList[0].AlertPosDir = 0;
		}

#endif
		//-----------------------------------------------
		// 判断Rsi所在路段是否与自车前方道路节点有关
		if ((pHV->ForwardNode == RsiMapInfo.ForwardNode)
				|| (pHV->ForwardNode == RsiMapInfo.BackNode))
		{
			// Rsi和自车在同一路段上
			if (((pHV->ForwardNode == RsiMapInfo.ForwardNode)
					&& (pHV->BackNode == RsiMapInfo.BackNode))
					|| ((pHV->ForwardNode == RsiMapInfo.BackNode)
							&& (pHV->BackNode == RsiMapInfo.ForwardNode)))
			{
				RsiRelated = 1;
				if (pHV->ForwardNode == RsiMapInfo.ForwardNode)
				{
					UpNode = RsiMapInfo.BackNode;
					DownNode = RsiMapInfo.ForwardNode;
					DownNodePos = RoadPt[0];
					UpNodePos = RoadPt[1];
				} else
				{
					DownNode = RsiMapInfo.BackNode;
					UpNode = RsiMapInfo.ForwardNode;
					DownNodePos = RoadPt[1];
					UpNodePos = RoadPt[0];
				}
			} else
			{
				if (pHV->ForwardNode == RsiMapInfo.ForwardNode)
				{
					UpNode = RsiMapInfo.ForwardNode;
					DownNode = RsiMapInfo.BackNode;
					DownNodePos = RoadPt[1];
					UpNodePos = RoadPt[0];
				} else
				{
					DownNode = RsiMapInfo.ForwardNode;
					UpNode = RsiMapInfo.BackNode;
					DownNodePos = RoadPt[0];
					UpNodePos = RoadPt[1];
				}
				//根据地图匹配结果，在地图信息中找到下游转向关系
				for (i = 0; i < pMapInfo.NodesCnt; i++)
				{
					//找到需判定的下游节点在地图中的位置
					if (DownNode == pMapInfo.pNodeInfo[i].ID)
					{
						for (j = 0; j < pMapInfo.pNodeInfo[i].UpNodeCnt; j++)
						{
							//找前方节点的上游节点
							if (UpNode == pMapInfo.pNodeInfo[i].pUpNode[j].ID)
							{
								RsiRelated = 1;
							}
						}
					}
				}
			}
		}

		if (RsiRelated)  // 将Rsi进行路段匹配，并算得Rsi与自车行驶方向相关
		{
//			CResult = IVS(pHV, pRsiUsage);
			if ((CResult.AlertPathList[0].AlertPath.Latitude
					!= CResult.AlertPathList[1].AlertPath.Latitude)
					&& (CResult.AlertPathList[0].AlertPath.Longitude
							!= CResult.AlertPathList[1].AlertPath.Longitude)) // 是路段消息
			{
				double a = FG_GetTrueBear(UpNodePos.Latitude,
						UpNodePos.Longitude, DownNodePos.Latitude,
						DownNodePos.Longitude);
				double b = FG_GetTrueBear(
						CResult.AlertPathList[0].AlertPath.Latitude,
						CResult.AlertPathList[0].AlertPath.Longitude,
						CResult.AlertPathList[1].AlertPath.Latitude,
						CResult.AlertPathList[1].AlertPath.Longitude);
				double err = a - b;
				//-
//				printf("a(%f) - b(%f) = err(%f)\n",a,b,err);
				if (err > -30 && err < 30)
				{
				}  // 行进方向与Rsi路段方向相同
				else  // 行进方向与Rsi路段方向不同
				{
					CResult.WarningType = NoWarning;
					CResult.RelPos = BACK;
				}
			}
		} else  // Rsi消息与自车行驶无关
		{
//			CResult = IVS(pHV, pRsiUsage);
			CResult.WarningType = NoWarning;
			CResult.RelPos = BACK;
		}
	} else  // Rsi节点与路段匹配失败
	{
		printf("Rsi (--- ---> ---) == ");
		CResult = IVS(pHV, pRsiUsage);

	}
	return CResult;
}

double Get_IconAngle(tRsiMsg *pRsiUsage, tMapInfo pMapInfo,
		uint32_t ForwardNode, uint32_t BackNode)
{
	tResult_Rsi CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
	int i;
	RSIPos3D RoadPt[2] =
	{
	{ 0 } };
	int RoadPtCnt = 0;

	// 检索事件所在路段两节点的坐标
	for (i = 0; i < pMapInfo.NodesCnt; i++)
	{
		//找到需判定的下游节点在地图中的位置
		if (ForwardNode == pMapInfo.pNodeInfo[i].ID) // RoadPt下标0中始终存储RsiMapInfo.ForwardNode的坐标
		{
			RoadPt[0].Latitude = pMapInfo.pNodeInfo[i].nodePos.Latitude;
			RoadPt[0].Longitude = pMapInfo.pNodeInfo[i].nodePos.Longitude;
			RoadPtCnt++;
		}
		if (BackNode == pMapInfo.pNodeInfo[i].ID)
		{
			RoadPt[1].Latitude = pMapInfo.pNodeInfo[i].nodePos.Latitude;
			RoadPt[1].Longitude = pMapInfo.pNodeInfo[i].nodePos.Longitude;
			RoadPtCnt++;
		}

		if (RoadPtCnt >= 2)
			break;
	}

	// 根据道路的方向计算显示端图标需要旋转的角度
	if (RoadPtCnt >= 2) // 找到了2个节点的坐标
	{
		// 计算两节点连线相对北方的夹角(0~360)
		CResult.AlertPathList[0].AlertPosDir = FG_GetTrueBear(
				RoadPt[0].Latitude, RoadPt[0].Longitude, RoadPt[1].Latitude,
				RoadPt[1].Longitude);
		// 计算显示端图标应旋转的角度（>180表示需逆时针旋转360-x度，小于180表示需顺时针旋转x度）
		if ((CResult.AlertPathList[0].AlertPosDir > 315
				&& CResult.AlertPathList[0].AlertPosDir <= 360)
				|| (CResult.AlertPathList[0].AlertPosDir >= 0
						&& CResult.AlertPathList[0].AlertPosDir < 45)
				|| (CResult.AlertPathList[0].AlertPosDir > 135
						&& CResult.AlertPathList[0].AlertPosDir < 225)) // 南北方向的路
		{
			if ((CResult.AlertPathList[0].AlertPosDir > 135
					&& CResult.AlertPathList[0].AlertPosDir < 180)
					|| (CResult.AlertPathList[0].AlertPosDir > 315
							&& CResult.AlertPathList[0].AlertPosDir < 360)) // 图标需要逆时针方向旋转
			{
				CResult.AlertPathList[0].AlertPosDir =
						(CResult.AlertPathList[0].AlertPosDir < 180) ?
								(CResult.AlertPathList[0].AlertPosDir + 180) :
								CResult.AlertPathList[0].AlertPosDir;
			} else if ((CResult.AlertPathList[0].AlertPosDir > 0
					&& CResult.AlertPathList[0].AlertPosDir < 45)
					|| (CResult.AlertPathList[0].AlertPosDir > 180
							&& CResult.AlertPathList[0].AlertPosDir < 225)) // 图标需要顺时针方向旋转
			{
				CResult.AlertPathList[0].AlertPosDir =
						(CResult.AlertPathList[0].AlertPosDir > 180) ?
								(CResult.AlertPathList[0].AlertPosDir - 180) :
								CResult.AlertPathList[0].AlertPosDir;
			}
		} else  // 东西方向的路
		{
			if ((CResult.AlertPathList[0].AlertPosDir >= 45
					&& CResult.AlertPathList[0].AlertPosDir < 90)
					|| (CResult.AlertPathList[0].AlertPosDir >= 225
							&& CResult.AlertPathList[0].AlertPosDir < 270)) // 图标需要逆时针方向旋转
			{
				CResult.AlertPathList[0].AlertPosDir =
						(CResult.AlertPathList[0].AlertPosDir < 180) ?
								(CResult.AlertPathList[0].AlertPosDir + 180) :
								CResult.AlertPathList[0].AlertPosDir;
				CResult.AlertPathList[0].AlertPosDir += 90;

			} else /*if((CResult.AlertPathList[0].AlertPosDir > 90 && CResult.AlertPathList[0].AlertPosDir < 135) ||
			 (CResult.AlertPathList[0].AlertPosDir > 180 && CResult.AlertPathList[0].AlertPosDir < 225))*/ // 图标需要顺时针方向旋转
			{
				CResult.AlertPathList[0].AlertPosDir =
						(CResult.AlertPathList[0].AlertPosDir > 180) ?
								(CResult.AlertPathList[0].AlertPosDir - 180) :
								CResult.AlertPathList[0].AlertPosDir;
				CResult.AlertPathList[0].AlertPosDir -= 90;

			}
		}
	} else  // 没找到全部两个节点的坐标
	{
		// 显示端需要显示的图标旋转角度为0
		CResult.AlertPathList[0].AlertPosDir = 0;
	}

	return CResult.AlertPathList[0].AlertPosDir;

}

double Get_RouteRsiDis(tVehData *pHV, tRsiMsg *pRsiUsage, tMapInfo pMapInfo)
{
	double Distance = 0;
	int i = 0;
	double CornerLat = 0, CornerLng = 0;
	//计算RSI事件方向角
	double EventHeading = FG_GetTrueBear(
			pRsiUsage->AlertPathList[0].AlertPath.Latitude,
			pRsiUsage->AlertPathList[0].AlertPath.Longitude,
			pRsiUsage->AlertPathList[1].AlertPath.Latitude,
			pRsiUsage->AlertPathList[1].AlertPath.Longitude);

	//ＲＳＩ事件道路匹配
	tVehData RsiMapInfo =
	{ .Heading = EventHeading, .Latitude = pRsiUsage->AlertPos.Latitude,
			.Longitude = pRsiUsage->AlertPos.Longitude };

	MapMatch_Pojcet(pMapInfo, &RsiMapInfo);

	//RSI事件与处在同一路段和方向
	if (RsiMapInfo.ForwardNode == pHV->ForwardNode
			&& RsiMapInfo.BackNode == pHV->BackNode)
	{
		//事件关于ＨＶ的相对方位
		double RSI_RelDirection = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
				pRsiUsage->AlertPos.Latitude, pRsiUsage->AlertPos.Longitude,
				pHV->Heading);

		//事件是否在ＨＶ前方
		if ((RSI_RelDirection >= 0 && RSI_RelDirection < 90)
				|| (RSI_RelDirection >= 270 && RSI_RelDirection < 360))
		{
			Distance = FG_Getdistance(pHV->Latitude, pHV->Longitude,
					pRsiUsage->AlertPos.Latitude,
					pRsiUsage->AlertPos.Longitude);
		}
	}	//RSI事件在ＨＶ所在路段的下游路段
	else if (RsiMapInfo.ForwardNode != pHV->BackNode
			&& RsiMapInfo.BackNode == pHV->ForwardNode)
	{
		//找到前方转角节点对应的经纬度
		for (; i < pMapInfo.NodesCnt; i++)
		{
			if (RsiMapInfo.BackNode == pMapInfo.pNodeInfo[i].ID)
			{
				CornerLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
				CornerLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
				break;
			}
		}
		Distance = FG_Getdistance(pHV->Latitude, pHV->Longitude, CornerLat,
				CornerLng)
				+ FG_Getdistance(CornerLat, CornerLng,
						pRsiUsage->AlertPos.Latitude,
						pRsiUsage->AlertPos.Longitude);
	} else
	{
		Distance = FG_Getdistance(pHV->Latitude, pHV->Longitude,
				pRsiUsage->AlertPos.Latitude, pRsiUsage->AlertPos.Longitude);
	}

	return Distance;
}

tResult_Rsi RSI_EventProcess(tVehData *pHV, tRsiMsg *pRsiUsage,
		tMapInfo pMapInfo)
{
	tResult_Rsi CResult =
	{ Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT };
	int i = 0;
	double CornerLat = 0, CornerLng = 0;
//	double NextPtLat = 0, NextPtLng = 0;

	// 先执行车内标牌算法
	CResult = IVS(pHV, pRsiUsage);
//	if(CResult.MsgType == 37)
//	{
//		if(CResult.WarningType > 0)
//			printf("1CResult.MsgType = %d CResult.WarningType = %d CResult.RelPos = %d\n",CResult.MsgType,CResult.WarningType,CResult.RelPos);
//	}
	//计算RSI事件方向角
	double EventHeading = FG_GetTrueBear(
			pRsiUsage->AlertPathList[0].AlertPath.Latitude,
			pRsiUsage->AlertPathList[0].AlertPath.Longitude,
			pRsiUsage->AlertPathList[1].AlertPath.Latitude,
			pRsiUsage->AlertPathList[1].AlertPath.Longitude);

	//ＲＳＩ事件道路匹配
	tVehData RsiMapInfo =
	{ .Heading = EventHeading, .Latitude = pRsiUsage->AlertPos.Latitude,
			.Longitude = pRsiUsage->AlertPos.Longitude };

	MapMatch_Pojcet(pMapInfo, &RsiMapInfo);

	//RSI事件与ＨＶ处在同一路段和方向
	if (RsiMapInfo.ForwardNode == pHV->ForwardNode
			&& RsiMapInfo.BackNode == pHV->BackNode)
	{
		//事件关于ＨＶ的相对方位
		double RSI_RelDirection = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
				pRsiUsage->AlertPos.Latitude, pRsiUsage->AlertPos.Longitude,
				pHV->Heading);

		//事件是否在ＨＶ前方
		if ((RSI_RelDirection >= 0 && RSI_RelDirection < 90)
				|| (RSI_RelDirection >= 270 && RSI_RelDirection < 360))
		{

			CResult.dis = FG_Getdistance(pHV->Latitude, pHV->Longitude,
					pRsiUsage->AlertPos.Latitude,
					pRsiUsage->AlertPos.Longitude);
			CResult.RelPos = FORWARD;
		} else
		{
			CResult.RelPos = BACK;
		}
	}
	//RSI事件在ＨＶ所在路段的下游路段
	else if (RsiMapInfo.ForwardNode != pHV->BackNode
			&& RsiMapInfo.BackNode == pHV->ForwardNode)
	{
		//找到前方转角节点对应的经纬度
		for (; i < pMapInfo.NodesCnt; i++)
		{
			if (RsiMapInfo.BackNode == pMapInfo.pNodeInfo[i].ID)
			{
				CornerLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
				CornerLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
				break;
			}
		}
		CResult.dis = FG_Getdistance(pHV->Latitude, pHV->Longitude, CornerLat,
				CornerLng)
				+ FG_Getdistance(CornerLat, CornerLng,
						pRsiUsage->AlertPos.Latitude,
						pRsiUsage->AlertPos.Longitude);

		//计算下游路段RSI事件相对于自车ＨＶ的方向
//		for (; j < pMapInfo.NodesCnt; j++)
//		{
//			if (RsiMapInfo.ForwardNode == pMapInfo.pNodeInfo[i].ID)
//			{
//				NextPtLat = pMapInfo.pNodeInfo[i].Lat;
//				NextPtLng = pMapInfo.pNodeInfo[i].Lng;
//				break;
//			}
//		}

//		double PtRelBear = FG_GetRelBear(pHV->Latitude, pHV->Longitude,
//				NextPtLat, NextPtLng, pHV->Heading);
//		double LateralOffset = CResult.dis
//				* fabs(sin(FG_Angletoradian(PtRelBear)));
//
//		if (LateralOffset > 25)
//			CResult.RelPos = RIGHT;   //右转
//		else if (LateralOffset < -25)
//			CResult.RelPos = LEFT;	  //左转
//		else
//			CResult.RelPos = FORWARD;
//		;	  //直行

	} else
	{
		CResult.RelPos = BACK;
	}

	//计算显示端图标的转角
	CResult.AlertPathList[0].AlertPosDir = Get_IconAngle(pRsiUsage, pMapInfo,
			RsiMapInfo.ForwardNode, RsiMapInfo.BackNode);

	//事件提醒处理
	return CResult;
}

