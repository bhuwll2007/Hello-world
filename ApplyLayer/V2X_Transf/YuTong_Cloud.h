#ifndef __YUTONGCLOUD_H_
#define __YUTONGCLOUD_H_

#include "./../DataSet/DataSet.h"
#include "./../../Include/FaciLayer_Include.h"
#include "./../../Include/BaseInterface_Include.h"
#include "./../Common/Common.h"

extern StcTcpClient *g_prectYuTongTcpClient; //连接宇通云平台
extern int g_nRecvRegisterOKFlag;
extern int FG_YuTong2CloudEncodeFrame(uint8_t p_ucVersionId, uint8_t p_ucFrameTypeId,
		nint8_t p_caDatabuf[], uint32_t p_unDataLen, nint8_t p_caDesbuf[], uint32_t *p_punDesLen);
extern int FG_YuTongCloudInit(tStateStore *p_prectApp);


static void FS_YuTongYunRegisterSendData(void);
static void FS_YuTongYunTimerSendRegister(void);
static void *FS_ThreadLinkCloud(void * p_pvArg);
static void FS_YuTongYunCallBack(char *p_pcRecvBuf, uint32_t p_pnRecvBufSize);

#endif
