/***************************************************************//**
 * @defgroup RsmConfig SpatConfig API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about RsmConfig.
 *
 * @file SpatConfig.h
 * @brief API file.
 *  Created on: 20200316
 *      Author: chaizhijun
 *******************************************************************/

#ifndef __SPATCONFIG_H_
#define __SPATCONFIG_H_
#include "./../Algorithm/Type.h"
#include "./../Include/BaseInterface_Include.h"

#include "../BaseInterface/DataSet.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../MsgFrame/MessageFrame.h"
#include "../SecLayer/SecLayer_Deal.h"

#include "../Include/FaciLayer_Include.h"

#include "ExtFunction.h"
#include "DataSet.h"
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------

enum SPAT_LightState
{
	GreenLight = 1, YellowLight = 2, RedLight = 3
};

typedef struct _PhaseInfo
{
	int ID;//ID
	int Property;//属性
	int GreenTime;//绿灯时间
	int YellowTime;//黄灯时间
	int RedTime;//红灯时间
	int Status;//当前状态
	int TimeLeft;//当前状态剩余时间
} tPhaseInfo;

typedef struct _IntersectionInfo
{
	int ID;
	short Status;
	int Cycle;
	int PhaseCnt;
	tPhaseInfo *pPhaseInfo;
} tIntersectionInfo;

typedef struct _SpatMsg
{
	int IntersectionCnt;
	tIntersectionInfo *pIntersectionInfo;
} tSpatMsg;

typedef struct SpatData
{
	nint32_t In;
	nint32_t Out;
	//MessageFrame_t *MsgData;
	int BufLen;
	char pBuf[1024];
} tSpatData;


extern void FG_SpatRecvMsgFree(void);

extern int FG_SpatRecvMsgGet(uint8_t *p_pucDestBuf, uint32_t *p_punLen);

extern int FG_SpatRecvMsgSet(tSpatMsg p_rectSpatInfo);

#endif /*__LASERDATA_H_*/

/**
 * @}
 */
