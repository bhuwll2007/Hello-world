/*
 * SAPT_Algorithm.c
 *
 *  Created on: Jun 6, 2018
 *      Author: root
 */
#include "SPAT_Algorithm.h"
void ClearSpatInfo(tSpatMsg *pSpatInfo)
{
	int i = 0;
	if (pSpatInfo != NULL)
	{
		if (pSpatInfo->pIntersectionInfo != NULL)
		{
			for (i = 0; i < pSpatInfo->IntersectionCnt; i++)
			{
				pSpatInfo->pIntersectionInfo[i].ID = 0;
				pSpatInfo->pIntersectionInfo[i].Status = 0;
				pSpatInfo->pIntersectionInfo[i].Cycle = 0;
				pSpatInfo->pIntersectionInfo[i].PhaseCnt = 0;
				if (pSpatInfo->pIntersectionInfo[i].pPhaseInfo != NULL)
				{
					memset(pSpatInfo->pIntersectionInfo[i].pPhaseInfo, 0x00,
							sizeof(tPhaseInfo));
					free(pSpatInfo->pIntersectionInfo[i].pPhaseInfo);
					pSpatInfo->pIntersectionInfo[i].pPhaseInfo = NULL;
				}
			}
			free(pSpatInfo->pIntersectionInfo);
			pSpatInfo->pIntersectionInfo = NULL;
			printf("\t\t\t\t   free\n");
		}
	}
}

void GetSpatInfo(MessageFrame_t *pSpatMsg, tSpatMsg *pSpatInfo)
{
	int i, j, k;
	int InterSecitonCnt, LightStateCnt;

	InterSecitonCnt = pSpatMsg->choice.spatFrame.intersections.list.count;
	pSpatInfo->IntersectionCnt = InterSecitonCnt;

	IntersectionState_t *pIntersection = NULL;
	//pIntersection = ALLOC_OBJ(IntersectionState_t, InterSecitonCnt);
	printf("\t\t\t\t   ALLOC_OBJ\n");
	pSpatInfo->pIntersectionInfo = ALLOC_OBJ(tIntersectionInfo,
			InterSecitonCnt);

	for (i = 0; i < InterSecitonCnt; i++)
	{
		//提取路口信息
		pIntersection = pSpatMsg->choice.spatFrame.intersections.list.array[i];

		pSpatInfo->pIntersectionInfo[i].ID = pIntersection->intersectionId.id;
		pSpatInfo->pIntersectionInfo[i].Status = pIntersection->status.buf[0]
				+ (pIntersection->status.buf[1] >> 8);
		//获取相位个数
		pSpatInfo->pIntersectionInfo[i].PhaseCnt =
				pIntersection->phases.list.count;

		Phase_t *pPhase;
		pPhase = ALLOC_OBJ(Phase_t, pSpatInfo->pIntersectionInfo[i].PhaseCnt);
		pSpatInfo->pIntersectionInfo[i].pPhaseInfo = malloc(
				pSpatInfo->pIntersectionInfo[i].PhaseCnt * sizeof(tPhaseInfo));

		for (j = 0; j < pSpatInfo->pIntersectionInfo[i].PhaseCnt; j++)
		{
			pPhase[j] = *pIntersection->phases.list.array[j];
			//相位ID
			pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].ID = pPhase[j].id;
			//灯色状态个数
			LightStateCnt = pPhase[j].phaseStates.list.count;

			PhaseState_t *pPhaseState;
			pPhaseState = ALLOC_OBJ(PhaseState_t, LightStateCnt);

			for (k = 0; k < LightStateCnt; k++)
			{
				pPhaseState[k] = *pPhase[j].phaseStates.list.array[k];
			}

			for (k = 0; k < LightStateCnt; k++)
			{
				if (pPhaseState[k].timing != NULL
						&& pPhaseState[k].timing->present
								== TimeChangeDetails_PR_counting)
				{
					if (pPhaseState[k].timing->choice.counting.startTime == 0)
					{
						pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].Status =
								pPhaseState[k].light;

						pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].TimeLeft =
								pPhaseState[k].timing->choice.counting.likelyEndTime
										/ 10;
#if 0
						printf("Status = %d\n", pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].Status);
						printf("TimeLeft = %d\n", pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].TimeLeft);
#endif
					}

					if (pPhaseState[k].timing->choice.counting.nextDuration
							== NULL)
						continue;

					if (pPhaseState[k].light == LightState_red)
					{
						pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].RedTime =
								*pPhaseState[k].timing->choice.counting.nextDuration
										/ 10;
					}

					if (pPhaseState[k].light == LightState_permissive_green)
					{
						pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].GreenTime =
								*pPhaseState[k].timing->choice.counting.nextDuration
										/ 10;
					}

					if (pPhaseState[k].light == LightState_yellow)
					{
						pSpatInfo->pIntersectionInfo[i].pPhaseInfo[j].YellowTime =
								*pPhaseState[k].timing->choice.counting.nextDuration
										/ 10;
					}
				}
			}
			if (pPhaseState != NULL)
			{
				free(pPhaseState);
				pPhaseState = NULL;
			}
		}
		if (pPhase != NULL)
		{
			free(pPhase);
			pPhase = NULL;
		}
	}
	pIntersection = NULL;
}

