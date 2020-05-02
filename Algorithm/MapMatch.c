/*
 * MapMatch.c
 *
 *  Created on: Jun 4, 2018
 *      Author: root
 */
#include "MapMatch.h"
#define PRINT_LOG 0

double GetMatchVaule(double ForwardLat, double ForwardLng, double BehindLat,
		double BehindLng, struct GnssPoint Current, struct GnssPoint Last);

int MapMatch_Pojcet_Reference(tNodeInfo *pNodeInfo, int NodesCnt,
		struct GnssPoint Current, struct GnssPoint Last,
		struct PosLane *pPosLane);

/*
 * 函数功能：可调用地图匹配函数接口
 * 输入：tMapInfo *pMapInfo 地图信息结构体指针
 * 		VehData *pVehInfo 车辆信息结构体指针
 * 返回值：
 * 		  0 -- 匹配成功
 * 		  1 -- 匹配失败
 */
int MapMatch_Pojcet(tMapInfo pMapInfo, tVehData *pVehInfo)
{
	int i, j;
	int ii = 10000, jj = 10000;
	double ForwardLat, BehindLat;
	double ForwardLng, BehindLng;
	double p_dVerticalDis, p_dLaneDirection;
	double p_dRelDirection;
	double CompareValue, Temp_Min = 10000;

//	printf("\n\n----\npMapInfo->NodesCnt = %d\n\n\n------\n",pMapInfo.NodesCnt);
	for (i = 0; i < pMapInfo.NodesCnt; i++)
	{
		ForwardLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
		ForwardLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
#if PRINT_LOG
		printf("\nNode ID: = %d\n", pMapInfo.pNodeInfo[i].ID);
		printf("\nupNode cnt: = %d\n", pMapInfo.pNodeInfo[i].UpNodeCnt);
#endif
		for (j = 0; j < pMapInfo.pNodeInfo[i].UpNodeCnt; j++)
		{
			BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Latitude;
			BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Longitude;

			//两个节点，定位点构成的三角形 是否是锐角三角形
#if PRINT_LOG
			if(pVehInfo == NULL)
				printf("0");
			else
				printf("1");
			printf("ForwardLat = %f, ",ForwardLat);
			printf("ForwardLng = %f, ",ForwardLng);
			printf("BehindLat = %f, ",BehindLat);
			printf("BehindLng = %f, ",BehindLng);
			printf("pVehInfo->Latitude = %f, ",	pVehInfo->Latitude);
			printf("pVehInfo->Longitude = %f\n",pVehInfo->Longitude);
#endif
			if (!IsAcutetriangle(ForwardLat, ForwardLng, BehindLat, BehindLng,
					pVehInfo->Latitude, pVehInfo->Longitude))
			{
#if PRINT_LOG
				printf(" %d %d ",pMapInfo.pNodeInfo[i].ID,pMapInfo.pNodeInfo[i].pUpNode[j].ID);
#endif
				//计算高度， 车辆到两节点线段的距离
				p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng,
						BehindLat, BehindLng, pVehInfo->Latitude,
						pVehInfo->Longitude);
#if PRINT_LOG
				printf("p_dVerticalDis = %f \n",p_dVerticalDis);
#endif
				if (p_dVerticalDis < g_V2V_Parameter.MapMatch_ConfidenceDis_toLink)
				{
#if PRINT_LOG
					printf("Up Node ID: = %d\n",
							pMapInfo.pNodeInfo[i].pUpNode[j].ID);
#endif
					//路段方向角度
					p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng,
							ForwardLat, ForwardLng);

#if PRINT_LOG
					printf("p_dLaneDirection: = %lf\n",
							p_dLaneDirection);
#endif
					if (p_dLaneDirection >= 180)
						p_dLaneDirection = p_dLaneDirection - 360;

					if (pVehInfo->Heading >= 180)
						pVehInfo->Heading = pVehInfo->Heading - 360;

#if PRINT_LOG
					printf("p_dLaneDirection: = %lf pVehInfo->Heading = %lf\n",
							p_dLaneDirection,pVehInfo->Heading);
#endif
//					p_dRelDirection = fabs(
//							p_dLaneDirection - pVehInfo->Heading);

					p_dRelDirection = fabs(p_dLaneDirection - pVehInfo->Heading);
					if (p_dRelDirection >= 180)
						p_dRelDirection = p_dRelDirection - 360;
					p_dRelDirection = fabs(p_dRelDirection);

					CompareValue = CofficDis * p_dVerticalDis
							+ CofficHeading * p_dRelDirection;
#if PRINT_LOG
					printf(
							"p_dVerticalDis = %f, p_dRelDirection = %f, CompareValue = %f \n",
							p_dVerticalDis, p_dRelDirection, CompareValue);
					printf(
							"CompareValue = %f, Temp_Min = %f \n",
							CompareValue, Temp_Min);
#endif
					if (CompareValue < Temp_Min)
					{
						Temp_Min = CompareValue;
						ii = i;
						jj = j;
					}
				}
			}
			else
			{

			}
		}
	}

