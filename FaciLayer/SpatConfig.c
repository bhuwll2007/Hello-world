/*
 * RsiConfig.c
 *
 *  Created on: 20200316
 *      Author: chaizhijun
 */
#include "SpatConfig.h"

static pthread_mutex_t s_mutexSpatRecvMsg;
static tQueue_RevLst *s_QueueSendSpat = NULL;

/**********************************************
*功能：释放信号机数据结构变量资源
*输入参数： void
*输出参数： 无
*函数返回值： 无
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
void FG_SpatRecvMsgFree(void)
{
	free(s_QueueSendSpat);
}

/**********************************************
*功能：获取接收到的信号机数据结构变量
*输入参数： 无
*输出参数：*p_pucDestBuf得到要广播的数据buff,*p_punLen获取到的buff字节长度
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
int FG_SpatRecvMsgGet(uint8_t *p_pucDestBuf, uint32_t *p_punLen)
{
	if (NULL == p_pucDestBuf)
	{
		return -1;
	}

	if (NULL == s_QueueSendSpat)
	{//先申请资源
		s_QueueSendSpat = calloc(1, (sizeof(tQueue_RevLst)));
		if (NULL == s_QueueSendSpat)
		{
			printf("--%s %s file %s line:%d\n",
			__DATE__, __TIME__, __FILE__, __LINE__);
			return -1;
		}

		memset(s_QueueSendSpat, 0x00, sizeof(tQueue_RevLst));
		sem_init(&(s_QueueSendSpat->m_product_number), 0, 0);
		sem_init(&(s_QueueSendSpat->m_blank_number), 0, NUM);
		pthread_mutex_init(s_QueueSendSpat, NULL);
		return -1;
	}
	else
	{
		//等到队列中的数据
		sem_wait(&(s_QueueSendSpat->m_product_number));

		pthread_mutex_lock(&s_mutexSpatRecvMsg);

		//从OUT队列中读出buff
		memcpy(p_pucDestBuf, s_QueueSendSpat->m_stcqueue[s_QueueSendSpat->m_nQIndx_Out].macBuff,
					(s_QueueSendSpat->m_stcqueue[s_QueueSendSpat->m_nQIndx_Out].mnBuffLen));
		*p_punLen = s_QueueSendSpat->m_stcqueue[s_QueueSendSpat->m_nQIndx_Out].mnBuffLen;
		s_QueueSendSpat->m_nQIndx_Out++;
		s_QueueSendSpat->m_nQIndx_Out %= NUM;

		sem_post(&(s_QueueSendSpat->m_blank_number));

		pthread_mutex_unlock(&s_mutexSpatRecvMsg);
	}

	return 0;
}

/**********************************************
*功能：填充接收到的信号机数据
*输入参数： p_rectSpatInfo  信号机结构数据
*输出参数： 无
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
int FG_SpatRecvMsgSet(tSpatMsg p_rectSpatInfo)
{
	if (NULL == s_QueueSendSpat)
	{//先申请资源
		s_QueueSendSpat = calloc(1, (sizeof(tQueue_RevLst)));
		if (NULL == s_QueueSendSpat)
		{
			printf("--%s %s file %s line:%d\n",
			__DATE__, __TIME__, __FILE__, __LINE__);
			return -1;
		}

		memset(s_QueueSendSpat, 0x00, sizeof(tQueue_RevLst));
		sem_init(&(s_QueueSendSpat->m_blank_number), 0, NUM);
		sem_init(&(s_QueueSendSpat->m_product_number), 0, 0);
		pthread_mutex_init(s_QueueSendSpat, NULL);
	}

	MessageFrame_t *pSpatMsg;
	pSpatMsg = ALLOC_OBJ(MessageFrame_t, 1);

	static int Cnt = 0;

	pSpatMsg->present = MessageFrame_PR_spatFrame;

	Cnt++;
	if (Cnt > 127)
		Cnt = 0;
	// Update MsgCnt
	pSpatMsg->choice.spatFrame.msgCnt = Cnt;

	int res = 0;

	int i, j;
	for (i = 0; i < p_rectSpatInfo.IntersectionCnt; i++)
	{
		//信号灯路口列表
		IntersectionState_t *IntersectionState;
		IntersectionState = calloc(1, sizeof(IntersectionState_t));
		assert(IntersectionState);

		IntersectionState->intersectionId.id = p_rectSpatInfo.pIntersectionInfo[i].ID;
		IntersectionState->status.buf = calloc(1, sizeof(char) * 2);

		IntersectionState->status.buf[0] = p_rectSpatInfo.pIntersectionInfo[i].Status
				& 0x00ff;
		IntersectionState->status.buf[1] = p_rectSpatInfo.pIntersectionInfo[i].Status
				>> 8;
		IntersectionState->status.size = 2;
		IntersectionState->status.bits_unused = 0;

		for (j = 0; j < p_rectSpatInfo.pIntersectionInfo[i].PhaseCnt; j++)
		{

			//相位列表
			Phase_t *Phase;
			Phase = calloc(1, sizeof(Phase_t));

			Phase->id = p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].ID;

			//相位状态列表
			PhaseState_t *PhaseState;

			//绿灯状态
			PhaseState = calloc(1, sizeof(PhaseState_t));
			PhaseState->light = LightState_permissive_green;
			PhaseState->timing = calloc(1, sizeof(struct TimeChangeDetails));
			PhaseState->timing->present = TimeChangeDetails_PR_counting;
			PhaseState->timing->choice.counting.nextStartTime = calloc(1, sizeof(TimeMark_t));
			PhaseState->timing->choice.counting.nextDuration = calloc(1, sizeof(TimeMark_t));

			*PhaseState->timing->choice.counting.nextDuration = p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime;

			if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
					== GreenLight)
			{
				PhaseState->timing->choice.counting.startTime = 0;
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
				*PhaseState->timing->choice.counting.nextStartTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime;
			}
			else
			{
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime;

				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
						== YellowLight)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}
				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
						== RedLight)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}

			}
			res = ASN_SEQUENCE_ADD(&(*Phase).phaseStates, PhaseState);
			assert(res == 0);

			//黄灯状态
			PhaseState = calloc(1, sizeof(PhaseState_t));
			PhaseState->light = LightState_yellow;
			PhaseState->timing = calloc(1, sizeof(struct TimeChangeDetails));
			PhaseState->timing->present = TimeChangeDetails_PR_counting;
			PhaseState->timing->choice.counting.nextStartTime = calloc(1, sizeof(TimeMark_t));
			PhaseState->timing->choice.counting.nextDuration = calloc(1, sizeof(TimeMark_t));
			*PhaseState->timing->choice.counting.nextDuration = p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime;
			if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
					== YellowLight)
			{
				PhaseState->timing->choice.counting.startTime = 0;
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
				*PhaseState->timing->choice.counting.nextStartTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime;
			}
			else
			{
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime;
				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
						== GreenLight)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}
				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status
						== RedLight)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}
			}
			res = ASN_SEQUENCE_ADD(&(*Phase).phaseStates, PhaseState);
			assert(res == 0);

			//红灯状态
			PhaseState = calloc(1, sizeof(PhaseState_t));
			PhaseState->light = LightState_red;
			PhaseState->timing = calloc(1, sizeof(struct TimeChangeDetails));
			PhaseState->timing->present = TimeChangeDetails_PR_counting;
			PhaseState->timing->choice.counting.nextStartTime = calloc(1, sizeof(TimeMark_t));
			PhaseState->timing->choice.counting.nextDuration = calloc(1, sizeof(TimeMark_t));
			*PhaseState->timing->choice.counting.nextDuration = p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime;
			if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status == 3)
			{
				PhaseState->timing->choice.counting.startTime = 0;
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
				*PhaseState->timing->choice.counting.nextStartTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime
								+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime;
			}
			else
			{
				PhaseState->timing->choice.counting.likelyEndTime =
						p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime;
				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status == 1)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}
				if (p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status == 2)
				{
					PhaseState->timing->choice.counting.startTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft;
					*PhaseState->timing->choice.counting.nextStartTime =
							p_rectSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft
									+ p_rectSpatInfo.pIntersectionInfo[i].Cycle;
				}
			}
			res = ASN_SEQUENCE_ADD(&(*Phase).phaseStates, PhaseState);
			assert(res == 0);

			res = ASN_SEQUENCE_ADD(&(*IntersectionState).phases, Phase);
			assert(res == 0);
		}

		res = ASN_SEQUENCE_ADD(&pSpatMsg->choice.spatFrame.intersections,
				IntersectionState);
		assert(res == 0);
	}

	pthread_mutex_lock(&s_mutexSpatRecvMsg);

	//等到空队列信号量
	sem_wait(&(s_QueueSendSpat->m_blank_number));
	//复制数据到IN数据buff
	if (MessageFrame_Memcpy1(pSpatMsg,
			s_QueueSendSpat->m_stcqueue[s_QueueSendSpat->m_nQIndx_In].macBuff,
			&(s_QueueSendSpat->m_stcqueue[s_QueueSendSpat->m_nQIndx_In].mnBuffLen)) == 0)
	{
		s_QueueSendSpat->m_nQIndx_In ++ ;
		s_QueueSendSpat->m_nQIndx_In %= NUM;
		if(s_QueueSendSpat->m_nQIndx_In == s_QueueSendSpat->m_nQIndx_Out)
		{
			s_QueueSendSpat->m_nQIndx_Out ++;
			s_QueueSendSpat->m_nQIndx_Out %= NUM;
		}
		sem_post(&(s_QueueSendSpat->m_product_number));
	}

	pthread_mutex_unlock(&s_mutexSpatRecvMsg);

//	xer_fprint(stdout, &asn_DEF_MessageFrame, pSpatMsg);
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSpatMsg);

	return 0;
}



