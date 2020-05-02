/*
 * RsmTx.c
 *
 *  Created on: Apr 13, 2018
 *      Author: root
 */
#include "RsmTx.h"
static int FS_RsmTxCreateMsg1(HL_RoadsideSafetyMessage_t *p_prectHLRoadsideSafetyMessage, unsigned int p_unNum, rectMSGLaserToV2X_t p_rectMsgRecvFromLaser_t);
static int FS_CalcTime(uint32_t p_unSeconds,HL_Time_t *p_prectHlTime);
//HL_Time_t hl_time;
int FG_RsmTxOpen(struct RsmTx **p_pprectRsm, pthread_attr_t *pAttr)
{
	int Res = Send_ERR_NONE;
	struct RsmTx *pRsm;

	// Create and check the local structure
	pRsm = (struct RsmTx *) malloc(sizeof(struct RsmTx));

	if (pRsm == NULL)
	{
		Res = MSG_ERR_MALLOC;
		goto Error;
	}
	memset(pRsm, 0, sizeof(struct RsmTx));

	pRsm->ThreadAttr = *pAttr;

	pthread_mutex_init(&mutex_TxRSM, NULL);

	// Create thread
	pRsm->ThreadState |= RSMTX_THREAD_STATE_INIT;
	Res = pthread_create(&pRsm->ThreadID, &pRsm->ThreadAttr,
			(void *) FG_RsmTxThreadProc, pRsm);

	if (Res)
	{
		pRsm->ThreadState = RSMTX_THREAD_STATE_NONE;
		Res = Send_ERR_THREAD_FAIL;
		goto Error;
	}

	//Success
	*p_pprectRsm = pRsm;
	Res = 0;
	goto Success;

	Error: *p_pprectRsm = NULL;
	FG_RsmTxclose(pRsm);
	Success: return Res;

}

void FG_RsmTxclose(struct RsmTx *p_prectRsm)
{

	// Signal thread to terminate
	p_prectRsm->ThreadState |= RSMTX_THREAD_STATE_STOP;
	// Wait for thread termination
	if (p_prectRsm->ThreadState & RSMTX_THREAD_STATE_INIT)
		pthread_join(p_prectRsm->ThreadID, NULL);
	free(p_prectRsm);
	return;
}