//	printf("\n\n\n\n\njj == %d && ii == %d\n\n\n",jj,ii);
	if (pVehInfo->Heading < 0)
		pVehInfo->Heading = pVehInfo->Heading + 360;

	if (jj == 10000 && ii == 10000)
	{

		pVehInfo->ForwardNode = 10000;
		pVehInfo->BackNode = 10000;
		return 1;
	}
	else
	{
		pVehInfo->ForwardNode = pMapInfo.pNodeInfo[ii].ID;
		pVehInfo->BackNode = pMapInfo.pNodeInfo[ii].pUpNode[jj].ID;
		return 0;
	}
}

/*
 * --------------------测试版本-----------
 */
double GetMatchVaule(double ForwardLat, double ForwardLng, double BehindLat,
		double BehindLng, struct GnssPoint Current, struct GnssPoint Last)
{

	//行车方向与候选路段的夹角
	double DeltaTheta;
	//候选路段与正北方向夹角
	double p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng, ForwardLat,
			ForwardLng);
	DeltaTheta = p_dLaneDirection - Current.Heading;

	if (DeltaTheta >= 0 && DeltaTheta <= 90)
		DeltaTheta = p_dLaneDirection - Current.Heading;
	else if (DeltaTheta > 90 && DeltaTheta <= 180)
		DeltaTheta = 180 - DeltaTheta;
	else if (DeltaTheta > 180 && DeltaTheta <= 270)
		DeltaTheta = DeltaTheta - 180;
	else
		DeltaTheta = 360 - DeltaTheta;

	//方向权重
	double w1 = cos(FG_Angletoradian(DeltaTheta));

	//车辆位置投影到候选路段的距离
	double p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng, BehindLat,
			BehindLng, Current.Lat, Current.Lng);
	//距离权重
	double w2 = 1 / (1 + p_dVerticalDis);

	//车辆规矩上一点到当前点的距离和方向角
	double p_dHistoryDis = FG_Getdistance(Current.Lat, Current.Lng, Last.Lat,
			Last.Lng);
	double p_dHistoryHeading = FG_GetTrueBear(Last.Lat, Last.Lng, Current.Lat,
			Current.Lng);
	double DeltaOmega = p_dLaneDirection - p_dHistoryHeading;
	if (DeltaOmega >= 0 && DeltaOmega <= 90)
		DeltaOmega = p_dLaneDirection - p_dHistoryHeading;
	else if (DeltaOmega > 90 && DeltaOmega <= 180)
		DeltaOmega = 180 - DeltaOmega;
	else if (DeltaOmega > 180 && DeltaOmega <= 270)
		DeltaOmega = DeltaOmega - 180;
	else
		DeltaOmega = 360 - DeltaOmega;

	//投影到候选路段的距离
	double p_dProjectDis = p_dHistoryDis * cos(FG_Angletoradian(DeltaOmega));
	//与匹配路段相似程度权重
	double w3 = 1 / (1 + p_dProjectDis);

	return w1 * weight_Heading + w2 * weight_Distance + w3 * weight_Same;
}

int MapMatch_Pojcet_Reference(tNodeInfo *pNodeInfo, int NodesCnt,
		struct GnssPoint Current, struct GnssPoint Last,
		struct PosLane *pPosLane)
{

	int i, j;
	int ii = 10000, jj = 10000;
	double ForwardLat, BehindLat;
	double ForwardLng, BehindLng;
	double p_dVerticalDis;
#if 0
	double p_dLaneDirection;
#endif
#if OPT_PRINTF
	double p_dRelDirection = -1;
#endif

