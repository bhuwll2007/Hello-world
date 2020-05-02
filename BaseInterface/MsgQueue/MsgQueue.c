/*send.c*/
#include "MsgQueue.h"

int MsgQueue_New(int nMsg_Key, int Mode)
{
	int msqid = -1;
	msqid = msgget(nMsg_Key, IPC_EXCL); /*检查消息队列是否存在*/
	if (msqid < 0)
	{
		if (Mode == IPC_EXCL)
		{
			printf("msq not excl!\n");
			return -1;
		} else if (Mode == IPC_CREAT)
		{
			msqid = msgget(nMsg_Key, IPC_CREAT | 0666);/*创建消息队列*/
			if (msqid < 0)
			{
				printf("failed to create msq | errno=%d [%s]\n", errno,
						strerror(errno));
				return -2;
			} else
			{
				printf("msq create success! msqid = %d \n", msqid);
				return msqid;
			}
		}
	} else
	{
		printf("msq excl! msqid = %d\n", msqid);
	}
	return msqid;
}
int MsgQueue_Send(int msqid, tMsg_Queue_STCU msgs)
{
	if (msqid < 0)
	{
		return 0;
	}
	int ret_value = 0;
	ret_value = msgsnd(msqid, &msgs, msgs.nMsg_Len + 2 * sizeof(int),
			IPC_NOWAIT);

	if (ret_value < 0)
	{
		printf("msgsnd() write msg failed, errno = %d[%s]\n", errno,
				strerror(errno));
		return -1;
	} else
	{
//		printf("ret_value = %d\n", ret_value);
	}
	return ret_value;
}
int MsgQueue_Recv(int msqid, tMsg_Queue_STCU *msgs)
{
	if (msqid < 0)
	{
		return 0;
	}
	int ret_value = 0;
	/*接收消息队列*/
	ret_value = msgrcv(msqid, msgs, sizeof(tMsg_Queue_STCU), 0, 0);
	if (ret_value > 0)
	{
		return ret_value;
	}
	return 0;
}
int MsgQueue_Delete(int msqid)
{
	if (msqid < 0)
	{
		return 0;
	}
	msgctl(msqid, IPC_RMID, 0); //删除消息队列
	return 0;
}