void FG_RsmTxThreadProc(void *p_pvArg)
{

	struct RsmTx *pRsm;
	pRsm = (struct RsmTx *) p_pvArg;
	struct timeval delay;
	memset(&RsmFrame, 0x00, sizeof(tRsmData));
	// Starting RsmTx Periodic Thread
	pRsm->ThreadState |= RSMTX_THREAD_STATE_RUN;

	//Thread loop
	while ((pRsm->ThreadState & RSMTX_THREAD_STATE_STOP) == 0)
	{
		if (Store.HostComInfo.RsmTx.TxEnable)
		{

			if (RsmFrame.In != RsmFrame.Out)
			{
				pthread_mutex_lock(&mutex_TxRSM);
				int Res = FG_RsmTxSendMsg(pRsm);
				pthread_mutex_unlock(&mutex_TxRSM);

				if (Res == 0)
				{
					pRsm->Stats.Tx.Okay++;
				}
				else
				{
					pRsm->Stats.Tx.Fail++;
					printf("The RSMTx Fail Code is = %d\n", Res);
				}

				RsmFrame.Out++;
				RsmFrame.Out %= SHARE_MAXNUM;
//				printf("Rsm_Tx Count = %d\n", pRsm->Stats.Tx.Okay);
			}
			else
				msTimeDelay(delay, 1);
		}
		else
		{
			msTimeDelay(delay, 1);
		}

	}

	(void) pthread_exit(NULL);
}
/******************************************************************
* 函数名称: FS_EncodePositionOffsetLL_t
* 函数功能: PositionOffsetLL_t协议层结构体赋值
* 函数参数：PositionOffsetLL_t类型结构体指针和HL_PositionOffsetLL_t类型指针
* 参数名称：pstruPositionOffsetLL和pstruHLPositionOffsetLL
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodePositionOffsetLL_t(PositionOffsetLL_t *pstruPositionOffsetLL, HL_PositionOffsetLL_t *pstruHLPositionOffsetLL)
{
    if ((NULL == pstruPositionOffsetLL) || (NULL == pstruHLPositionOffsetLL))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruPositionOffsetLL->present = pstruHLPositionOffsetLL->HL_enumpresent;
    if (PositionOffsetLL_PR_position_LL1 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL1.lon = pstruHLPositionOffsetLL->choice.HL_position_LL1.HL_lon;
        pstruPositionOffsetLL->choice.position_LL1.lat = pstruHLPositionOffsetLL->choice.HL_position_LL1.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LL2 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL2.lon = pstruHLPositionOffsetLL->choice.HL_position_LL2.HL_lon;
        pstruPositionOffsetLL->choice.position_LL2.lat = pstruHLPositionOffsetLL->choice.HL_position_LL2.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LL3 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL3.lon = pstruHLPositionOffsetLL->choice.HL_position_LL3.HL_lon;
        pstruPositionOffsetLL->choice.position_LL3.lat = pstruHLPositionOffsetLL->choice.HL_position_LL3.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LL4 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL4.lon = pstruHLPositionOffsetLL->choice.HL_position_LL4.HL_lon;
        pstruPositionOffsetLL->choice.position_LL4.lat = pstruHLPositionOffsetLL->choice.HL_position_LL4.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LL5 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL5.lon = pstruHLPositionOffsetLL->choice.HL_position_LL5.HL_lon;
        pstruPositionOffsetLL->choice.position_LL5.lat = pstruHLPositionOffsetLL->choice.HL_position_LL5.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LL6 == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LL6.lon = pstruHLPositionOffsetLL->choice.HL_position_LL6.HL_lon;
        pstruPositionOffsetLL->choice.position_LL6.lat = pstruHLPositionOffsetLL->choice.HL_position_LL6.HL_lat;
    }
    else if (PositionOffsetLL_PR_position_LatLon == pstruPositionOffsetLL->present)
    {
        pstruPositionOffsetLL->choice.position_LatLon.lon = pstruHLPositionOffsetLL->choice.HL_position_LatLon.HL_lon;
        pstruPositionOffsetLL->choice.position_LatLon.lat = pstruHLPositionOffsetLL->choice.HL_position_LatLon.HL_lat;
    }
    else
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeVerticalOffset_t
* 函数功能: VerticalOffset_t协议层结构体赋值
* 函数参数：VerticalOffset_t类型结构体指针和HL_VerticalOffset_t类型指针
* 参数名称：pstruVerticalOffset和pstruHLVerticalOffset
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeVerticalOffset_t(VerticalOffset_t *pstruVerticalOffset, HL_VerticalOffset_t *pstruHLVerticalOffset)
{
    if ((NULL == pstruVerticalOffset) || (NULL == pstruHLVerticalOffset))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruVerticalOffset->present = pstruHLVerticalOffset->HL_enumVerticalOffset_PRpresent;
    if (VerticalOffset_PR_offset1 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset1 = pstruHLVerticalOffset->choice.HL_offset1;
    }
    else if (VerticalOffset_PR_offset2 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset2 = pstruHLVerticalOffset->choice.HL_offset2;
    }
    else if (VerticalOffset_PR_offset3 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset3 = pstruHLVerticalOffset->choice.HL_offset3;
    }
    else if (VerticalOffset_PR_offset4 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset4 = pstruHLVerticalOffset->choice.HL_offset4;
    }
    else if (VerticalOffset_PR_offset5 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset5 = pstruHLVerticalOffset->choice.HL_offset5;
    }
    else if (VerticalOffset_PR_offset6 == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.offset6 = pstruHLVerticalOffset->choice.HL_offset6;
    }
    else if (VerticalOffset_PR_elevation == pstruVerticalOffset->present)
    {
        pstruVerticalOffset->choice.elevation = pstruHLVerticalOffset->choice.HL_elevation;
    }
    else
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodePositionOffsetLLV_t
* 函数功能: PositionOffsetLLV_t协议层结构体赋值
* 函数参数：PositionOffsetLLV_t类型结构体指针和HL_PositionOffsetLLV_t类型指针
* 参数名称：pstruPositionOffsetLLV和pstruHLPositionOffsetLLV
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodePositionOffsetLLV_t(PositionOffsetLLV_t *pstruPositionOffsetLLV, HL_PositionOffsetLLV_t *pstruHLPositionOffsetLLV)
{
    if ((NULL == pstruPositionOffsetLLV) || (NULL == pstruHLPositionOffsetLLV))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    int ret;
    ret = FS_EncodePositionOffsetLL_t(&(pstruPositionOffsetLLV->offsetLL), &(pstruHLPositionOffsetLLV->HL_offsetLL));
    if (is_err == ret)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    if (HL_PRESENT == pstruHLPositionOffsetLLV->HL_offsetVpresent)
    {
        pstruPositionOffsetLLV->offsetV = CALLOC(1, sizeof *(pstruPositionOffsetLLV->offsetV));
        ret = FS_EncodeVerticalOffset_t(pstruPositionOffsetLLV->offsetV, &(pstruHLPositionOffsetLLV->HL_offsetV));
        if (is_err == ret)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
    }
    return is_ok;
}

/******************************************************************
* 函数名称: FS_EncodePositionConfidenceSet_t
* 函数功能: PositionConfidenceSet_t协议层结构体赋值
* 函数参数：PositionConfidenceSet_t类型结构体指针和HL_PositionConfidenceSet_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodePositionConfidenceSet_t(PositionConfidenceSet_t *pstruMsg, HL_PositionConfidenceSet_t *pstruHlMsg)
{
    if ((NULL == pstruMsg) || (NULL == pstruHlMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruMsg->pos = pstruHlMsg->HL_pos;
    if (HL_PRESENT == pstruHlMsg->HL_elevationPresent)
    {
        pstruMsg->elevation = ALLOC_OBJ(ElevationConfidence_t, 1);
        if (NULL == pstruMsg->elevation)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->elevation) = pstruHlMsg->HL_elevation;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeMotionConfidenceSet_t
* 函数功能: MotionConfidenceSet_t协议层结构体赋值
* 函数参数：MotionConfidenceSet_t类型结构体指针和HL_MotionConfidenceSet_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeMotionConfidenceSet_t(MotionConfidenceSet_t *pstruMsg, HL_MotionConfidenceSet_t *pstruHlMsg)
{
    if ((NULL == pstruMsg) || (NULL == pstruHlMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    if (HL_PRESENT == pstruHlMsg->HL_speedCfdPresent)
    {
        pstruMsg->speedCfd = ALLOC_OBJ(SpeedConfidence_t, 1);
        if (NULL == pstruMsg->speedCfd)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->speedCfd) = pstruHlMsg->HL_speedCfd;
    }
    if (HL_PRESENT == pstruHlMsg->HL_headingCfdPresent)
    {
        pstruMsg->headingCfd = ALLOC_OBJ(HeadingConfidence_t, 1);
        if (NULL == pstruMsg->headingCfd)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->headingCfd) = pstruHlMsg->HL_headingCfd;
    }
    if (HL_PRESENT == pstruHlMsg->HL_steerCfdPresent)
    {
        pstruMsg->steerCfd = ALLOC_OBJ(SteeringWheelAngleConfidence_t, 1);
        if (NULL == pstruMsg->steerCfd)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->steerCfd) = pstruHlMsg->HL_steerCfd;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeAccelerationSet4Way_t
* 函数功能: AccelerationSet4Way_t协议层结构体赋值
* 函数参数：AccelerationSet4Way_t类型结构体指针和HL_AccelerationSet4Way_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeAccelerationSet4Way_t(AccelerationSet4Way_t *pstruMsg, HL_AccelerationSet4Way_t *pstruHlMsg)
{
    if ((NULL == pstruMsg) || (NULL == pstruHlMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruMsg->Long = pstruHlMsg->HL_Long;
    pstruMsg->lat = pstruHlMsg->HL_lat;
    pstruMsg->vert = pstruHlMsg->HL_vert;
    pstruMsg->yaw = pstruHlMsg->HL_yaw;
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeVehicleSize_t
* 函数功能: VehicleSize_t协议层结构体赋值
* 函数参数：VehicleSize_t类型结构体指针和HL_VehicleSize_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeVehicleSize_t(VehicleSize_t *pstruMsg, HL_VehicleSize_t *pstruHlMsg)
{
    if ((NULL == pstruMsg) || (NULL == pstruHlMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruMsg->width = pstruHlMsg->HL_width;
    pstruMsg->length = pstruHlMsg->HL_length;
    if (HL_PRESENT == pstruHlMsg->HL_heightPresent)
    {
        pstruMsg->height = ALLOC_OBJ(VehicleHeight_t, 1);
        if (NULL == pstruMsg->height)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->height) = pstruHlMsg->HL_height;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeVehicleClassification_t
* 函数功能: VehicleClassification_t协议层结构体赋值
* 函数参数：VehicleClassification_t类型结构体指针和HL_VehicleClassification_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeVehicleClassification_t(VehicleClassification_t *pstrMsg, HL_VehicleClassification_t *pstruHlMsg)
{
    if ((NULL == pstrMsg) || (NULL == pstruHlMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstrMsg->classification = pstruHlMsg->HL_classification;
    return is_ok;
}

/******************************************************************
* 函数名称: FS_EncodeParticipantData_t
* 函数功能: ParticipantData_t协议层结构体赋值
* 函数参数：ParticipantData_t类型结构体指针和HL_ParticipantData_t类型指针
* 参数名称：pstruMsg和pstruHlmsg
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeParticipantData_t(ParticipantData_t *pstruMsg, HL_ParticipantData_t *pstruHlmsg)
{
    if ((NULL == pstruMsg) || (NULL == pstruHlmsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    char *pu8Node;
    int u32i,u32Ret;
    pstruMsg->ptcType = pstruHlmsg->HL_ptcType;
    pstruMsg->ptcId = pstruHlmsg->HL_ptcId;
    pstruMsg->source = pstruHlmsg->HL_source;
    if (HL_PRESENT == pstruHlmsg->HL_idPresent)
    {
        pstruMsg->id = ALLOC_OBJ(OCTET_STRING_t, 1);
        if (NULL == pstruMsg->id)
        {
            printf("--%s %s file %s line:%d\n",
                   __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        pstruMsg->id->buf = ALLOC_OBJ(char, pstruHlmsg->HL_id.HL_size);
        if (NULL == pstruMsg->id->buf)
        {
            printf("--%s %s file %s line:%d\n",
                   __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        pu8Node = pstruMsg->id->buf;
        for (u32i = 0; u32i < pstruHlmsg->HL_id.HL_size; u32i++)
        {
            *pu8Node = pstruHlmsg->HL_id.HL_buf[u32i];
            pu8Node++;
        }
    }
    pstruMsg->secMark = pstruHlmsg->HL_secMark;
    u32Ret = FS_EncodePositionOffsetLLV_t(&(pstruMsg->pos), &(pstruHlmsg->HL_pos));
    if (is_err == u32Ret)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    u32Ret = FS_EncodePositionConfidenceSet_t(&(pstruMsg->posConfidence), &(pstruHlmsg->HL_accuracy));
    if (is_err == u32Ret)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    if (HL_PRESENT == pstruHlmsg->HL_transmissionPresent)
    {
        pstruMsg->transmission = ALLOC_OBJ(TransmissionState_t, 1);
        if (NULL == pstruMsg->transmission)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->transmission) = pstruHlmsg->HL_transmission;
    }
    pstruMsg->speed = pstruHlmsg->HL_speed;
    pstruMsg->heading = pstruHlmsg->HL_heading;
    if (HL_PRESENT == pstruHlmsg->HL_anglePresent)
    {
        pstruMsg->angle = ALLOC_OBJ(TransmissionState_t, 1);
        if (NULL == pstruMsg->angle)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruMsg->angle) = pstruHlmsg->HL_angle;
    }
    if (HL_PRESENT == pstruHlmsg->HL_motionCfdPresent)
    {
        pstruMsg->motionCfd = ALLOC_OBJ(MotionConfidenceSet_t, 1);
        if (NULL == pstruMsg->motionCfd)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        u32Ret = FS_EncodeMotionConfidenceSet_t(pstruMsg->motionCfd,
                        &(pstruHlmsg->HL_motionCfd));
        if (is_err == u32Ret)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
    }
    if (HL_PRESENT == pstruHlmsg->HL_accelSetPresent)
    {
        pstruMsg->accelSet = ALLOC_OBJ(AccelerationSet4Way_t, 1);
        if (NULL == pstruMsg->accelSet)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        u32Ret = FS_EncodeAccelerationSet4Way_t(pstruMsg->accelSet, &(pstruHlmsg->HL_accelSet));
        if (is_err == u32Ret)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
    }
    u32Ret = FS_EncodeVehicleSize_t(&(pstruMsg->size), &(pstruHlmsg->HL_size));
    if (is_err == u32Ret)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    if (HL_PRESENT == pstruHlmsg->HL_vehicleClassPresent)
    {
        pstruMsg->vehicleClass = ALLOC_OBJ(VehicleClassification_t, 1);
        if (NULL == pstruMsg->vehicleClass)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        u32Ret = FS_EncodeVehicleClassification_t(pstruMsg->vehicleClass, &(pstruHlmsg->HL_vehicleClass));
        if (is_err == u32Ret)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
    }

    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodePosition3D_t
* 函数功能: Position3D_t协议层结构体赋值
* 函数参数：Position3D_t类型结构体指针和HL_Position3D_t类型指针
* 参数名称：pstruPosition3D和pstruHLPosition3D
* 返 回 值：函数运行状态
* 函数说明: 函数实现HL结构体和协议口之间的赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodePosition3D_t(Position3D_t *pstruPosition3D, HL_Position3D_t *pstruHLPosition3D)
{
    if ((NULL == pstruPosition3D) || (NULL == pstruHLPosition3D))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pstruPosition3D->lat = pstruHLPosition3D->HL_lat;
    pstruPosition3D->Long = pstruHLPosition3D->HL_Long;
    if (HL_PRESENT == pstruHLPosition3D->HL_elevation_present)
    {
        pstruPosition3D->elevation = CALLOC(1, sizeof *(pstruPosition3D->elevation));
        if (NULL == pstruPosition3D->elevation)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        *(pstruPosition3D->elevation) = pstruHLPosition3D->HL_elevation;
    }
    return is_ok;
}
/******************************************************************
* 函数名称: FS_EncodeRoadsideSafetyMessage_t
* 函数功能: RoadsideSafetyMessage_t协议层结构体赋值
* 函数参数：RoadsideSafetyMessage_t类型结构体指针和HL_RoadsideSafetyMessage_t
* 参数名称：pstruRsmMsg和pstruHlRsmMsg
* 返 回 值：函数运行状态
* 函数说明: 函数主要实现协议结构体和hl结构体赋值
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      创建函数
******************************************************************/
static int FS_EncodeRoadsideSafetyMessage_t(RoadsideSafetyMessage_t *pstruRsmMsg, HL_RoadsideSafetyMessage_t *pstruHlRsmMsg)
{
    if ((NULL == pstruRsmMsg) || (NULL == pstruHlRsmMsg))
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    char *pu8Node;
    int u32i,u32Ret;
    ParticipantData_t *pstruParticipantDataNode;
    pstruRsmMsg->msgCnt = pstruHlRsmMsg->HL_msgCnt;
    pstruRsmMsg->id.buf = ALLOC_OBJ(char, pstruHlRsmMsg->HL_id.HL_size);
    if (NULL == pstruRsmMsg->id.buf)
    {
        printf("--%s %s file %s line:%d\n",
               __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    pu8Node = pstruRsmMsg->id.buf;
    for (u32i = 0; u32i < pstruHlRsmMsg->HL_id.HL_size; u32i++)
    {
        *pu8Node = pstruHlRsmMsg->HL_id.HL_buf[u32i];
        pu8Node++;
    }
    pstruRsmMsg->id.size = pstruHlRsmMsg->HL_id.HL_size;
    u32Ret = FS_EncodePosition3D_t(&(pstruRsmMsg->refPos), &(pstruHlRsmMsg->HL_refPos));
    if (is_err == u32Ret)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    for (u32i = 0; u32i < pstruHlRsmMsg->HL_participants.HL_participantsNum; u32i++)
    {
    	pstruParticipantDataNode = ALLOC_OBJ(ParticipantData_t, 1);
        u32Ret = FS_EncodeParticipantData_t(pstruParticipantDataNode, &(pstruHlRsmMsg->HL_participants.HL_participantsList[u32i]));
        if (is_err == u32Ret)
        {
            printf("--%s %s file %s line:%d\n",
                   __DATE__,__TIME__,__FILE__,__LINE__);
            return is_err;
        }
        ASN_SEQUENCE_ADD(&(pstruRsmMsg->participants), pstruParticipantDataNode);
    }

    return is_ok;
}
/******************************************************************
* 函数名称: RsmTx_SendMsg
* 函数功能: 发送RSM消息
* 函数参数：tRsmTx类型结构体指针
* 参数名称：pstruRsm
* 返 回 值：函数运行状态
* 函数说明: 函数主要填充RSM消息结构体，调用接口发送消息
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛      修改函数
* 2020/01/13   v1.1     柴志军    修改函数
* 修改内容：局部激光雷达结构体变量取代原全局变量，并通过FG_RecvFromLaserMsgGet借口获取变量数据
* 将原来操作激光雷达结构体变量的互斥变量搬到RsmConfig接口中
******************************************************************/
int FG_RsmTxSendMsg(struct RsmTx *p_prectRsm)
{
    if (NULL == p_prectRsm)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    int u32Res = Send_ERR_NONE;
    int u32BufLen = 2028;
    char u8Buf[u32BufLen];
    uint8_t u8Cout;
    uint32_t u32i;
    asn_enc_rval_t struEncRetVal;
    MessageFrame_t *pstruSendMsg;
    HL_RoadsideSafetyMessage_t struHlRoadsideSafetyMessage = {0};

    //added by chaizhijun 20200305
    rectMSGLaserToV2X_t *l_rectMsgRecvFromLaser = calloc(1, (sizeof(rectMSGLaserToV2X_t)));
    if(NULL == l_rectMsgRecvFromLaser)
	{
		printf("--%s %s file %s line:%d\n",
			__DATE__,__TIME__,__FILE__,__LINE__);
		return is_err;
	}
    FG_RecvFromLaserMsgGet(l_rectMsgRecvFromLaser);
    //added end

    if (100 < l_rectMsgRecvFromLaser->m_ucParticipantsNum)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    u8Cout = (l_rectMsgRecvFromLaser->m_ucParticipantsNum + 15) / 16;
    for (u32i = 1; u32i <= u8Cout; u32i++)
    {
        /* 填充消息结构体 */
        u32Res = FS_RsmTxCreateMsg1(&struHlRoadsideSafetyMessage, u32i, *l_rectMsgRecvFromLaser);
        if (is_err == u32Res)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return Send_ERR_CREATE_FAIL;
        }
        pstruSendMsg = ALLOC_OBJ(MessageFrame_t, 1);
        if (NULL == pstruSendMsg)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            return MSG_ERR_MALLOC;
        }
        pstruSendMsg->present = MessageFrame_PR_rsmFrame;
        /* 编码赋值 */
        u32Res = FS_EncodeRoadsideSafetyMessage_t(&(pstruSendMsg->choice.rsmFrame), &struHlRoadsideSafetyMessage);
        if (is_err == u32Res)
        {
            printf("--%s %s file %s line:%d\n",
                __DATE__,__TIME__,__FILE__,__LINE__);
            ASN_STRUCT_FREE(asn_DEF_MessageFrame, pstruSendMsg);
            return Send_ERR_CREATE_FAIL;
        }
        p_prectRsm->Params = *pstruSendMsg;
        struEncRetVal = uper_encode_to_buffer(&asn_DEF_MessageFrame,NULL,
                             pstruSendMsg, u8Buf, 2000);

//		xer_fprint(stdout, &asn_DEF_MessageFrame, pstruSendMsg);
		if (NULL != struEncRetVal.failed_type)
		{
			printf("The failed name is: %s\n", struEncRetVal.failed_type->name);
	        ASN_STRUCT_FREE(asn_DEF_MessageFrame, pstruSendMsg);
			printf("--%s %s file %s line:%d\n",
					  __DATE__,__TIME__,__FILE__,__LINE__);
			return is_err;
		}
		else
	    {
	        ASN_STRUCT_FREE(asn_DEF_MessageFrame, pstruSendMsg);
	        u32BufLen = ((struEncRetVal.encoded + 7) / 8);

	        tDSM_Request dsmSend_rq;
			dsmSend_rq.Length = u32BufLen;
			dsmSend_rq.Data = (nint8_t *) u8Buf;
			DSMSend(dsmSend_rq);
	       // DSMSend(u8Buf, u32BufLen);
	    }
    }
    //added by chaizhijun 20200305
    free(l_rectMsgRecvFromLaser);
    //added end
    return is_ok;
}