	double CompareValue, Temp_Max = 0;

	for (i = 0; i < NodesCnt; i++)
	{
		ForwardLat = pNodeInfo[i].nodePos.Latitude;
		ForwardLng = pNodeInfo[i].nodePos.Longitude;
#if OPT_PRINTF
		printf("\nNode ID: = %d\n", pNodeInfo[i].ID);
#endif

		for (j = 0; j < pNodeInfo[i].UpNodeCnt; j++)
		{
			BehindLat = pNodeInfo[i].pUpNode[j].nodePos.Latitude;
			BehindLng = pNodeInfo[i].pUpNode[j].nodePos.Longitude;

#if OPT_PRINTF
			printf("Up Node ID: = %d\n", pNodeInfo[i].pUpNode[j].ID);
#endif
			if (!IsAcutetriangle(ForwardLat, ForwardLng, BehindLat, BehindLng,
					Current.Lat, Current.Lng))
			{
				p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng,
						BehindLat, BehindLng, Current.Lat, Current.Lng);

				if (p_dVerticalDis < g_V2V_Parameter.MapMatch_ConfidenceDis_toLink)
				{
#if 0
					p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng,
							ForwardLat, ForwardLng);
#else
					FG_GetTrueBear(BehindLat, BehindLng, ForwardLat,
							ForwardLng);
#endif
					CompareValue = GetMatchVaule(ForwardLat, ForwardLng,
							BehindLat, BehindLng, Current, Last);
#if OPT_PRINTF
					printf(
							"p_dVerticalDis = %f, p_dRelDirection = %f, CompareValue = %f \n",
							p_dVerticalDis, p_dRelDirection, CompareValue);
#endif
					if (CompareValue > Temp_Max)
					{
						Temp_Max = CompareValue;
						ii = i;
						jj = j;
					}
				}

			}

		}
	}
	if (jj == 10000 && ii == 10000)
	{
		return 1;
	}
	else
	{
		pPosLane->ForwordNodeId = pNodeInfo[ii].ID;
		pPosLane->BehideNodeId = pNodeInfo[ii].pUpNode[jj].ID;
		return 0;
	}

}


//double Get_CompareValue(double ForwardLat, double ForwardLng, double BehindLat, double BehindLng, tVehData *pVehInfo)
//{
//	double p_dVerticalDis, p_dLaneDirection;
//	double p_dRelDirection;
//	double CompareValue = 10000;
//
//	//两个节点，定位点构成的三角形 是否是锐角三角形
//	if (!IsAcutetriangle(ForwardLat, ForwardLng, BehindLat, BehindLng,
//			pVehInfo->Latitude, pVehInfo->Longitude) || 1)
//	{
//#if PRINT_LOG
//		printf(" %d %d ",pMapInfo.pNodeInfo[i].ID,pMapInfo.pNodeInfo[i].pUpNode[j].ID);
//#endif
//		//计算高度， 车辆到两节点线段的距离
//		p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng, BehindLat,
//				BehindLng, pVehInfo->Latitude, pVehInfo->Longitude);
//#if PRINT_LOG
//		printf("p_dVerticalDis = %f \n",p_dVerticalDis);
//#endif
//		if (p_dVerticalDis < CONFIDENCE_DIS_TOLINK)
//		{
//#if PRINT_LOG
//			printf("Up Node ID: = %d\n",
//					pMapInfo.pNodeInfo[i].pUpNode[j].ID);
//#endif
//			//路段方向角度
//			p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng, ForwardLat,
//					ForwardLng);
//
//#if PRINT_LOG
//			printf("p_dLaneDirection: = %lf\n",
//					p_dLaneDirection);
//#endif
//			if (p_dLaneDirection >= 180)
//				p_dLaneDirection = p_dLaneDirection - 360;
//
//			if (pVehInfo->Heading >= 180)
//				pVehInfo->Heading = pVehInfo->Heading - 360;
//
//#if PRINT_LOG
//			printf("p_dLaneDirection: = %lf pVehInfo->Heading = %lf\n",
//					p_dLaneDirection,pVehInfo->Heading);
//#endif
////					p_dRelDirection = fabs(
////							p_dLaneDirection - pVehInfo->Heading);
//
//			p_dRelDirection = fabs(p_dLaneDirection - pVehInfo->Heading);
//			if (p_dRelDirection >= 180)
//				p_dRelDirection = p_dRelDirection - 360;
//			p_dRelDirection = fabs(p_dRelDirection);
//
//			CompareValue = COFFIC_DIS * p_dVerticalDis
//					+ COFFIC_ANGLE * p_dRelDirection;
//#if 0
//			printf(
//					"p_dVerticalDis = %f, p_dRelDirection = %f, CompareValue = %f \n",
//					p_dVerticalDis, p_dRelDirection, CompareValue);
//
//#endif
//
//		}
//	}
//	else
//	{
//		CompareValue = 10000;
//	}
//	return CompareValue;
//}