void GetForward_Intersection(tVehData *pHV, tSpatMsg *pSpatInfo,
		tMapInfo *pMapInfo, tSpeedGudie_Input *pSpeedGudie_Input)
{
	int i, j, k, n, m;
	double DownNode_Lat, DownNode_Lng;
	Point tPoint;

	memset(pSpeedGudie_Input, 0x00, sizeof(tSpeedGudie_Input));

	//根据地图匹配结果，在地图信息中找到下游转向关系
//	printf("Find Turn Section\n");
	for (i = 0; i < pMapInfo->NodesCnt; i++)
	{
		//找到车辆在地图中前方节点
		if (pHV->ForwardNode == pMapInfo->pNodeInfo[i].ID)
		{
			//红绿灯所在位置经纬度
			pSpeedGudie_Input->Lat = pMapInfo->pNodeInfo[i].nodePos.Latitude;
			pSpeedGudie_Input->Lng = pMapInfo->pNodeInfo[i].nodePos.Longitude;

			for (j = 0; j < pMapInfo->pNodeInfo[i].UpNodeCnt; j++)
			{
				//找前方节点的上游节点
				if (pHV->BackNode == pMapInfo->pNodeInfo[i].pUpNode[j].ID)
				{
					printf("Toward Forward Node Found\n");
					//下游通行方向个数，转向关系个数---------实际的红绿灯相位个数
					pSpeedGudie_Input->PhaseCnt =
							pMapInfo->pNodeInfo[i].pUpNode[j].MoveMentCnt;

					for (k = 0; k < pSpeedGudie_Input->PhaseCnt; k++)
					{
						//获取转向关系中ID对应的相位ID, 以及对应下游节点经纬度
						pSpeedGudie_Input->pLightInfo[k].ID =
								*pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase;
						DownNode_Lat =
								pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Lat;
						DownNode_Lng =
								pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Lng;

						for (n = 0; n < pSpatInfo->IntersectionCnt; n++)
						{
							//找到车辆前方节点对应的路口ID
							if (pHV->ForwardNode
									== pSpatInfo->pIntersectionInfo[n].ID)
							{
								printf(
										"---------------------------------------Phase Found-----------------------------------");

								for (m = 0;
										m
												< pSpatInfo->pIntersectionInfo[n].PhaseCnt;
										m++)
								{
									//找到相位ID在Spat信息中匹配的相位ID
									if (pSpeedGudie_Input->pLightInfo[k].ID
											== pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].ID)
									{
										pSpeedGudie_Input->pLightInfo[k].Status =
												pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].Status;
										pSpeedGudie_Input->pLightInfo[k].GreenTime =
												pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].GreenTime;
										pSpeedGudie_Input->pLightInfo[k].YellowTime =
												pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].YellowTime;
										pSpeedGudie_Input->pLightInfo[k].RedTime =
												pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].RedTime;
										pSpeedGudie_Input->pLightInfo[k].TimeLeft =
												pSpatInfo->pIntersectionInfo[n].pPhaseInfo[m].TimeLeft;

										double LinkDir =
												FG_GetTrueBear(
														pMapInfo->pNodeInfo[i].pUpNode[j].nodePos.Latitude,
														pMapInfo->pNodeInfo[i].pUpNode[j].nodePos.Longitude,
														pSpeedGudie_Input->Lat,
														pSpeedGudie_Input->Lng);

										tPoint = gpsToRelativeLoc(
												pSpeedGudie_Input->Lng,
												pSpeedGudie_Input->Lat, LinkDir,
												DownNode_Lng, DownNode_Lat);

//										tPoint = gpsToRelativeLoc(pHV->Latitude,
//												pHV->Longitude, pHV->Heading,
//												DownNode_Lat, DownNode_Lng);

										if (tPoint.x_ > 25)
											pSpeedGudie_Input->pLightInfo[k].Direction =
													3;   //右转
										else if (tPoint.x_ < -25)
											pSpeedGudie_Input->pLightInfo[k].Direction =
													1;	  //左转
										else
											pSpeedGudie_Input->pLightInfo[k].Direction =
													2;	  //直行

										printf(
												"ID:%d\tGreen:%d\tYellow:%d\tRed:%d\tStatus:%d\tTimeleft:%d\tDirection:%d\n",
												pSpeedGudie_Input->pLightInfo[k].ID,
												pSpeedGudie_Input->pLightInfo[k].GreenTime,
												pSpeedGudie_Input->pLightInfo[k].YellowTime,
												pSpeedGudie_Input->pLightInfo[k].RedTime,
												pSpeedGudie_Input->pLightInfo[k].Status,
												pSpeedGudie_Input->pLightInfo[k].TimeLeft,
												pSpeedGudie_Input->pLightInfo[k].Direction);
									}
								}
#if 1
								printf(
										"=================PhaseCnt:%d===============\n",
										pSpatInfo->pIntersectionInfo[n].PhaseCnt);

								break;
#endif
							}
							else
							{
								pSpeedGudie_Input->PhaseCnt = 0;
							}
						}
					}
				}
			}
		}
	}
}

