/*
 * DataSet.c
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */
#include "DataSet.h"
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
//MessageFrame_t tDecodedBuf;

tConv_RxMsgDa bsmFrame;
tConv_RxMsgDa rsiFrame;
tConv_RxMsgDa mapFrame;
tConv_RxMsgDa spatFrame;
tConv_RxMsgDa rsmFrame;

pthread_mutex_t mutex_BSM;
pthread_mutex_t mutex_RSI;
pthread_mutex_t mutex_MAP;
pthread_mutex_t mutex_SPAT;
pthread_mutex_t mutex_RSM;

//读取发送数据记录的文件的互斥锁
pthread_mutex_t mutex_StoreBSM;
pthread_mutex_t mutex_StoreRSI;
pthread_mutex_t mutex_StoreMAP;
pthread_mutex_t mutex_StoreSPAT;
pthread_mutex_t mutex_StoreRSM;

pthread_mutex_t mutex_TxSPAT;
pthread_mutex_t mutex_TxRSM;

//全局变量
tStateStore Store;


//接收消息
tQueue_RevLst g_Queue_Recv_bsm;
tQueue_RevLst g_Queue_Recv_spat;
tQueue_RevLst g_Queue_Recv_rsm;
tQueue_RevLst g_Queue_Recv_rsi;
tQueue_RevLst g_Queue_Recv_map;
//发送消息
tQueue_RevLst g_Queue_Send_Rsm;