double LaneMatch(uint32_t LaneCnt, tLaneInfo *pLane, tVehData *pVehInfo)
{

	int i, j, Min = 0;
	double disToPoint, tmp = 10000;
	for (i = 0; i < LaneCnt; i++)
	{
		for (j = 0; j < pLane[i].roadPointCnt; j++)
		{


			disToPoint = FG_Getdistance(pVehInfo->Latitude, pVehInfo->Longitude,
					pLane[i].roadPointInfo[j].Latitude, pLane[i].roadPointInfo[j].Longitude);


			if (tmp > disToPoint)
			{
				tmp = disToPoint;
				Min = i;
//				printf("\t%d, %f, %d\n", pLane[i].ID, disToPoint, Min);
			}
		}

	}

	pVehInfo->LaneID =  pLane[Min].ID;
	return tmp;
}

int MapMatch(tMapInfo pMapInfo, tVehData *pVehInfo)
{
	double ForwardLat, BehindLat;
	double ForwardLng, BehindLng;


	if (pMapInfo.NodesCnt > 1)
	{
		int i, j, k;
		int ii = 10000, jj = 10000;
		double CompareValue = 10000, Temp_Min = 10000;

		for (i = 0; i < pMapInfo.NodesCnt; i++)
			{
				for (j = 0; j < pMapInfo.pNodeInfo[i].UpNodeCnt; j++)
				{

					printf("\tupNode-ID= %d\n", pMapInfo.pNodeInfo[i].pUpNode[j].ID);

					ForwardLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
					ForwardLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;

					if (pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt != 0)
					{
						for (k = 0; k < pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt; k++)
						{
							if (k == 0)
							{
								ForwardLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Latitude;
								ForwardLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Longitude;
								BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Latitude;
								BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Longitude;
							}
							else if (k == (pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt -1))
							{
								ForwardLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
								ForwardLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
								BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Latitude;
								BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Longitude;
							}
							else
							{
								ForwardLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Latitude;
								ForwardLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Longitude;
								BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Latitude;
								BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Longitude;
							}

							CompareValue = Get_CompareValue(ForwardLat, ForwardLng, BehindLat, BehindLng, pVehInfo);
							if (CompareValue < Temp_Min)
							{
									Temp_Min = CompareValue;
									ii = i;
									jj = j;
							}

						}
					}
					else
					{
						BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Latitude;
						BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Longitude;
						CompareValue = Get_CompareValue(ForwardLat, ForwardLng, BehindLat, BehindLng, pVehInfo);
					}

					if (CompareValue < Temp_Min)
					{
						Temp_Min = CompareValue;
						ii = i;
						jj = j;
					}
				}
			}


			printf("jj == %d && ii == %d, CompareValue = %f\n\n",jj,ii,CompareValue);
			if (pVehInfo->Heading < 0)
				pVehInfo->Heading = pVehInfo->Heading + 360;

			if (jj == 10000 && ii == 10000)
			{

				pVehInfo->ForwardNode = 10000;
				pVehInfo->BackNode = 10000;
				return 1;
			}
			else
			{
				pVehInfo->ForwardNode = pMapInfo.pNodeInfo[ii].ID;
				pVehInfo->BackNode = pMapInfo.pNodeInfo[ii].pUpNode[jj].ID;

				LaneMatch(pMapInfo.pNodeInfo[ii].pUpNode[jj].LaneCnt,
						&pMapInfo.pNodeInfo[ii].pUpNode[jj].laneInfo[0], pVehInfo);
				return 0;
			}
	}
	else
	{
		double Dis, tmp = 10000;
		int i, Min = 0;

		for (i = 0; i < pMapInfo.pNodeInfo[0].UpNodeCnt; i++)
		{
//			printf("%d\n", pMapInfo.pNodeInfo[0].pUpNode[i].ID);

			Dis = LaneMatch(pMapInfo.pNodeInfo[0].pUpNode[i].LaneCnt,
					&pMapInfo.pNodeInfo[0].pUpNode[i].laneInfo[0], pVehInfo);

			if (tmp > Dis)
			{
				tmp = Dis;
				pVehInfo->ForwardNode = pMapInfo.pNodeInfo[0].ID;
				pVehInfo->BackNode = pMapInfo.pNodeInfo[0].pUpNode[i].ID;
				Min = i;
			}
		}


		LaneMatch(pMapInfo.pNodeInfo[0].pUpNode[Min].LaneCnt,
							&pMapInfo.pNodeInfo[0].pUpNode[Min].laneInfo[0], pVehInfo);

		return 0;
	}
}

