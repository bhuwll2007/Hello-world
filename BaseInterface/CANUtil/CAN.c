#include "CAN.h"
#define MSGKEY_RECVCAN 10051
#define MSGKEY_SENDCAN 10052

pthread_mutex_t g_mutex_CanData;

CANData g_stcCanData;
/**
 * 接受CAN数据线程
 * 函数名称：void Thread_RecvCANData(void *p)
 * 函数功能：接受CAN数据线程，并按照定义解析；
 * 参数说明：无
 * 函数返回值：无
 * */
void Thread_RecvCANData(void *a)
{
	//初始化互斥信号量
	pthread_mutex_init(&g_mutex_CanData, NULL);

	//创建消息队列
	tMsg_Queue_STCU msgs;
	int msgid;
	int nkeyId = MSGKEY_RECVCAN;
	nkeyId = MSGKEY_RECVCAN;
	while (1) {
		msgid = MsgQueue_New(nkeyId,IPC_CREAT);
		if (msgid < 0) {
			printf("msq not existed! errno=%d [%s]\n", errno, strerror(errno));
			sleep(2);
			continue;
		}
		else
		{
			break;
		}
	}
	//循环读取消息队列
	while (1) {
		int nLen = MsgQueue_Recv(msgid, &msgs);
		if(nLen > 0)
		{
//			printf("%d, acMsg_Text = %s, %d\n", msgs.nMsg_Cmd, msgs.acMsg_Text, msgs.nMsg_Len);

			if(msgs.nMsg_Cmd == 1)
			{
				pthread_mutex_lock(&g_mutex_CanData);
				memcpy(&(g_stcCanData.lat),&(msgs.acMsg_Text),
						sizeof(CANData));
				pthread_mutex_unlock(&g_mutex_CanData);
			}
		}
	}
}
/**
 * CAN模块初始化
 * 函数名称：int CAN_Open()
 * 函数功能：打开获取CAN数据的串口，并开启接收CAN数据线程；
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 * */
int CAN_Open(char *pCanName)
{
	pthread_t canthreadid;
	int nRet = 0;
	nRet = pthread_create(&canthreadid, NULL, (void*) Thread_RecvCANData,
			NULL);
	return nRet;
}
/**
 * 获取当前CAN数据
 * 函数名称：int CAN_GetData(GpsLocation * fix)
 * 函数功能：读取当前最新的CAN数据，CAN数据格式按照指定的数据格式传出；
 * 参数说明：
 * 		输出参数：GpsLocation * fix 最新获取的CAN信息的指针
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 * */
int CAN_GetData(CANData * pstcCanData)
{
	pthread_mutex_lock(&g_mutex_CanData);
	if (g_stcCanData.Release1 != 0)
		memset(&(g_stcCanData.lat), 0x00, sizeof(CANData));
	memcpy(&(pstcCanData->lat), &(g_stcCanData.lat), sizeof(CANData));
	pthread_mutex_unlock(&g_mutex_CanData);
	return 0;
}
/**
 * 发送CAN数据
 * 函数名称：int CAN_SendData(int sock, char* str, int len)
 * 函数功能：往sock端口中发送数据，数据内容为str，数据长度为len；
 * 参数说明：
 * 		输入参数：
 * 		int sock, 数据通讯端口句柄
 * 		char* str, 数据内容
 * 		int len 数据长度
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 * */
int CAN_SendData(int psock, char* str, int len)
{
	return 0;
}
/**
 * 关闭CAN模块
 * 函数名称：int CAN_Close()
 * 函数功能：关闭CAN数据获取模块，即关闭CAN网口，停止CAN数据接收线程，清空相关变量内容；
 * 参数说明：无
 * 函数返回值：执行状态，0 执行成功，非0 执行失败
 * */
int CAN_Close()
{
	return 0;
}