int FG_RsmTxReadlstTxMsg(MessageFrame_t *p_prectRsmMsg)
{
	pthread_mutex_lock(&mutex_StoreRSM);
	ReadXml(&p_prectRsmMsg, "./TxLog/RSM_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreRSM);
	return p_prectRsmMsg->present;
}
int FG_RsmTxCreateMsg(tRsmMsg p_rectRsmMsg)
{
	int RetVal = MSG_ERR_NONE;
	MessageFrame_t *pRsmMsg;
	pRsmMsg = ALLOC_OBJ(MessageFrame_t, 1);

	static int Cnt = 0;

	pRsmMsg->present = MessageFrame_PR_rsmFrame;

	Cnt++;
	if (Cnt > 127)
		Cnt = 0;
	// Update MsgCnt
	pRsmMsg->choice.rsmFrame.msgCnt = Cnt;

	// Vehicle ID
	pRsmMsg->choice.rsmFrame.id.buf = ALLOC_OBJ(uint8_t, 8);
	if (!pRsmMsg->choice.rsmFrame.id.buf)
	  {
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	  }
	pRsmMsg->choice.rsmFrame.id.size = 8;
	memcpy(pRsmMsg->choice.rsmFrame.id.buf, &Store.HostAttriInfo.ID[0], sizeof(uint8_t) * 8);

	//Get GPS Information
	GpsLocation *pGpsData;
	pGpsData = ALLOC_OBJ(GpsLocation, 1);
	RetVal = GPS_GetData(pGpsData);
	if (RetVal)
	{
		RetVal = MSG_ERR_INTERFACE;
		goto Error;
	}

	//refPostion
	pRsmMsg->choice.rsmFrame.refPos.elevation = ALLOC_OBJ(Elevation_t, 1);
	if (!pRsmMsg->choice.rsmFrame.refPos.elevation)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	pRsmMsg->choice.rsmFrame.refPos.lat = pGpsData->latitude;
	pRsmMsg->choice.rsmFrame.refPos.Long = pGpsData->longitude;
	//add by wll 2019-03-26 15:09:53 兼容激光雷达和路侧设备位置不一致的情况
	if((p_rectRsmMsg.RefPosLat != 0)||(p_rectRsmMsg.RefPosLng != 0))
	{
		pRsmMsg->choice.rsmFrame.refPos.lat = p_rectRsmMsg.RefPosLat;
		pRsmMsg->choice.rsmFrame.refPos.Long = p_rectRsmMsg.RefPosLng;
	}

	*(pRsmMsg->choice.rsmFrame.refPos.elevation) = 0;
	int i = 0;
	for (; i < p_rectRsmMsg.ParticipantCnt; i++)
	{
		ParticipantData_t *pParticipantData;

		pParticipantData = calloc(1, sizeof(ParticipantData_t));

		pParticipantData->ptcType = p_rectRsmMsg.pParticipantInfo[i].ParticType;
		pParticipantData->ptcId = p_rectRsmMsg.pParticipantInfo[i].ID;
		pParticipantData->source = p_rectRsmMsg.pParticipantInfo[i].SourecType;

		pParticipantData->secMark = GetDSecond();

		pParticipantData->pos.offsetLL.present =
				PositionOffsetLL_PR_position_LatLon;
		pParticipantData->pos.offsetLL.choice.position_LatLon.lat =
				(long) (p_rectRsmMsg.pParticipantInfo[i].Latitude * 10000000);
		pParticipantData->pos.offsetLL.choice.position_LatLon.lon =
				(long) (p_rectRsmMsg.pParticipantInfo[i].Longitude * 10000000);

		pParticipantData->pos.offsetV = calloc(1, sizeof(VerticalOffset_t));
		pParticipantData->pos.offsetV->present = VerticalOffset_PR_elevation;
		pParticipantData->pos.offsetV->choice.elevation = 10
				* p_rectRsmMsg.pParticipantInfo[i].Altitude;

		pParticipantData->speed = 50 * p_rectRsmMsg.pParticipantInfo[i].Speed;
		pParticipantData->heading = 80 * p_rectRsmMsg.pParticipantInfo[i].Heading;

		pParticipantData->size.length = p_rectRsmMsg.pParticipantInfo[i].ObjLength
				* 100;
		pParticipantData->size.width = p_rectRsmMsg.pParticipantInfo[i].ObjWidth
				* 100;

		pParticipantData->size.height = calloc(1, sizeof(VehicleHeight_t));
		*pParticipantData->size.height = p_rectRsmMsg.pParticipantInfo[i].ObjHeight
				* 20;

		ASN_SEQUENCE_ADD(&pRsmMsg->choice.rsmFrame.participants, pParticipantData);
		//if(pParticipantData != NULL) free(pParticipantData);pParticipantData = NULL;
	}

//	memcpy(&RsmFrame.MsgData.present, pRsmMsg, sizeof(MessageFrame_t));

	pthread_mutex_lock(&mutex_TxRSM);
	//memset(&(RsmFrame.MsgData), 0x00, sizeof(MessageFrame_t));
	//if(MessageFrame_Memcpy(pRsmMsg, &(RsmFrame.MsgData)) == 0)

	if(MessageFrame_Memcpy1(pRsmMsg, RsmFrame.pBuf, &(RsmFrame.BufLen))== 0)
	{
		RsmFrame.In++;
		RsmFrame.In %= SHARE_MAXNUM;
	}

	pthread_mutex_unlock(&mutex_TxRSM);
	Error:
//		free(pRsmMsg);
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRsmMsg);
	return RetVal;
}
/******************************************************************
* 函数名称: FS_RsmTxCreateMsg1
* 函数功能: 填充消息RSM消息结构体
* 函数参数：HL_RoadsideSafetyMessage_t类型结构体
* 参数名称：p_prectHLRoadsideSafetyMessage
* 返 回 值：函数运行状态
* 函数说明: 函数主要填充RSM消息结构体
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/13   v1.0     黄赛        创建函数
* 2020/01/13   v1.1     柴志军       修改函数
* 修改内容：增加传入参数p_rectMsgRecvFromLaser_t，避免原来雷达结构体重复操作
******************************************************************/

