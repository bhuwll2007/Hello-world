/*
 * Spat.c
 *
 *  Created on: Apr 12, 2018
 *      Author: root
 */
#include "SpatTx.h"

/**
 * 功能描述: 分配Spat消息对象，配置线程、关联Spat数据结构
 * 输入参数: ppSpat 指向Spat句柄的指针
 * 			pAttr 线程属性
 * 输出参数:
 * 返回值 : 0 执行成功， 非0 失败标识
 **/
int FG_SpatTxOpen(struct SpatTx **p_pprectSpat, pthread_attr_t *p_pAttr)
{
	int Res = Send_ERR_NONE;
	struct SpatTx *pSpat;

	// Create and check the local structure
	pSpat = (struct SpatTx *) malloc(sizeof(struct SpatTx));

	if (pSpat == NULL)
	{
		Res = MSG_ERR_MALLOC;
		goto Error;
	}
	memset(pSpat, 0, sizeof(struct SpatTx));

	pSpat->ThreadAttr = *p_pAttr;

	// Create thread
	pSpat->ThreadState |= SPATTX_THREAD_STATE_INIT;
	Res = pthread_create(&pSpat->ThreadID, &pSpat->ThreadAttr,
			(void *) FS_SpatTxThreadProc, pSpat);

	if (Res)
	{
		pSpat->ThreadState = SPATTX_THREAD_STATE_NONE;
		Res = Send_ERR_THREAD_FAIL;
		goto Error;
	}

	//Success
	*p_pprectSpat = pSpat;
	Res = 0;
	goto Success;

	Error: *p_pprectSpat = NULL;
	FG_SpatTxclose(pSpat);
	Success: return Res;
}

/**
 * 功能描述: 停止执行SpatTx，释放线程及相关资源
 * 输入参数: pSpat Spat句柄
 * 输出参数:
 * 返回值 :
 **/
void FG_SpatTxclose(struct SpatTx *p_prectSpat)
{
	// Signal thread to terminate
	p_prectSpat->ThreadState |= SPATTX_THREAD_STATE_STOP;
	// Wait for thread termination
	if (p_prectSpat->ThreadState & SPATTX_THREAD_STATE_INIT)
		pthread_join(p_prectSpat->ThreadID, NULL);
	free(p_prectSpat);
	return;
}

/**
 * 功能描述: 周期执行Spat发送线程
 * 输入参数: pArg 线程调用传入参数指针
 * 输出参数:
 * 返回值 :
 **/
static void FS_SpatTxThreadProc(void *p_pvArg)
{
//	int Res;
	struct SpatTx *pSpat;
	pSpat = (struct SpatTx *) p_pvArg;
	struct timeval delay;

//	memset(&SpatFrame, 0x00, sizeof(tSpatData));

	// Starting SpatTx Periodic Thread
	pSpat->ThreadState |= SPATTX_THREAD_STATE_RUN;

	int BufLen = 0;
	char pBuf[2028];
//Thread loop
	while ((pSpat->ThreadState & SPATTX_THREAD_STATE_STOP) == 0)
	{
		if (Store.HostComInfo.SpatTx.TxEnable)
		{
			int Res = 0;
			Res = FG_SpatRecvMsgGet(pBuf,&BufLen);

			if(0 == Res)
			{
				if(0 < BufLen)
				{
					Res = FG_SpatTxSendMsg(pBuf, BufLen);
					if (Res == 0)
					{
						pSpat->Stats.Tx.Okay++;
					}
					else
					{
						pSpat->Stats.Tx.Fail++;
						printf("The SpatTx Fail Code is = %d\n", Res);
					}
					printf("Spat_Tx Count = %d\n", pSpat->Stats.Tx.Okay);
				}
			}
		}
		else
		{
			msTimeDelay(delay, 10);
		}
	}

	FG_SpatRecvMsgFree();
	(void) pthread_exit(NULL);
}

/**
 * 功能描述: 发送一条Spat消息
 * 输入参数: pSpat Spat句柄
 * 输出参数:
 * 返回值 : 0 执行成功， 非0 失败标识
 **/
int FG_SpatTxSendMsg(uint8_t *p_ppucSrcBuf, uint32_t p_punLen)
{
	int Res = Send_ERR_NONE;

	tDSM_Request dsmSend_rq;
	dsmSend_rq.AID = 17;
	dsmSend_rq.Length = p_punLen;
	dsmSend_rq.Data = p_ppucSrcBuf;
	Res = SecLayer_Send(dsmSend_rq);
	if (Res)
	{
		Res = Send_ERR_SEND_FAIL;
	}

	return Res;
}

/**
 * @brief 读取设备最后发送的那一条Spat消息.
 * @param[out] pSpatMsg Spat句柄.
 * @return 0 读取失败，4 读取成功，本消息确实为Spat信息. 其他 ，不合法
 */
static int FS_SpatTxReadlstTxMsg(MessageFrame_t *p_prectSpatMsg)
{
	pthread_mutex_lock(&mutex_StoreSPAT);
	ReadXml(&p_prectSpatMsg, "./TxLog/Spat_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreSPAT);
	return p_prectSpatMsg->present;
}




