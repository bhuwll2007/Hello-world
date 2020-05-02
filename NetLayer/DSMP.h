
/**
 * @defgroup NetLayer NetLayer API
 * @{
 *
 * 本模块提供V2X网络层的API接口.
 *
 * @}
 *
 */

/**
 * @ingroup NetLayer API
 * @{
 *
 * 本模块V2X网络层的API接口.
 *
 * @file DSMP.h
 * @brief API file.
 *
 */
#ifndef NETLAYER_DSMP_H_
#define NETLAYER_DSMP_H_

#include "DataSet.h"

//Protool Type Define

#define ADAPT_LAYER_IPV6 0
#define ADAPT_LAYER_DSMP 4
#define ADAPT_LAYER_IPV4 5

//DSMP Version
#define DSMP_VERSION 0

//AID Allocation
#define AID_VEHICLE_SAFETY 	  17
#define AID_NETLAYER_TEST 	 127
#define AID_DSRC_SERVICE_ADV 135

//Element ID
//DSA Header
#define DSMP_EID_TX_POWER_USED		   4
#define DSMP_EID_2DLOCATION            5
#define DSMP_EID_3DLOCATION            6
#define DSMP_EID_ADVERTISER_ID         7
#define DSMP_EID_BROADCAST_FREQUENCY  14
#define DSMP_EID_AREA_STRING          15

//DSA Application Info
#define DSMP_EID_PROVIDER_SERV_CONTEXT 8
#define DSMP_EID_IPV6_ADDRESS          9
#define DSMP_EID_SERVICE_PORT         10
#define DSMP_EID_PROVIDER_MAC_ADDRESS 11
#define DSMP_EID_RCPI_THRESHOLD       16
#define DSMP_EID_DSA_COUNT_THRESHOLD  17
#define DSMP_EID_WSA_COUNT_THRES_INT  18

//DSMP Header
#define DSMP_EID_CHANNEL_NUMBER		  12
#define DSMP_EID_DATA_RATE		      13

//DSMP Header Extension
#define DSM_HEADER_EXTENSION_YES 0
#define DSM_HEADER_EXTENSION_NO  1

/**
 * @brief 高层请求发送的DSM数据信息.
 */
typedef struct _DSM_Request
{
	char ProtocalType;   //协议类型
	short AID;			//应用标识
	int Priority;		//优先级
	short Length;			//DSM数据实体字节长度
	char *Data;		//DSM数据实体部分
	char *DsmpHeaderExtensions;		//指定帧头扩展域

}__attribute__ ((packed)) tDSM_Request;



//typedef struct WAVEConfig
//{
//	uint8_t OptionIndicator;
//	uint16_t AID;
//} tWAVEConfig;
//
//typedef struct Extension
//{
//    uint8_t ElementID;
//    uint8_t ByteLength;
//    char *ExtensionInfo;
//} __attribute__ ((packed)) tExtension;
//
//typedef struct DSMPHdr
//{
//    uint8_t Version;
//    uint8_t DSMP_OptionIdr;
//    uint16_t AID;
//    int Length;
//    uint8_t *Data;
//    tExtension *pExtension;
//} __attribute__ ((packed)) tDSMPHdr;

typedef void (*pDSMData)(void *p_DSMPHdr);


/**
 * @brief 接收网络层数据.
 * @param[in] nAid  AID.
 * @param[in] buf  获取网络层数据.
 * @param[in] buflen  获取网络层数据长度.
 */
int GetLstNetRecvData(int *nAid, char buf[], int *buflen);

int DSM_Init(int RxMode, uint16_t Aid[], int nAidCnt);
int DSM_CallbackRegister(void (* ptr)(tDSMPHdr *ptr));


int DSMPHeader_Create(tDSM_Request dsmSend_rq, char **pHdr);
void DSMPHeader_Decode(char *pBuf, int *pnRecvBufSize, tDSMPHdr *pDsmpHdr);
int DSMSend(tDSM_Request dsmSend_rq);

void MsgRx_CallBack(char *p_cRecvBuf, int *pnRecvBufSize);

void DSMPHeader_free(tDSMPHdr *pDsmpHdr);
#endif /* DSMP_H_ */
