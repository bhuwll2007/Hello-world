/*
 * RsiConfig.c
 *
 *  Created on: 20200316
 *      Author: chaizhijun
 */
#include "RsiConfig.h"

//功能：读取和填充平台Rsi结构数据使用，互斥变量
//作者：柴志军 时间：20200317
static pthread_mutex_t s_mutexRecvPlatForm;


//功能：平台Rsi数据结构变量，用于存储激光雷达结构变量
//作者：柴志军 时间：20200317
static rectRsiPlatformToV2X_t *s_prectRsiRecvFromPlatform = NULL;

/**********************************************
*功能：释放平台Rsi数据结构变量资源
*输入参数： void
*输出参数： 无
*函数返回值： 无
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
void FG_RsiRecvFromPlatformFree(void)
{
	free(s_prectRsiRecvFromPlatform);
}

/**********************************************
*功能：获取接收到的平台Rsi数据结构变量
*输入参数： 无
*输出参数：*p_prectRsiPlatformToV2Xt  平台Rsi结构数据
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
int FG_RsiRecvFromPlatformMsgGet(rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt)
{
	if (NULL == p_prectRsiPlatformToV2Xt)
	{
		printf("--%s %s file %s line:%d\n",
		__DATE__, __TIME__, __FILE__, __LINE__);
		return -1;
	}

	//数据已经填充，直接赋值；若没填充，以0赋值；
	if (NULL == s_prectRsiRecvFromPlatform)
	{
		memset(p_prectRsiPlatformToV2Xt, 0, sizeof(rectRsiPlatformToV2X_t));
		return -1;
	}
	else
	{
		pthread_mutex_lock(&s_mutexRecvPlatForm);
		*p_prectRsiPlatformToV2Xt = *s_prectRsiRecvFromPlatform;
		pthread_mutex_unlock(&s_mutexRecvPlatForm);
	}

	return 0;
}

/**********************************************
*功能：填充接收到的平台Rsi数据结构变量
*输入参数： *p_prectRsiPlatformToV2Xt  平台Rsi结构数据
*输出参数： 无
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-17 版本号：V1.0.0
修改记录：无
************************************************/
int FG_RsiRecvFrompPlatformMsgSet(rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt)
{
	if (NULL == p_prectRsiPlatformToV2Xt)
	{
		printf("--%s %s file %s line:%d\n",
		__DATE__, __TIME__, __FILE__, __LINE__);
		return -1;
	}

	if (NULL == s_prectRsiRecvFromPlatform)
	{//先申请结构数据资源
		s_prectRsiRecvFromPlatform = calloc(1, (sizeof(rectRsiPlatformToV2X_t)));
		if (NULL == s_prectRsiRecvFromPlatform)
		{
			printf("--%s %s file %s line:%d\n",
			__DATE__, __TIME__, __FILE__, __LINE__);
			return -1;
		}
		pthread_mutex_init(&s_mutexRecvPlatForm, NULL);
	}

	//直接赋值
	pthread_mutex_lock(&s_mutexRecvPlatForm);
	*s_prectRsiRecvFromPlatform = *p_prectRsiPlatformToV2Xt;
	pthread_mutex_unlock(&s_mutexRecvPlatForm);

	return 0;
}



