/*
 * GetMapPoints.c
 *
 *  Created on: Aug 27, 2019
 *      Author: wanji-hxy
 */

#include "GetMapPoints.h"


void LoadXml(MessageFrame_t **pSendMsg, char *pConfigFileName)
{
	char *p;
	char *xer_buf = NULL;
	int fsize = 0;
	FILE *pxerfp;

	pxerfp = fopen(pConfigFileName, "r"); //Read all bytes in RSI config file
	fseek(pxerfp, 0, SEEK_END);
	fsize = ftell(pxerfp);

	xer_buf = calloc(1, fsize);
	p = xer_buf;

	rewind(pxerfp);
	fread(xer_buf, fsize, 1, pxerfp);
	fclose(pxerfp);


	asn_dec_rval_t rval;
	rval = xer_decode(0, &asn_DEF_MessageFrame, (void **) pSendMsg, xer_buf,
			fsize);
	if (rval.code != RC_OK)
	{
		(*pSendMsg)->present = MessageFrame_PR_NOTHING;

	}

	free(p);
}


void Out_Map(tMapInfo *map)
{
	int i, j, s, m, n ;
	if (map == NULL)
	{
		printf("Can not find Map Message!!!\n");
		return;
	}
	printf("NodesCnt:%d\n", map->NodesCnt);
	tNodeInfo *nodeInfo;
	if (map->NodesCnt == 0)
	{
		printf("map->pNodeInfo is NULL\n");
		return;
	}
	nodeInfo = map->pNodeInfo;
	for (i = 0; i < map->NodesCnt; i++)
	{
		printf("\t %d\n", nodeInfo[i].ID);
		printf("\t %f\n", nodeInfo[i].nodePos.Latitude);
		printf("\t %f\n", nodeInfo[i].nodePos.Longitude);
		printf("\t UpNodeCnt:%d\n", nodeInfo[i].UpNodeCnt);
		tUpPointInfo *upPointInfo;
		if (nodeInfo[i].pUpNode == 0)
		{
			printf("map->pNodeInfo->pUpNode is NULL\n");
		}
		else
		{
			upPointInfo = nodeInfo[i].pUpNode;
			for (j = 0; j < nodeInfo[i].UpNodeCnt; j++)
			{
				printf("\t\t %d\n", upPointInfo[j].ID);
				printf("\t\t %f\n", upPointInfo[j].nodePos.Latitude);
				printf("\t\t %f\n", upPointInfo[j].nodePos.Longitude);

				printf("\t\t SpeedCnt:%d\n", upPointInfo[j].SpeedCnt);
				tMapSpeedLimitInfo *mapSpeedLimitInfo;
				if (upPointInfo[j].SpeedCnt == 0) {
					printf("map->pNodeInfo->pUpNode->MapSpeedLimits is NULL\n");
				} else
				{
					mapSpeedLimitInfo = upPointInfo[j].MapSpeedLimits;
					printf("\t\tupPointInfo.SpeedCnt = %d\n", upPointInfo[j].SpeedCnt);
					if (mapSpeedLimitInfo != NULL) {
						for (s = 0; s < upPointInfo[j].SpeedCnt; s++) {
							printf("\t\t\t %d\n",
									mapSpeedLimitInfo[s].SpeedLimitType);
							printf("\t\t\t %f\n",
									mapSpeedLimitInfo[s].SpeedLimitValue);
						}
					}
				}

				printf("\t\t MoveMentCnt:%d\n", upPointInfo[j].MoveMentCnt);
				tMoveMent *moveMents;
				if (upPointInfo[j].MoveMentCnt == 0) {
					printf("map->pNodeInfo->pUpNode->pMoveMents is NULL\n");
				} else {
					moveMents = upPointInfo[j].pMoveMents;
					for (s = 0; s < upPointInfo[j].MoveMentCnt; s++) {
						printf("\t\t\t %d\n", moveMents[s].ID);
						printf("\t\t\t %f\n", moveMents[s].Lat);
						printf("\t\t\t %f\n", moveMents[s].Lng);
						if (moveMents[s].Phase != NULL)
							printf("\t\t\t %d\n", *moveMents[s].Phase);
						else
							printf("\t\t\t moveMents[%d].Phase is NULL\n", s);
					}
				}

				printf("\t\t RoadPointsCnt:%d\n", upPointInfo[j].RoadPointsCnt);
				tRoadPoint *roadPoint;
				if (upPointInfo[j].RoadPointsCnt == 0)
				{
					printf("map->pNodeInfo->pUpNode->pRoadPoints is NULL\n");
				}
				else
				{
					roadPoint = upPointInfo[j].RoadPointInfo;
					for (s = 0; s < upPointInfo[j].RoadPointsCnt; s++)
					{
						printf("\t\t\t %f, %f\n", roadPoint[s].Latitude, roadPoint[s].Longitude);
					}
				}

				for (m = 0; m < upPointInfo[j].LaneCnt; m++)
				{
					printf("\t\t\t %d, %f, %d\n", upPointInfo[j].laneInfo[m].ID,
							upPointInfo[j].laneInfo[m].LaneWidth, upPointInfo[j].laneInfo[m].roadPointCnt);


					for (n = 0;
							n < upPointInfo[j].laneInfo[m].roadPointCnt;
							n++)
					{
						printf("\t\t\t\t %f, %f\n",
								upPointInfo[j].laneInfo[m].roadPointInfo[n].Latitude,
								upPointInfo[j].laneInfo[m].roadPointInfo[n].Longitude);
					}

				}



			}
		}
	}
}
int cpyMapInfo(tMapInfo *pSrcMapInfo, tMapInfo *pDstMapInfo)
{
	int i, j, k;
	memcpy(pDstMapInfo, pSrcMapInfo, sizeof(tMapInfo));
	for (i = 0; i < pDstMapInfo->NodesCnt; i++)
	{
		for (j = 0; j < pDstMapInfo->pNodeInfo[i].UpNodeCnt; j++)
		{
			for (k = 0; k < pDstMapInfo->pNodeInfo[i].pUpNode[j].MoveMentCnt; k++)
			{
				pDstMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase = ALLOC_OBJ(nint32_t, 1);
				*pDstMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase = *pSrcMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase;
			}
		}
	}
	return 0;
}
int ClearMapInfo(tMapInfo *pMapInfo)
{
	int i, j, k;
	for (i = 0; i < NODE_MAX_NUM; i++)
	{
		for (j = 0; j < LINK_MAX_NUM; j++)
		{
			for (k = 0; k < MOVEMENT_MAX_NUM; k++)
			{
				if(pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase != NULL)
				{
					free(pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase);
					pMapInfo->pNodeInfo[i].pUpNode[j].pMoveMents[k].Phase = NULL;
				}
			}
			pMapInfo->pNodeInfo[i].pUpNode[j].MoveMentCnt = 0;
		}
		pMapInfo->pNodeInfo[i].UpNodeCnt = 0;
	}
	pMapInfo->NodesCnt = 0;
	//memset(pMapInfo, 0, sizeof(tMapInfo));
	return 0;
}
int GetMapInfo(tMapInfo *pMapInfo, MessageFrame_t *pMAPMsg)
{
	tNodeInfo *nodeInfo;

	int i, j, k, m, n, nLink, nNode;
	nNode = pMAPMsg->choice.mapFrame.nodes.list.count;
	pMapInfo->NodesCnt = pMAPMsg->choice.mapFrame.nodes.list.count;

	nodeInfo = pMapInfo->pNodeInfo;
	Node_t **node = pMAPMsg->choice.mapFrame.nodes.list.array;

	//获取节点信息
	for (i = 0; i < nNode; i++)
	{
		nodeInfo[i].ID = (*node[i]).id.id;
		nodeInfo[i].nodePos.Latitude = ((*node[i]).refPos.lat * 1.0) / 10000000;
		nodeInfo[i].nodePos.Longitude = ((*node[i]).refPos.Long * 1.0) / 10000000;
		nodeInfo[i].UpNodeCnt = 0;
	}

	//获取路段信息
	Link_t **link;
	for (i = 0; i < nNode; i++)
	{

		//路段数量
		if ((*pMAPMsg->choice.mapFrame.nodes.list.array[i]).inLinks == NULL)
		{
			nLink = 0;
		}
		else
		{
			nLink =
					(*pMAPMsg->choice.mapFrame.nodes.list.array[i]).inLinks->list.count;
			nodeInfo[i].UpNodeCnt = nLink;

		}


		for (j = 0; j < nLink; j++)
		{
			int s, nSpeed, nMoveMent, nRoadPoint;
			link = (*node[i]).inLinks->list.array;
			nodeInfo[i].pUpNode[j].ID = (*link[j]).upstreamNodeId.id;


			//获取限速信息
			if ((*link[j]).speedLimits != NULL) {
				nSpeed = (*link[j]).speedLimits->list.count;
//				printf(
//						"###########i = %d j = %d nSpeed = %d######################\n",
//						i, j, nSpeed);
				nodeInfo[i].pUpNode[j].SpeedCnt = nSpeed;
				struct RegulatorySpeedLimit **speedLimit =
						(*link[j]).speedLimits->list.array;
				for (s = 0; s < nSpeed; s++) {
					nodeInfo[i].pUpNode[j].MapSpeedLimits[s].SpeedLimitType =
							(*speedLimit[s]).type;			//限速类型
					nodeInfo[i].pUpNode[j].MapSpeedLimits[s].SpeedLimitValue =
							(*speedLimit[s]).speed * 0.02;	//车速
				}
			} else {
				nodeInfo[i].pUpNode[j].SpeedCnt = 0;
			}

			//获取连接关系
			if ((*link[j]).movements != NULL) {
				nMoveMent = (*link[j]).movements->list.count;
				struct Movement **movement = (*link[j]).movements->list.array;
				//				nodeInfo[i].pUpNode[j].pMoveMents = (tMoveMent *) malloc(
				//						sizeof(tMoveMent) * nMoveMent);

				nodeInfo[i].pUpNode[j].MoveMentCnt = nMoveMent;

			for (s = 0; s < nMoveMent; s++) {
				if (movement[s] == NULL) {
					printf("pMoveMents %d is NULL\n", s);
				}
				nodeInfo[i].pUpNode[j].pMoveMents[s].ID =
						(*movement[s]).remoteIntersection.id;
				if ((*movement[s]).phaseId != NULL) {
					nodeInfo[i].pUpNode[j].pMoveMents[s].Phase = (int *) malloc(
							sizeof(int));
					*nodeInfo[i].pUpNode[j].pMoveMents[s].Phase =
							*(*movement[s]).phaseId;
				} else {
					nodeInfo[i].pUpNode[j].pMoveMents[s].Phase = NULL;
				}
				//寻找节点坐标
				k = 0;
				while (nodeInfo[k].ID != nodeInfo[i].pUpNode[j].pMoveMents[s].ID
						&& k < nNode) {
					k++;
				}

				if (k < nNode) {
					nodeInfo[i].pUpNode[j].pMoveMents[s].Lat =
							nodeInfo[k].nodePos.Latitude;
					nodeInfo[i].pUpNode[j].pMoveMents[s].Lng = nodeInfo[k].nodePos.Longitude;

				}
			}
#if __PRINT_DEBUGLOG_
			printf("nMoveMent\n");
#endif
		}
		else
		{
			nodeInfo[i].pUpNode[j].MoveMentCnt = 0;
		}


			//获取中间点信息
			if ((*link[j]).points != NULL)
			{
				nRoadPoint = (*link[j]).points->list.count;
				nodeInfo[i].pUpNode[j].RoadPointsCnt = nRoadPoint;

				struct RoadPoint **roadpoint = (*link[j]).points->list.array;

				for (s = 0; s < nRoadPoint; s++)
				{
					nodeInfo[i].pUpNode[j].RoadPointInfo[s].Latitude =
							(*roadpoint[s]).posOffset.offsetLL.choice.position_LatLon.lat / 10000000.0;	//纬度
					nodeInfo[i].pUpNode[j].RoadPointInfo[s].Longitude =
							(*roadpoint[s]).posOffset.offsetLL.choice.position_LatLon.lon / 10000000.0;	//经度
				}
			}
			else
			{
				nodeInfo[i].pUpNode[j].RoadPointsCnt = 0;
			}

			//获取车道信息
			Lane_t **lane;
			nodeInfo[i].pUpNode[j].LaneCnt = (*link[j]).lanes.list.count;
			for (m = 0; m < nodeInfo[i].pUpNode[j].LaneCnt; m++)
			{
				lane = (*link[j]).lanes.list.array;

				if ((*lane[m]).points != NULL)
				{
					nodeInfo[i].pUpNode[j].laneInfo[m].ID = (*lane[m]).laneID;

					nodeInfo[i].pUpNode[j].laneInfo[m].roadPointCnt = (*lane[m]).points->list.count;
						//中间点
						for (n = 0; n < (*lane[m]).points->list.count; n++)
						{

							nodeInfo[i].pUpNode[j].laneInfo[m].roadPointInfo[n].Latitude =
									(*lane[m]).points->list.array[n]->posOffset.offsetLL.choice.position_LatLon.lat
											/ 10000000.0;

							nodeInfo[i].pUpNode[j].laneInfo[m].roadPointInfo[n].Longitude =
									(*lane[m]).points->list.array[n]->posOffset.offsetLL.choice.position_LatLon.lon
											/ 10000000.0;
						}
				}
				else
				{
					nodeInfo[i].pUpNode[j].laneInfo[m].roadPointCnt = 0;
				}

			} //loop in lane

			//寻找当前link对应上游节点坐标
			k = 0;
			while (nodeInfo[k].ID != nodeInfo[i].pUpNode[j].ID && k < nNode)
			{
				k++;
			}
			if (k < nNode) {
				nodeInfo[i].pUpNode[j].nodePos.Latitude = nodeInfo[k].nodePos.Latitude;
				nodeInfo[i].pUpNode[j].nodePos.Longitude = nodeInfo[k].nodePos.Longitude;
			}

#if 0
			printf("Node %d 's UpNode ID = %d, Lat = %f, Lng = %f\n",
					nodeInfo[i].ID, nodeInfo[i].pUpNode[j].ID,
					nodeInfo[i].pUpNode[j].nodePos.Latitude , nodeInfo[i].pUpNode[j].nodePos.Longitude);
			printf("\nupNode cnt: = %d\n", nodeInfo[i].UpNodeCnt);
#endif

		}	//loop in link
	}  //loop in node
	return 0;
}