double Get_CompareValue(double ForwardLat, double ForwardLng, double BehindLat, double BehindLng, tVehData *pVehInfo)
{

	double p_dVerticalDis, p_dLaneDirection;
	double p_dRelDirection;
	double CompareValue = 10000;

	//两个节点，定位点构成的三角形 是否是锐角三角形
	if (!IsAcutetriangle(ForwardLat, ForwardLng, BehindLat, BehindLng,
			pVehInfo->Latitude, pVehInfo->Longitude))
	{

		//计算高度， 车辆到两节点线段的距离
		p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng, BehindLat,
				BehindLng, pVehInfo->Latitude, pVehInfo->Longitude);

		if (p_dVerticalDis < g_V2V_Parameter.MapMatch_ConfidenceDis_toLink)
		{

			//路段方向角度
			p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng, ForwardLat,
					ForwardLng);

			if (p_dLaneDirection >= 180)
				p_dLaneDirection = p_dLaneDirection - 360;

			if (pVehInfo->Heading >= 180)
				pVehInfo->Heading = pVehInfo->Heading - 360;

#if PRINT_LOG_TEST
			printf("\t\tp_dLaneDirection: = %lf pVehInfo->Heading = %lf\n",
					p_dLaneDirection,pVehInfo->Heading);
#endif
			p_dRelDirection = fabs(p_dLaneDirection - pVehInfo->Heading);
			if (p_dRelDirection >= 180)
				p_dRelDirection = p_dRelDirection - 360;
			p_dRelDirection = fabs(p_dRelDirection);

			CompareValue = CofficDis * p_dVerticalDis
					+ CofficHeading * p_dRelDirection;
#if PRINT_LOG_TEST
			printf(
					"\t\tp_dVerticalDis = %f, p_dRelDirection = %f, CompareValue = %f \n",
					p_dVerticalDis, p_dRelDirection, CompareValue);

#endif

		}
	}
	else
	{
#if PRINT_LOG_TEST
			printf("\t\tNot IN This Link \n");

#endif
		CompareValue = 10000;
	}
	return CompareValue;
}


