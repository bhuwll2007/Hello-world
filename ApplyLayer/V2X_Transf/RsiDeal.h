/*
 * RsiDeal.h
 *
 *  Created on: Jul 11, 2019
 *      Author: wanji-hxy
 */

#ifndef APPLYLAYER_V2X_TRANSF_PLATFORM_H_
#define APPLYLAYER_V2X_TRANSF_PLATFORM_H_


#include "./../Common/Common.h"
#include "TcpClient.h"
#include "./../Log/FileUnit.h"

#define LOGLENG 7


extern int FG_PlatformLinkTcpinit();
extern int FG_PlatformLinkUdpinit(int p_nCliPort);




static void *SF_PlatformThreadLinklive(void * p_pvArgs);

static void SF_PlatformSendLogin();

static int SF_PlatformEncodeFrame(uint8_t p_ucCmdType, uint8_t p_ucResponseType, uint8_t p_ucFrameCnt, \
		nint8_t p_pcDatabuf[], uint16_t p_sDataLen, nint8_t p_pcDesbuf[], uint32_t *p_punDesLen);

static int SF_PlatformEncodeFrame1(uint8_t p_ucCmdId, uint8_t p_ucSubCmdId, uint8_t p_ucState, \
		char p_pcSrcbuf[], uint32_t p_unSrcLen, char p_pcDesbuf[], uint32_t *p_punDesLen);

static int SF_AnalysisPlatformBuff(rectRsiPlatformToV2X_t **p_prectRsiPlatformToV2Xt,nint8_t *p_pcRecvBuf, uint32_t p_nRecvBufSize);

static void SF_RsiUdpRecvData(nint8_t *p_pcRecvBuf, uint32_t p_nRecvBufSize);


#endif /* APPLYLAYER_V2X_TRANSF_PLATFORM_H_ */
