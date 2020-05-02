
/**
 * @ingroup SecLayeer API
 * @{
 *
 * 本模块提供V2X安全层相关的API接口.
 *
 * @file FunCommun.h
 * @brief API file.
 *
 */

#ifndef SECLAYER_FUNCOMMUN_H_
#define SECLAYER_FUNCOMMUN_H_

#include <sys/times.h>
#include <sys/time.h>

#include "../NetLayer/DataSet.h"
#include "../NetLayer/DSMP.h"
#include "../SecFrame/SecuredMessage.h"
#include "IWALL_V2X_API.h"
#include "sec_lib.h"
#include "../Include/BaseInterface_Include.h"

#define SECMODE_DMD31_DT 1
#define SECMODE_DMD31_IWALL_SOFT 0
#define SECMODE_AG15_IWALL_SOFT 0
#define SECMODE_AG15_IWALL_HARD 0

#define SECLAYER_PRINTLOG 0
/**
 * @brief 安全验证结果码.
 */
typedef enum SecurityResultCode {
	SecurityResultCode_unknown	= 0,
	SecurityResultCode_incorrectSecureMessageVersion	= 1,
	SecurityResultCode_incorrectSignerType	= 2,
	SecurityResultCode_incorrectCertVersion	= 3,
	SecurityResultCode_incorrectCertIssueDigest	= 4,
	SecurityResultCode_incorrectCertSubjectInfo	= 5,
	SecurityResultCode_incorrectCertSubjectAttribute	= 6,
	SecurityResultCode_incorrectCertValidityPeriod	= 7,
	SecurityResultCode_incorrectCertTimeStartAndEnd	= 8,
	SecurityResultCode_incorrectSubcertAuthority	= 9,
	SecurityResultCode_incorrectCertChain	= 10,
	SecurityResultCode_incorrectCertSignature	= 11,
	SecurityResultCode_incorrectTbsDataGenTime	= 12,
	SecurityResultCode_incorrectTbsDataHashAlg	= 13,
	SecurityResultCode_incorrectTbsDataItsAid	= 14,
	SecurityResultCode_incorrectSignedMessageSignature	= 15
	/*
	 * Enumeration is extensible
	 */
} e_SecurityResultCode;
#define FileMaxSize 500

extern tQueue_RevLst g_Queue_SecRecv;

//asn_dec_rval_t Sec_UperDecode(SecuredMessage_t *pRecvMsg, char *pBuf, size_t Size);
/**
 * @brief 安全消息oer编码.
 * @param[in] pSecMsg  指向用于编码的SecuredMessage_t指针..
 * @param[in] pEcdBuf  编码后数据指针.
 * @param[in] Size  编码后数据长度.
 * @return 编码信息
 */
asn_enc_rval_t Sec_OerEncode(SecuredMessage_t *pSecMsg, char *pEcdBuf, size_t Size);
/**
 * @brief 安全消息oerR解码.
 * @param[in] pSecMsg  指向解码后的SecuredMessage_t指针.
 * @param[in] pDcdBuf      用于解码的数据指针.
 * @param[in] Size      用于解码的数据长度.
 * @return  编码信息.
 */
asn_dec_rval_t Sec_OerDecode(SecuredMessage_t *pSecMsg, char *pDcdBuf, size_t Size);
/**
 * @brief 计算2004年到现在的微秒数.
 * @return  2004年到现在的微妙数.
 */
int GetTimeStamp2004_US(uint64_t *lTimeStamp);
/**
 * @brief 计算2004年到现在的秒数.
 * @return  2004年到现在的妙数.
 */
int GetTimeStamp2004_S(uint32_t *lTimeStamp);

int GetTimeStamp_s_fmGenTime(Time64_t *genTime, uint64_t *lTimeStamp);
/**
 * @brief 读取证书.
 * @param[in] pCertificate  指向证书的Certificate_t指针.
 * @param[in] filename 文件路径.
 */
int Create_Certificate_fromFile(Certificate_t *pCertificate, char filename[]);

int SetGenTime(Time64_t *genTime, uint64_t lTimeStamp);

int print_Info(char print_Info[], unsigned char buf[], int buflen);
/**
 * @brief 读取文件信息，获取数据.
 * @param[in] filename 文件路径.
 * @param[in] buf   用于存储数据.
 * @param[in] buflen 数据长度.
 */
int ReadFile(char filename[], unsigned char buf[], int *buflen);
/**
 * @brief 将ASIC转变成Hex.
 * @param[in] Asicbuf  Asic.
 * @param[in] lAsicbuflen 数据长度.
 * @return  Hex数据.
 */
int Trans_Asic2Hex(unsigned char Asicbuf[], int lAsicbuflen);
#endif /* SECLAYER_FUNCOMMUN_H_ */
/**
 * @}
 */