int MapMatch_Pojcet_test(tMapInfo pMapInfo, tVehData *pVehInfo)
{
	int i, j, k;
	int ii = 10000, jj = 10000;
	double ForwardLat, BehindLat;
	double ForwardLng, BehindLng;

	double CompareValue = 10000, Temp_Min = 10000;
//	printf("Veh lat= %f, lng = %f\n", pVehInfo->Latitude, pVehInfo->Longitude);
	for (i = 0; i < pMapInfo.NodesCnt; i++)
	{
		for (j = 0; j < pMapInfo.pNodeInfo[i].UpNodeCnt; j++)
		{

#if PRINT_LOG_TEST
		printf("Node ID= %d\n\t UpNodeID = %d:\n", pMapInfo.pNodeInfo[i].ID,pMapInfo.pNodeInfo[i].pUpNode[j].ID);
#endif

			if (pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt != 0)
			{

				for (k = 0; k <= pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt; k++)
				{
					if (k == 0)
					{
						ForwardLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Latitude;
						ForwardLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Longitude;
						BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Latitude;
						BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Longitude;
					}
					else if (k == (pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointsCnt))
					{
						ForwardLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
						ForwardLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
						BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Latitude;
						BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Longitude;
					}
					else
					{
						ForwardLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Latitude;
						ForwardLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k].Longitude;
						BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Latitude;
						BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].RoadPointInfo[k-1].Longitude;
					}
#if PRINT_LOG_TEST
					printf("\t\tForwardLat = %f, ",ForwardLat);
					printf("ForwardLng = %f, ",ForwardLng);
					printf("BehindLat = %f, ",BehindLat);
					printf("BehindLng = %f\n, ",BehindLng);
#endif
					CompareValue = Get_CompareValue(ForwardLat, ForwardLng, BehindLat, BehindLng, pVehInfo);
					if (CompareValue < Temp_Min)
					{
							Temp_Min = CompareValue;
							ii = i;
							jj = j;
					}
#if PRINT_LOG_TEST
					printf("\t\t jj == %d && ii == %d, CompareValue = %f\n\n",jj,ii,CompareValue);
#endif
				}
			}
			else
			{
				ForwardLat = pMapInfo.pNodeInfo[i].nodePos.Latitude;
				ForwardLng = pMapInfo.pNodeInfo[i].nodePos.Longitude;
				BehindLat = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Latitude;
				BehindLng = pMapInfo.pNodeInfo[i].pUpNode[j].nodePos.Longitude;
				CompareValue = Get_CompareValue(ForwardLat, ForwardLng, BehindLat, BehindLng, pVehInfo);
			}

			if (CompareValue < Temp_Min)
			{
				Temp_Min = CompareValue;
				ii = i;
				jj = j;
			}
		}
	}


//	printf("jj == %d && ii == %d, CompareValue = %f\n\n",jj,ii,CompareValue);
	if (pVehInfo->Heading < 0)
		pVehInfo->Heading = pVehInfo->Heading + 360;

	if (jj == 10000 && ii == 10000)
	{

		pVehInfo->ForwardNode = 10000;
		pVehInfo->BackNode = 10000;
		return 0;
	}
	else
	{
		pVehInfo->ForwardNode = pMapInfo.pNodeInfo[ii].ID;
		pVehInfo->BackNode = pMapInfo.pNodeInfo[ii].pUpNode[jj].ID;

		if (pMapInfo.pNodeInfo[ii].pUpNode[jj].RoadPointsCnt != 0)
			return 2;
		else
			return 1;
	}
}

int MapMatch_RoadPints(tRoadPoint *proadPoints, int pointsCnt, tVehData *pVehInfo)
{
	int res = -1;

	double ForwardLat, BehindLat;
	double ForwardLng, BehindLng;

	double p_dLaneDirection, p_dVerticalDis;

	int i;
	for (i=0; i<pointsCnt-1; i++)
	{
		ForwardLat = proadPoints[i+1].Latitude;
		ForwardLng = proadPoints[i+1].Longitude;
		BehindLat = proadPoints[i].Latitude;
		BehindLng = proadPoints[i].Longitude;

		//两个节点，定位点构成的三角形 是否是锐角三角形
		if (!IsAcutetriangle(ForwardLat, ForwardLng, BehindLat, BehindLng,
				pVehInfo->Latitude, pVehInfo->Longitude) || 1)
		{
			//路段方向角度
			p_dLaneDirection = FG_GetTrueBear(BehindLat, BehindLng, ForwardLat,
						ForwardLng);

			if (p_dLaneDirection >= 180)
				p_dLaneDirection = p_dLaneDirection - 360;

			if (pVehInfo->Heading >= 180)
				pVehInfo->Heading = pVehInfo->Heading - 360;

			if (fabs(p_dLaneDirection - pVehInfo->Heading) < 15)
			{
				//到路段距离判断
				p_dVerticalDis = GetVerticalDis(ForwardLat, ForwardLng, BehindLat,
						BehindLng, pVehInfo->Latitude, pVehInfo->Longitude);

				if (p_dVerticalDis < g_V2V_Parameter.MapMatch_ConfidenceDis_toLink)
				{
					res = 0;
				}else
				{
					res = -1;
				}
			}else
			{
				res = -1;
			}
		}else
		{
			res = -1;
		}
	}

	return res;
}