static int FS_RsmTxCreateMsg1(HL_RoadsideSafetyMessage_t *p_prectHLRoadsideSafetyMessage, unsigned int p_unNum, rectMSGLaserToV2X_t p_rectMsgRecvFromLaser_t)
{
    /* 入参检查 */
    if (NULL == p_prectHLRoadsideSafetyMessage)
    {
        printf("--%s %s file %s line:%d\n",
            __DATE__,__TIME__,__FILE__,__LINE__);
        return is_err;
    }
    static unsigned char u8count;
    unsigned int u32i;
    unsigned int u32Ret;
    unsigned char u8temp;
    HL_Time_t hltime_t={0};

    WayPoint struLaserLoALat, struXyz, struLoALat;
    rectLasetParticipants_t structSub1[16];
    rectGeoDeticParticipants_t structSub2[16];
    rectLaGParticipants_t structSub3[16];

    if (127 > u8count)
    {
        u8count = 0;
    }
    p_prectHLRoadsideSafetyMessage->HL_msgCnt = u8count;
    p_prectHLRoadsideSafetyMessage->HL_id.HL_size = 8;
    memcpy(p_prectHLRoadsideSafetyMessage->HL_id.HL_buf, Store.HostAttriInfo.ID, 8);
    p_prectHLRoadsideSafetyMessage->HL_refPos.HL_lat = p_rectMsgRecvFromLaser_t.m_unLat;
    p_prectHLRoadsideSafetyMessage->HL_refPos.HL_Long = p_rectMsgRecvFromLaser_t.m_unLon;
    p_prectHLRoadsideSafetyMessage->HL_refPos.HL_elevation_present = HL_PRESENT;
    p_prectHLRoadsideSafetyMessage->HL_refPos.HL_elevation = (p_rectMsgRecvFromLaser_t.m_unFrame & 0xffff);
	FS_CalcTime(p_rectMsgRecvFromLaser_t.m_ulTimeSec>>32,&hltime_t);

	if ((p_unNum * 16) > p_rectMsgRecvFromLaser_t.m_ucParticipantsNum)
	{
		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsNum = p_rectMsgRecvFromLaser_t.m_ucParticipantsNum  - (p_unNum - 1) * 16;
	}
	else
	{
		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsNum = 16;
	}

	if (LaserRawData == p_rectMsgRecvFromLaser_t.m_ucEnumSubCmd)
	{
		for (u32i = 0; u32i < p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsNum; u32i++)
		{
			u8temp = u32i + (16 * (p_unNum - 1));
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_ucEnumtTrafficType;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcId = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usId;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_source = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_ucEnumFrom;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_idPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_plateNoPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_secMark = GetDSecond();
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.HL_enumpresent = HL_PositionOffsetLL_PR_position_LatLon;
			/* 利用激光经纬度和交通参与者坐标XYZ转换成交通参与者经纬度 */
	//    		struLaserLoALat.x = pstruMsgRecvFromLaser.u32Lat / 10000000.0;
	//    		struLaserLoALat.y = pstruMsgRecvFromLaser.u32Lon / 10000000.0;
	//    		struXyz.x = pstruMsgRecvFromLaser.choice.sub1[u8temp].u16coordinate_X / 100.0;
	//    		struXyz.y = pstruMsgRecvFromLaser.choice.sub1[u8temp].u16coordinate_Y / 100.0;
	//    		XYZ_To_BLH(struLaserLoALat, struXyz, pstruMsgRecvFromLaser.u16angle - 90, &struLoALat);
	//    		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lon = struLoALat.x;
	//    		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lat = struLoALat.y;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lon = struLoALat.x;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lat = struLoALat.y;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetVpresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.HL_enumVerticalOffset_PRpresent = HL_VerticalOffset_PR_elevation;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.choice.HL_elevation = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usCoordinate_Z / 10.0;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_pos = 9;/* 定义当前精度为1M */
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_elevationPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_transmissionPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_speed = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usSpeed / 2;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_heading = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usDirection * 80;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_anglePresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_motionCfdPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSetPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_length = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usLongth / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_width = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usWidth / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_heightPresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_height = p_rectMsgRecvFromLaser_t.choice.sub1[u8temp].m_usHeight / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_ABSENT;
		}
	}
	else if (GeodeticLatLon == p_rectMsgRecvFromLaser_t.m_ucEnumSubCmd)
	{
		for (u32i = 0; u32i < p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsNum; u32i++)
		{
			u8temp = u32i + (16 * (p_unNum - 1));
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_ucEnumtTrafficType;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcId = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usId;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_source = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_ucEnumFrom;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_idPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_plateNoPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_secMark = GetDSecond();
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.HL_enumpresent = HL_PositionOffsetLL_PR_position_LatLon;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lon = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_unLon / 10000000.0;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lat = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_unLat/ 10000000.0;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetVpresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.HL_enumVerticalOffset_PRpresent = HL_VerticalOffset_PR_elevation;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.choice.HL_elevation = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usAltitude / 10.0;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_pos = 9;/* 定义当前精度为1M */
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_elevationPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_transmissionPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_speed = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usSpeed / 2;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_heading = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usHeadAnagle * 80;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_anglePresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_motionCfdPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSetPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_length = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usLongth / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_width = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usWidth / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_heightPresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_height = p_rectMsgRecvFromLaser_t.choice.sub2[u8temp].m_usHeight / 100;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_ABSENT;
		}
	}
	else if (LaserAndGeodetic == p_rectMsgRecvFromLaser_t.m_ucEnumSubCmd)
	{
		for (u32i = 0; u32i < p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsNum; u32i++)
		{
			u8temp = u32i + (16 * (p_unNum - 1));
	//	        	printf("u8temp = %d u32i=%d u32num=%d\n",u8temp,u32i,u32num);
	//	        	printf("pstruMsgRecvFromLaser.choice.sub3[u8temp].enumtTrafficType = %d\n", pstruMsgRecvFromLaser.choice.sub3[u8temp].enumtTrafficType);
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType;
			/*0--未知
			 * 交通参与者类型--0 && 车辆类型无
			 * */
			if(0==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 0;
			/*1--汽车
			 * 交通参与者类型--1 && 车辆类型--27
			 * */
			else if(1==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
			{
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 1;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_PRESENT;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClass.HL_classification = 27;
			}
			/*2--卡车、货车
			 * 交通参与者类型--1  && 车辆类型--21
			 * */
			else if(2==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
			{
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 1;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_PRESENT;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClass.HL_classification = 21;
			}
			/*3--大巴车
			 * 交通参与者类型--1 && 车辆类型--11
			 * */
			else if(3==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
			{
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 1;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_PRESENT;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClass.HL_classification = 11;
			}
			/*4--行人
			 * 交通参与者类型--3 && 车辆类型无
			 * */
			else if(4==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 3;
			/*5--自行车
			 * 交通参与者类型--2 && 车辆类型无
			 * */
			else if(5==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 2;
			/*6--摩托车/电动车
			 * 交通参与者类型--1 && 车辆类型--40
			 * */
			else if(6==p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumtTrafficType)
			{
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcType = 1;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_PRESENT;
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClass.HL_classification = 40;
			}
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_ptcId = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usId;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_source = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumFrom;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_idPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_plateNoPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_secMark = (((p_rectMsgRecvFromLaser_t.m_ulTimeSec&0x0ffffffff)/4.295)/1000000);
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.HL_enumpresent = HL_PositionOffsetLL_PR_position_LatLon;
			/* 利用激光经纬度和交通参与者坐标XYZ转换成交通参与者经纬度 */
	//    		struLaserLoALat.x = pstruMsgRecvFromLaser.choice.sub3[u8temp].u32Lat / 10000000.0;
	//    		struLaserLoALat.y = pstruMsgRecvFromLaser.choice.sub3[u8temp].u32Lon / 10000000.0;
	//    		struXyz.x = pstruMsgRecvFromLaser.choice.sub3[u8temp].u16coordinate_X / 100.0;
	//    		struXyz.y = pstruMsgRecvFromLaser.choice.sub3[u8temp].u16coordinate_Y / 100.0;
	//    		XYZ_To_BLH(struLaserLoALat, struXyz, pstruMsgRecvFromLaser.u16angle - 90, &struLoALat);
	//    		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lon = struLoALat.x;
	//    		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lat = struLoALat.y;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lon = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_unLat;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetLL.choice.HL_position_LatLon.HL_lat = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_unLon;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetVpresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.HL_enumVerticalOffset_PRpresent = HL_VerticalOffset_PR_elevation;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_pos.HL_offsetV.choice.HL_elevation = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usCoordinate_Z / 10.0;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_pos =  9;//精度1M
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accuracy.HL_elevationPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_transmissionPresent = HL_ABSENT;

			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_speed = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usSpeed;
			if(360 > p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usHeadAnagle)
			{
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_heading = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usHeadAnagle;
			}
			else
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_heading = (p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usHeadAnagle-360);
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_anglePresent = HL_PRESENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_motionCfdPresent = HL_ABSENT;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSetPresent = HL_PRESENT;
			/*四轴加速度（AccelerationSet4Way）中
			 * 纵向加速度（Long）和横向加速度（Lat）分别代表激光雷达产生数据的时间的小时数和分钟数
			 * 横摆角速度（yaw）分别代表激光雷达产生数据的时间的当前分钟下的秒数
			 * */
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSet.HL_Long =hltime_t.Hour;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSet.HL_lat =hltime_t.Min;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSet.HL_yaw= hltime_t.Second;

			/* 四轴加速度（AccelerationSet4Way）中重力加速度（vert）
			 * 用来表示激光雷达数据中颜色信息
			 * 0--未知   1--白色  2--黑色  3--红色  4--银色
			 * 5--黄色   6--蓝色  7--彩色/杂色
			 * */
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_accelSet.HL_vert =p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucEnumMyColor;
			/* 车辆转向轮角度（SteeringWheelAngle ）
			 * 来表示激光雷达数据中置信度信息
			 * */
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_angle = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_ucConfidenceLevel;

			if(4000 > p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usLongth)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_length = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usLongth;
			if(1000 > p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usWidth)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_width =  p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usWidth;
			p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_heightPresent = HL_PRESENT;
			if(32675 > p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usHeight)
				p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_size.HL_height = p_rectMsgRecvFromLaser_t.choice.sub3[u8temp].m_usHeight/5.0;
	//    		p_prectHLRoadsideSafetyMessage->HL_participants.HL_participantsList[u32i].HL_vehicleClassPresent = HL_ABSENT;
		}
	}
	else
	{
		printf("--%s %s file %s line:%d\n",
			__DATE__,__TIME__,__FILE__,__LINE__);
		return is_err;
	}
    return is_ok;
}
/******************************************************************
* 函数名称: FS_CalcTime
* 函数功能: 将从1900年1月1日0分0秒到现在的总秒数计算当前时间
* 函数参数：Second_data 1900年1月1日0分0秒到现在的总秒数
* 返 回 值：当前时间
* 函数说明: 函数主要填充RSM消息结构体
* 修改日期:        版本号       修改人       修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/02/14   v1.0     姜博        创建函数
* 2020/03/10   v1.1     柴志军      修改函数
* 修改原因：原函数执行次数太多，导致运行时间过长
* 修改内容：原函数功能不变，修改函数实现方式，并将该函数修改为局部
******************************************************************/
static int FS_CalcTime(uint32_t p_unSeconds,HL_Time_t *p_prectHlTime)
{
	const char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	unsigned int Pass4year;
	int hours_per_year;
	//取秒时间
	p_prectHlTime->Second=(int)(p_unSeconds % 60);
	p_unSeconds /= 60;
	//取分钟时间
	p_prectHlTime->Min=(int)(p_unSeconds % 60);
	p_unSeconds /= 60;
	//取过去多少个四年，每四年有 1461*24 小时
	Pass4year=((unsigned int)p_unSeconds / (1461L * 24L));
	//计算年份
	p_prectHlTime->Year=(Pass4year << 2) + 1900;
	//四年中剩下的小时数
	p_unSeconds %= 1461L * 24L;
	//校正闰年影响的年份，计算一年中剩下的小时数
	for (;;)
	{
		//一年的小时数
		hours_per_year = 365 * 24;
		//判断闰年
		if ((p_prectHlTime->Year & 3) == 0)
		{
			//是闰年，一年则多24小时，即一天
			hours_per_year += 24;
		}
		if (p_unSeconds < hours_per_year)
		{
			break;
		}
		p_prectHlTime->Year++;
		p_unSeconds -= hours_per_year;
	}
	//小时数
	p_prectHlTime->Hour=(int)(p_unSeconds % 24);
	//一年中剩下的天数
	p_unSeconds /= 24;
	//假定为闰年
	p_unSeconds++;
	//校正闰年的误差，计算月份，日期
	if((p_prectHlTime->Year & 3) == 0)
	{
		if (p_unSeconds > 60)
		{
			p_unSeconds--;
		}
		else
		{
			if (p_unSeconds == 60)
			{
				p_prectHlTime->Month = 1;
				p_prectHlTime->Day = 29;
				return is_ok;
			}
		}
	}
	//计算月日
	for (p_prectHlTime->Month = 0; Days[p_prectHlTime->Month] < p_unSeconds;p_prectHlTime->Month++)
	{
		p_unSeconds -= Days[p_prectHlTime->Month];
	}

	p_prectHlTime->Day = (int)(time);
	return is_ok;
}

#if 0
extern HL_Time_t hl_time;
void Calc_Time(unsigned long Second_data)
{
	unsigned long  Year = 1900;
	unsigned long  Month = 1;

	unsigned long Day = 1;

	unsigned long Hour = 8;

	unsigned long Min = 0;

	unsigned long Second = 0;

	char buf[32];

	printf("Second_data==%u\n",Second_data);
	while(Second_data--)
	{
		//printf("Second_data==%u\n",Second_data);
		Second++;
		if(Second ==60)
		{
			Second = 0;
			Min ++;
			if(Min == 60)
			{
				Min = 0;
				Hour++;
				if(Hour == 24)
				{
					Hour = 0;
					Day++;
					switch(Day)
					{
						case 29:
						if((Month==2)&&((Year%4)!=0 ||(Year%400)!=0))
						{
							Day= 1;
							Month ++;
						}break;
						case 30:
						if((Month==2)&&((Year%4)==0 ||(Year%400)==0))
						{
							Day= 1;
							Month ++;
						}break;
						case 31:
						if((Month==4)||(Month==6)||(Month==9)||(Month==11))
						{
							Day = 1;
							Month ++;
						}break;
						case 32:
						Day= 1;
						Month ++;
						if(Month == 13)
						{
							Month =1;
							Year ++;
						}break;
						default: break;
					}
				}
			}
		}
	}
	printf("end_Second_data==%u\n",Second_data);
	while(1);
//	snprintf(buf, sizeof(buf),"%04d-%02d-%02d %02d:%02d:%02d (GMT)",Year, Month, Day,Hour, Min, Second);
//	printf("%s\n",buf);

	hl_time.Year = Year;
	hl_time.Month = Month;
	hl_time.Day = Day;
	hl_time.Hour = Hour;
	hl_time.Min = Min;
	hl_time.Second = Second;
//	printf("hl_time.Year = %d hl_time.Month = %d hl_time.Day = %d "
//			"hl_time.Hour = %d hl_time.Min =%d hl_time.Second = %d\n",
//			hl_time.Year,hl_time.Month,hl_time.Day,hl_time.Hour,hl_time.Min,hl_time.Second);

}

#endif



