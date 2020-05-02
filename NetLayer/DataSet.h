/*
 * DataSet.h
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#ifndef __NETLAYER_DATASET_H_
#define __NETLAYER_DATASET_H_
#include "../BaseInterface/DataSet.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#define NETLAYER_PRINTLOG 0
//extern int g_Recv102BsmTime;//统计接收到102车的BSM个数
//extern int g_RecvBsmTime;//统计接收到周围车的BSM个数
//extern int g_RecvSecMsgOk;//统计安全层延签通过的数据的个数
//extern int g_RecvSecMsgCnt;//统计安全层接收到的数据的个数
//extern int g_RecvNetMsgCnt;//统计网络层接收到的数据的个数

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
#define DCR_OPTHOSTINFO 0
#define DCR_OPTHOSTSTSTE 0
#define DCR_OPTGETCOMMINFO 0
#define DCR_OPTSETCOMMCFG 0
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

typedef enum
{
	Init_ERR_NONE = 0,
	Init_ERR_REJECT = 1,
	Init_ERR_TxINIT_FAIL = 2,
	Init_ERR_RxINIT_FAIL = 3,
	Init_ERR_Rxpthread_FAIL = 4,
} Init_ErrorCode;
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
/// Error codes
typedef enum
{
	MSG_ERR_NONE = 0,
	MSG_ERR_MALLOC = 1,
	MSG_ERR_FILEOPEN = 2,
	MSG_ERR_CONFIGFILE = 3,
	MSG_ERR_INTERFACE = 4,
} MGS_ErrorCode;

/// Msg Send Error Codes
typedef enum
{
	Send_ERR_NONE = 0,
	Send_ERR_REJECT = 1,
	Send_ERR_CREATE_FAIL = 2,
	Send_ERR_ENCODE_FAIL = 3,
	Send_ERR_SEND_FAIL = 4,
	Send_ERR_THREAD_FAIL = 5,

} SEND_ErrorCode;

/// Error codess
typedef enum
{
	MsgDup_ERR_NONE = 0, MsgDup_ERR_REJECT = 1, MsgDup_ERR_MSGTYPE = 2,
} MsgDup_ErrorCode;


typedef struct WAVEConfig
{
	uint8_t OptionIndicator;
	uint16_t AID;
} tWAVEConfig;

//extern tDsaTx_Base g_DsaTx;
#define NUM 4
#define MAXSIZE_MSGBUF 1500
typedef struct stc_msg
{
	nint32_t mnTxAid;
	nint32_t mnBuffLen;
	nint8_t macBuff[MAXSIZE_MSGBUF];
} tRcvMsg;
typedef struct stc_Queue
{
	tRcvMsg m_stcqueue[NUM];
	nint32_t m_nQIndx_In;
	nint32_t m_nQIndx_Out;
	sem_t m_blank_number;
	sem_t m_product_number;
} tQueue_RevLst;

extern tQueue_RevLst g_Queue_NetRecv;
//extern pthread_mutex_t mutex_RecvNetLayer;
//extern pthread_mutex_t mutex_RecvSecLayer;

typedef struct Extension
{
    uint8_t ElementID;
    uint8_t ByteLength;
    char *ExtensionInfo;
} tExtension;

typedef struct DSMPHdr
{
    uint8_t Version;
    uint8_t DSMP_OptionIdr;
    uint16_t AID;
    int Length;
    uint8_t *Data;
    tExtension *pExtension;
} __attribute__ ((packed)) tDSMPHdr;


#endif /* __NETLAYER_DATASET_H_ */
