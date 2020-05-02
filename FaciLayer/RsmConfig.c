/*
 * RsmConfig.c
 *
 *  Created on: 20200310
 *      Author: chaizhijun
 */
#include "RsmConfig.h"

//功能：读取和填充激光雷达结构数据使用，互斥变量
//作者：柴志军 时间：20200310
static pthread_mutex_t S_mutexRecvFromLaser;

//功能：激光雷达数据结构变量，用于存储激光雷达结构变量
//作者：柴志军 时间：20200310
static rectMSGLaserToV2X_t *S_prectMsgRecvFromLaser = NULL;

/**********************************************
*功能：释放激光雷达数据结构变量资源
*输入参数： void
*输出参数： 无
*函数返回值： 无
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
void FG_RecvFromLaserMsgFree(void)
{
	free(S_prectMsgRecvFromLaser);
}

/**********************************************
*功能：获取接收到的激光雷达数据结构变量
*输入参数： 无
*输出参数：p_prectMSGLaserToV2Xt  激光雷达结构数据
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
int FG_RecvFromLaserMsgGet(rectMSGLaserToV2X_t *p_prectMSGLaserToV2Xt)
{
	if (NULL == p_prectMSGLaserToV2Xt)
	{
		printf("--%s %s file %s line:%d\n",
		__DATE__, __TIME__, __FILE__, __LINE__);
		return -1;
	}

	//激光雷达结构数据已经填充，直接赋值；若没填充，以0赋值；
	if (NULL == S_prectMsgRecvFromLaser)
	{
		memset(p_prectMSGLaserToV2Xt, 0, sizeof(rectMSGLaserToV2X_t));
	}
	else
	{
		pthread_mutex_lock(&S_mutexRecvFromLaser);
		*p_prectMSGLaserToV2Xt = *S_prectMsgRecvFromLaser;
		pthread_mutex_unlock(&S_mutexRecvFromLaser);
	}

	return 0;
}

/**********************************************
*功能：填充接收到的激光雷达数据结构变量
*输入参数： p_prectMSGLaserToV2Xt  激光雷达结构数据
*输出参数： 无
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
int FG_RecvFromLaserMsgSet(rectMSGLaserToV2X_t *p_prectMSGLaserToV2Xt)
{
	if (NULL == p_prectMSGLaserToV2Xt)
	{
		printf("--%s %s file %s line:%d\n",
		__DATE__, __TIME__, __FILE__, __LINE__);
		return -1;
	}

	if (NULL == S_prectMsgRecvFromLaser)
	{//先申请激光雷达结构数据资源
		S_prectMsgRecvFromLaser = calloc(1, (sizeof(rectMSGLaserToV2X_t)));
		if (NULL == S_prectMsgRecvFromLaser)
		{
			printf("--%s %s file %s line:%d\n",
			__DATE__, __TIME__, __FILE__, __LINE__);
			return -1;
		}
		pthread_mutex_init(&S_mutexRecvFromLaser, NULL);
	}

	//直接赋值
	pthread_mutex_lock(&S_mutexRecvFromLaser);
	*S_prectMsgRecvFromLaser = *p_prectMSGLaserToV2Xt;
	pthread_mutex_unlock(&S_mutexRecvFromLaser);

	return 0;
}