int LightDirection_Detect(Point tPoint)
{
	if (tPoint.y_ >= 0)
	{
		if (tPoint.x_ > g_Spat_Parameter.LightDir_Dis_Value)
			return Turn_Right;
		else if (tPoint.x_ < -g_Spat_Parameter.LightDir_Dis_Value)
			return Turn_Left;
		else
			return Go_straight;
	}
	else
		return Turn_Round;
}

tResult_Spat SpeedGuide(tVehData *pHV, tSpeedGudie_Input *pSpeedGudie_Input)
{

	tResult_Spat CResult =
	{ Type_DEFAULT, V2I_STR_DEFAULT, PRIO_DEFAULT, -1, -1, -1 };
	CResult.wType = Type_SPAT;
	int i;
	double Distance = FG_Getdistance(pHV->Latitude, pHV->Longitude,
			pSpeedGudie_Input->Lat, pSpeedGudie_Input->Lng);
	CResult.Distance = (Distance - pHV->VehicleLength / 2)
			- g_Spat_Parameter.DistoCrossRdMid; // 距离为车头到停车线的距离
	CResult.RelBearing = (uint32_t) FG_GetRelBear(pHV->Latitude, pHV->Longitude,
			pSpeedGudie_Input->Lat, pSpeedGudie_Input->Lng, pHV->Heading);
	CResult.PhaseCnt = pSpeedGudie_Input->PhaseCnt;

	if (CResult.Distance < 0)
	{
		return CResult;
	}
	for (i = 0; i < pSpeedGudie_Input->PhaseCnt; i++)
	{
		CResult.tUpPhaseInfo[i].Status =
				pSpeedGudie_Input->pLightInfo[i].Status;
		CResult.tUpPhaseInfo[i].TimeLeft =
				pSpeedGudie_Input->pLightInfo[i].TimeLeft;
		CResult.tUpPhaseInfo[i].Direction =
				pSpeedGudie_Input->pLightInfo[i].Direction;

		if (CResult.Distance
				< g_Spat_Parameter.SPATWarnDis
						+ g_Spat_Parameter.WarnCalcTime * pHV->Speed)
		{

			CResult.wStr = GLOSA_Warning;

			CResult.Cycle = pSpeedGudie_Input->pLightInfo[i].GreenTime
					+ pSpeedGudie_Input->pLightInfo[i].YellowTime
					+ pSpeedGudie_Input->pLightInfo[i].RedTime;

			if (pSpeedGudie_Input->pLightInfo[i].Status == App_LightState_Red
					|| pSpeedGudie_Input->pLightInfo[i].Status
							== App_LightState_Yellow)
			{
				double Va = 0.0;
				double Vb = 0.0;
				if (pSpeedGudie_Input->pLightInfo[i].Status
						== App_LightState_Red)
				{
					Va = Distance / pSpeedGudie_Input->pLightInfo[i].TimeLeft; //距离绿灯开始的时间
					Vb =
							Distance
									/ (pSpeedGudie_Input->pLightInfo[i].TimeLeft
											+ pSpeedGudie_Input->pLightInfo[i].GreenTime); //距离下次黄灯开始的时间
				}

				else if (pSpeedGudie_Input->pLightInfo[i].Status
						== App_LightState_Yellow)
				{
					Va = Distance
							/ (pSpeedGudie_Input->pLightInfo[i].TimeLeft
									+ pSpeedGudie_Input->pLightInfo[i].RedTime); //距离绿灯开始的时间
					Vb =
							Distance
									/ (pSpeedGudie_Input->pLightInfo[i].TimeLeft
											+ pSpeedGudie_Input->pLightInfo[i].RedTime
											+ pSpeedGudie_Input->pLightInfo[i].GreenTime); //距离下次黄灯开始的时间
				}

//				if (pHV->Speed > Va)
//				{
//					//闯红灯预警
//					CResult.wStr = RLVW_Waring;
//				}

				//时速>剩余距离，那么到终点肯定存在闯红灯风险
//				if((pHV->Speed * 3.6 > Distance) &&((pSpeedGudie_Input->pLightInfo[i].Status
//						== App_LightState_Red) || (pSpeedGudie_Input->pLightInfo[i].Status
//								== App_LightState_Yellow)))
				if ((pHV->Speed * 3.6 > CResult.Distance)
						&& ((pSpeedGudie_Input->pLightInfo[i].Status
								== App_LightState_Red)
								|| (pSpeedGudie_Input->pLightInfo[i].Status
										== App_LightState_Yellow)))
				{
					//闯红灯预警
					CResult.wStr = RLVW_Waring;
				}
				if (Va > g_Spat_Parameter.SpeedLimit_High
						&& (Vb > g_Spat_Parameter.SpeedLimit_Low
								&& Vb < g_Spat_Parameter.SpeedLimit_High))
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = ((
							(pHV->Speed > Vb) ? pHV->Speed : Vb) + 2) * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low = Vb * 3.6;
				}
				else if ((Va > g_Spat_Parameter.SpeedLimit_Low
						&& Va < g_Spat_Parameter.SpeedLimit_High)
						&& (Vb > g_Spat_Parameter.SpeedLimit_Low
								&& Vb < g_Spat_Parameter.SpeedLimit_High))
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = Va * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low = Vb * 3.6;
				}
				else if ((Va > g_Spat_Parameter.SpeedLimit_Low
						&& Va < g_Spat_Parameter.SpeedLimit_High)
						&& Vb < g_Spat_Parameter.SpeedLimit_Low)
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = Va * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low =
							g_Spat_Parameter.SpeedLimit_Low * 3.6;
				}
				else if (Va > g_Spat_Parameter.SpeedLimit_High
						&& Vb < g_Spat_Parameter.SpeedLimit_Low)
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High =
							g_Spat_Parameter.SpeedLimit_High * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low =
							g_Spat_Parameter.SpeedLimit_Low * 3.6;
				}
				else
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = 0;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low = 0;
					CResult.tUpPhaseInfo[i].SuggestSpeed = 0;  //建议停车等待（速度为0）
					continue;
				}

				if (pHV->Speed > CResult.tUpPhaseInfo[i].SuggestSpeed_High)
				{
					if ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low)
							< (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
									- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
											% 5)
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								(double) ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
										- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
												% 5);
					}
					else
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								CResult.tUpPhaseInfo[i].SuggestSpeed_High;
					}
				}
				else if (pHV->Speed < CResult.tUpPhaseInfo[i].SuggestSpeed_Low)
				{
					if ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
							> ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
									+ 5)
									- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
											+ 5) % 5))
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								(double) ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
										+ 5)
										- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
												+ 5) % 5); //建议速度的步进值为5
					}
					else
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								CResult.tUpPhaseInfo[i].SuggestSpeed_Low;
					}
				}
				else
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed = pHV->Speed;
				}

			}

			if (pSpeedGudie_Input->pLightInfo[i].Status == App_LightState_Green)
			{

				double Vc = 0.0;
				double Vd = 0.0;
				Vc = Distance / pSpeedGudie_Input->pLightInfo[i].TimeLeft; //距离绿灯结束的时间
				Vd = Distance
						/ (pSpeedGudie_Input->pLightInfo[i].TimeLeft
								+ pSpeedGudie_Input->pLightInfo[i].RedTime
								+ pSpeedGudie_Input->pLightInfo[i].YellowTime); //距离下次绿灯开始的时间

				if (Vc > g_Spat_Parameter.SpeedLimit_High
						&& (Vd > g_Spat_Parameter.SpeedLimit_Low
								&& Vd < g_Spat_Parameter.SpeedLimit_High))
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = Vd * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low =
							g_Spat_Parameter.SpeedLimit_Low * 3.6;
				}
				else if (Vd < g_Spat_Parameter.SpeedLimit_Low
						&& (Vc > g_Spat_Parameter.SpeedLimit_Low
								&& Vc < g_Spat_Parameter.SpeedLimit_High))
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High =
							g_Spat_Parameter.SpeedLimit_High * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low = Vc * 3.6;
				}
				else if (Vd > g_Spat_Parameter.SpeedLimit_Low
						&& Vc < g_Spat_Parameter.SpeedLimit_High)
				{
					if (pHV->Speed < (Vc + Vd) / 2)
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed_High = Vd * 3.6;
						CResult.tUpPhaseInfo[i].SuggestSpeed_Low =
								g_Spat_Parameter.SpeedLimit_Low * 3.6;
					}
					else if (pHV->Speed >= (Vc + Vd) / 2)
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed_High =
								g_Spat_Parameter.SpeedLimit_High * 3.6;
						CResult.tUpPhaseInfo[i].SuggestSpeed_Low = Vc * 3.6;
					}
				}
				else if (Vc < g_Spat_Parameter.SpeedLimit_Low
						|| Vd > g_Spat_Parameter.SpeedLimit_High)
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed_High =
							g_Spat_Parameter.SpeedLimit_High * 3.6;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low =
							g_Spat_Parameter.SpeedLimit_Low * 3.6;
				}
				else
				{  //  Vd < SpeedLimit_Low && Vc > SpeedLimit_High
					CResult.tUpPhaseInfo[i].SuggestSpeed_High = 0;
					CResult.tUpPhaseInfo[i].SuggestSpeed_Low = 0;
					CResult.tUpPhaseInfo[i].SuggestSpeed = 0;  //建议停车等待（速度为0）
					continue;
				}

				if (pHV->Speed > CResult.tUpPhaseInfo[i].SuggestSpeed_High)
				{
					if ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low)
							< (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
									- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
											% 5)
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								(double) ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
										- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
												% 5);
					}
					else
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								CResult.tUpPhaseInfo[i].SuggestSpeed_High;
					}
				}
				else if (pHV->Speed < CResult.tUpPhaseInfo[i].SuggestSpeed_Low)
				{
					if ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_High)
							> (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
									+ 5)
									- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
											+ 5) % 5)
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								(double) ((int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
										+ 5)
										- (int) (CResult.tUpPhaseInfo[i].SuggestSpeed_Low
												+ 5) % 5); //建议速度的步进值为5
					}
					else
					{
						CResult.tUpPhaseInfo[i].SuggestSpeed =
								CResult.tUpPhaseInfo[i].SuggestSpeed_Low;
					}
				}
				else
				{
					CResult.tUpPhaseInfo[i].SuggestSpeed = pHV->Speed;
				}
			}
		}
	}

	return CResult;
}

