
#ifndef __APPLYLAYER_MSGQUEUE_MSGQUEUE_C_
#define __APPLYLAYER_MSGQUEUE_MSGQUEUE_C_

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <sys/time.h> // for rate control
typedef struct _Msg_Queue {
	int nMsg_Cmd;
	int nMsg_Len;
	char acMsg_Text[2048];
}tMsg_Queue_STCU;

int MsgQueue_New(int nMsg_Key, int Mode);
int MsgQueue_Send(int msqid, tMsg_Queue_STCU msgs);
int MsgQueue_Recv(int msqid, tMsg_Queue_STCU *msgs);
int MsgQueue_Delete(int msqid);

#endif
