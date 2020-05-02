/***************************************************************//**
 * @defgroup SpatTx SpatTx API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about SpatMessage.
 *
 * @file SpatTx.h
 * @brief API file.
 *
 *
 *******************************************************************/

#ifndef SPATTX_H_
#define SPATTX_H_
//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../Include/FaciLayer_Include.h"

#include "ExtFunction.h"
#include "DataSet.h"
#include "SpatConfig.h"


extern int FG_SpatTxOpen(struct SpatTx **p_pprectSpat, pthread_attr_t *p_pAttr);
extern void FG_SpatTxclose(struct SpatTx *p_prectSpat);
extern int FG_SpatTxSendMsg(uint8_t *p_ppucSrcBuf, uint32_t p_punLen);


static void FS_SpatTxThreadProc(void *p_pvArg);
static int FS_SpatTxReadlstTxMsg(MessageFrame_t *p_prectSpatMsg);

#endif /* SPATTX_H_ */

/**
 * @}
 */
