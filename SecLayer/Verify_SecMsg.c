/*
 * Verify_SecMsg.c
 *
 *  Created on: Sep 24, 2019
 *      Author: wanji
 */

#include "Verify_SecMsg.h"
/// Alloc structure size with calloc
#define ALLOC_OBJ(type, size) \
  (type *)calloc(1, (sizeof(type)*size))
#define Test_Check_All 1

#define MAX_BUFSIZE 1000

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
#if SECMODE_DMD31_DT
int Certificate_Verify(char PCAName[], Certificate_t *PA_Cert)
{
	int ret = 0;
	asn_enc_rval_t EncRVal;
	//读取两个证书
	Certificate_t *PCA_Cert = ALLOC_OBJ(Certificate_t, 1);
	Create_Certificate_fromFile(PCA_Cert, PCAName);

	//计算PCA的oer
	unsigned char pBuf_PCA_Cert[1000];
	int nBuf_PCA_CertSize = 1000;
	EncRVal = oer_encode_to_buffer(&asn_DEF_Certificate, NULL, PCA_Cert,
			pBuf_PCA_Cert, nBuf_PCA_CertSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
		ASN_STRUCT_FREE(asn_DEF_Certificate, PCA_Cert);
		return 1;
	} else
	{
		nBuf_PCA_CertSize = EncRVal.encoded;
	}

	//计算hash值
	unsigned char pBuf_PCA_cer_hsah[1000];
	int nBuf_PCA_cer_hsahSize = 1000;
	memset(pBuf_PCA_cer_hsah, 0x00, 1000);
	ret = SM3Hash(pBuf_PCA_Cert, nBuf_PCA_CertSize, pBuf_PCA_cer_hsah,
			nBuf_PCA_cer_hsahSize);

	//对比摘要

	//计算PCA公钥
	unsigned char pk_c[32];
	unsigned char pk_de[64];
	if (PCA_Cert->tbs.subjectAttributes.verificationKey.key.present
			== ECCPoint_PR_compressed_y_0)
	{
		memcpy(pk_c,
				PCA_Cert->tbs.subjectAttributes.verificationKey.key.choice.compressed_y_0.buf,
				32);
		SM2PkDecompress(pk_de, pk_c, 0);
	}
	if (PCA_Cert->tbs.subjectAttributes.verificationKey.key.present
			== ECCPoint_PR_compressed_y_1)
	{
		memcpy(pk_c,
				PCA_Cert->tbs.subjectAttributes.verificationKey.key.choice.compressed_y_1.buf,
				32);
		SM2PkDecompress(pk_de, pk_c, 1);
	}

	//计算OBU假名证书的tbs的oer
	unsigned char pBuf_OBU_Tbs[1000];
	int nBuf_OBU_TbsSize = 1000;
	EncRVal = oer_encode_to_buffer(&asn_DEF_TbsCert, NULL, &PA_Cert->tbs,
			pBuf_OBU_Tbs, nBuf_OBU_TbsSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
	} else
	{
		nBuf_OBU_TbsSize = EncRVal.encoded;
		print_Info("OBU_tbs_oer ", pBuf_OBU_Tbs, nBuf_OBU_TbsSize);
	}

	//计算OBU假名证书的tbs的hash值
	unsigned char pBuf_obu_tbs_hsah[1000];
	int nBuf_obu_tbs_hsahSize = 1000;
	memset(pBuf_obu_tbs_hsah, 0x00, 1000);
	SM3Hash(pBuf_OBU_Tbs, nBuf_OBU_TbsSize, pBuf_obu_tbs_hsah,
			nBuf_obu_tbs_hsahSize);

	//延签
	memcpy(&pBuf_obu_tbs_hsah[32], pBuf_PCA_cer_hsah, 32);
	unsigned char pBuf_obu_cert_sign_Info[64];
	int nBuf_obu_cert_sign_InfoSize = 64;
	if (PA_Cert->signature.r.choice.x_only.buf != NULL)
		memcpy(pBuf_obu_cert_sign_Info, PA_Cert->signature.r.choice.x_only.buf,
				PA_Cert->signature.r.choice.x_only.size);
	if (PA_Cert->signature.s.buf != NULL)
		memcpy(&pBuf_obu_cert_sign_Info[32], PA_Cert->signature.s.buf,
				PA_Cert->signature.s.size);

	ret = SM2VerifyMessage(pBuf_obu_tbs_hsah, 64, pk_de,
			pBuf_obu_cert_sign_Info, nBuf_obu_cert_sign_InfoSize);
	ASN_STRUCT_FREE(asn_DEF_Certificate, PCA_Cert);
	return ret;
}

int SecuredMessage_Verify(SecuredMessage_t *pSecuredMessage)
{
	int ret = 0;
	asn_enc_rval_t EncRVal;
//负载信息	Payload_t	 payload;
	Payload_t* payload = &(pSecuredMessage->payload);

	TBSData_t *ptbs = &payload->choice.signedData.tbs;
	unsigned char pBuf_tbs[2000];
	int nBuf_tbsSize = 2000;
	EncRVal = oer_encode_to_buffer(&asn_DEF_TBSData, NULL, ptbs, pBuf_tbs,
			nBuf_tbsSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
		return -1;
	} else
	{
		nBuf_tbsSize = EncRVal.encoded;
	}

	unsigned char pBuf_cer[2000];
	int nBuf_cerSize = 2000;

	struct Certificate *pCertificate =
			payload->choice.signedData.signer.choice.certificateList.list.array[0];
	EncRVal = oer_encode_to_buffer(&asn_DEF_Certificate, NULL, pCertificate,
			pBuf_cer, nBuf_cerSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
		return -2;
	} else
	{
		nBuf_cerSize = EncRVal.encoded;
	}
//计算hash值
	unsigned char pBuf_tbs_hsah[MAX_BUFSIZE];
	int nBuf_tbs_hsahSize = MAX_BUFSIZE;
	unsigned char pBuf_cer_hsah[MAX_BUFSIZE];
	int nBuf_cer_hsahSize = MAX_BUFSIZE;
	memset(pBuf_tbs_hsah, 0x00, sizeof(pBuf_tbs_hsah));
	memset(pBuf_cer_hsah, 0x00, sizeof(pBuf_cer_hsah));
	ret = SM3Hash(pBuf_tbs, nBuf_tbsSize, pBuf_tbs_hsah, nBuf_tbs_hsahSize);

	ret = SM3Hash(pBuf_cer, nBuf_cerSize, pBuf_cer_hsah, nBuf_cer_hsahSize);

	memcpy(&pBuf_tbs_hsah[32], pBuf_cer_hsah, 32);

//延签
	unsigned char *message = pBuf_tbs_hsah;
	unsigned char pk_comp[32];
	struct Certificate *pCert1 =
			payload->choice.signedData.signer.choice.certificateList.list.array[0];
	unsigned char pk_de[64];
	if (pCert1->tbs.subjectAttributes.verificationKey.key.present
			== ECCPoint_PR_compressed_y_0)
	{
		memcpy(pk_comp,
				pCert1->tbs.subjectAttributes.verificationKey.key.choice.compressed_y_0.buf,
				32);
		SM2PkDecompress(pk_de, pk_comp, 0);
	}
	if (pCert1->tbs.subjectAttributes.verificationKey.key.present
			== ECCPoint_PR_compressed_y_1)
	{
		memcpy(pk_comp,
				pCert1->tbs.subjectAttributes.verificationKey.key.choice.compressed_y_1.buf,
				32);
		SM2PkDecompress(pk_de, pk_comp, 1);
	}

	unsigned char sign_Info[64];
	if (payload->choice.signedData.sign.r.choice.x_only.buf != NULL)
	{
		memcpy(sign_Info, payload->choice.signedData.sign.r.choice.x_only.buf,
				payload->choice.signedData.sign.r.choice.x_only.size);
	}
	if (payload->choice.signedData.sign.s.buf != NULL)
	{
		memcpy(&sign_Info[32], payload->choice.signedData.sign.s.buf,
				payload->choice.signedData.sign.s.size);
	}
	ret = SM2VerifyMessage(message, 64, pk_de, sign_Info, 64);
	return ret;
}

int SecurityVerify(SecuredMessage_t *pSecMsg, int nTxAid)
{
//	SecurityResultCode_unknown	= 0,未知
	int ResultCode = SecurityResultCode_unknown;
//	SecurityResultCode_incorrectSecureMessageVersion	= 1,安全消息版本号信息非法
	if (pSecMsg->version != 2)
	{
		ResultCode = SecurityResultCode_incorrectSecureMessageVersion;
		goto VerifyEnd;
	}
//	SecurityResultCode_incorrectSignerType	= 2,签名者信息中签名方式非法
#if Test_Check_All
	if (pSecMsg->payload.present != Payload_PR_signedData)
	{
		ResultCode = SecurityResultCode_incorrectSignerType;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertVersion	= 3,数字证书版本号信息非法
#if Test_Check_All
	struct Certificate *pCert =
			pSecMsg->payload.choice.signedData.signer.choice.certificateList.list.array[0];
	if (pCert->version != 2)
	{
		ResultCode = SecurityResultCode_incorrectCertVersion;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertIssueDigest	= 4,数字证书签发者信息非法
#if Test_Check_All
	if (pCert->issuer.present == IssuerId_PR_certificateDigest)
	{
		if (pCert->issuer.choice.certificateDigest.algorithm
				== HashAlgorithm_sgdsm3)
		{
			if (pCert->issuer.choice.certificateDigest.digest.size == 8)
			{
				//做PCA的oer->hash 取最后8字节和携带证书的Digest对比,一致则认为正常.
//				printf("CertIssueDigest Ok\n");
			} else
			{
				ResultCode = SecurityResultCode_incorrectCertIssueDigest;
				goto VerifyEnd;
			}
		} else
		{
			ResultCode = SecurityResultCode_incorrectCertIssueDigest;
			goto VerifyEnd;
		}
	} else
	{
		ResultCode = SecurityResultCode_incorrectCertIssueDigest;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertSubjectInfo	= 5,数字证书中待签主题信息非法
#if Test_Check_All
	if (pCert->tbs.subjectInfo.subjectType == SubjectType_pseudonymTicket)
	{
//		主题类型为:pseudonymTicket;
//		主题名称参数长度合规,PCA生成的8字节随机数
		if (pCert->tbs.subjectInfo.subjectName.size == 8)
		{
//			printf("CertIssueDigest Ok\n");
		} else
		{
			ResultCode = SecurityResultCode_incorrectCertSubjectInfo;
			goto VerifyEnd;
		}
	} else if (pCert->tbs.subjectInfo.subjectType
			== SubjectType_authorizationTicket)
	{
//		主题类型为:authorizationTicket;
//		主题名称参数长度合规。
		if (pCert->tbs.subjectInfo.subjectName.size <= 32)
		{
//			printf("CertIssueDigest Ok\n");
		} else
		{
			ResultCode = SecurityResultCode_incorrectCertSubjectInfo;
			goto VerifyEnd;
		}
	} else
	{
		ResultCode = SecurityResultCode_incorrectCertSubjectInfo;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertSubjectAttribute	= 6,数字证书中待签主题属性信息非法
#if Test_Check_All
	if (pCert->tbs.subjectAttributes.verificationKey.curve == EccCurve_sgdsm2)
	{
		if (pCert->tbs.subjectInfo.subjectName.size > 5)
		{
//			printf("subjectName.size Ok\n");
		} else
		{
			ResultCode = SecurityResultCode_incorrectCertSubjectAttribute;
			goto VerifyEnd;
		}
	} else
	{
		ResultCode = SecurityResultCode_incorrectCertSubjectAttribute;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertValidityPeriod	= 7,数字证书中有效限定信息非法
#if Test_Check_All
	if (pCert->tbs.validityRestrictions.validityPeriod.present
			== ValidityPeriod_PR_timeStartAndEnd)
	{
//		printf("validityRestrictions.validityPeriod Ok\n");
	} else
	{
		ResultCode = SecurityResultCode_incorrectCertValidityPeriod;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectCertTimeStartAndEnd	= 8,数字证书中有效时间信息非法
#if Test_Check_All && 0

	Time32_t startValidity =
	pCert->tbs.validityRestrictions.validityPeriod.choice.timeStartAndEnd.startValidity;
	Time32_t endValidity =
	pCert->tbs.validityRestrictions.validityPeriod.choice.timeStartAndEnd.endValidity;
//时间大小不合法
	if (endValidity - startValidity > 0)
	{

	} else
	{
		ResultCode = SecurityResultCode_incorrectCertTimeStartAndEnd;
		goto VerifyEnd;
	}
	//判断证书有效期
	uint32_t nCurTime = 0;
	int nRet = GetTimeStamp2004_S(&nCurTime);
	if(nRet == 0);
	if ((nCurTime > startValidity) && (nCurTime < endValidity))
	{

	} else
	{
		ResultCode = SecurityResultCode_incorrectCertTimeStartAndEnd;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectSubcertAuthority	= 9,数字证书父子关系非法
//	SecurityResultCode_incorrectCertChain	= 10,证书链非法
//	SecurityResultCode_incorrectCertSignature	= 11,数字证书签名信息非法
#if Test_Check_All
	if (pCert->signature.curve == EccCurve_sgdsm2)
	{
		//延签,使用证书的公钥+签名原文延签.判断签名值是否正常
		int nCertVer = 0;
		uint8_t digestbuf[8] = {0};
		if (pCert->issuer.choice.certificateDigest.digest.buf != NULL)
			memcpy(digestbuf, pCert->issuer.choice.certificateDigest.digest.buf,
					pCert->issuer.choice.certificateDigest.digest.size);
		char pPCAHashName[100];
		sprintf(pPCAHashName,
				"/usr/local/config/Cert/%02X%02X%02X%02X%02X%02X%02X%02X.cert",
				digestbuf[0], digestbuf[1], digestbuf[2], digestbuf[3],
				digestbuf[4], digestbuf[5], digestbuf[6], digestbuf[7]);
		if ((access(pPCAHashName, F_OK)) != -1)
		{
			nCertVer = Certificate_Verify(pPCAHashName, pCert);
		} else
		{
			ResultCode = SecurityResultCode_incorrectCertSignature;
			goto VerifyEnd;
		}
		if (nCertVer != 0)
		{
			ResultCode = SecurityResultCode_incorrectCertSignature;
			goto VerifyEnd;
		}
	} else
	{
		ResultCode = SecurityResultCode_incorrectCertSignature;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectTbsDataGenTime	= 12,待签数据中数据产生时间信息非法
#if Test_Check_All && 0
	if (pSecMsg->payload.choice.signedData.tbs.headerInfo.genTime != NULL)
	{
		uint64_t lTimeStamp2 = 0;
		uint64_t lTimeStamp3 = 0;
		GetTimeStamp2(&lTimeStamp2);
		GetTimeStamp3(pSecMsg->payload.choice.signedData.tbs.headerInfo.genTime,
				&lTimeStamp3);
		printf("lTimeStamp2 = %llu\n", lTimeStamp2);
		printf("lTimeStamp3 = %llu\n", lTimeStamp3);
		if (abs(lTimeStamp3 - lTimeStamp2) <= 60 * 60 * 24)
		{
			printf("TbsDataGenTime ok\n");
		} else
		{
			ResultCode = SecurityResultCode_incorrectTbsDataGenTime;
			goto VerifyEnd;
		}
	} else
	{
		ResultCode = SecurityResultCode_incorrectTbsDataGenTime;
		goto VerifyEnd;
	}
#endif
//	SecurityResultCode_incorrectTbsDataHashAlg	= 13,待签数据中杂凑算法信息非法
	if (pSecMsg->payload.choice.signedData.tbs.headerInfo.hashAlg != NULL)
	{
		if (*pSecMsg->payload.choice.signedData.tbs.headerInfo.hashAlg
				!= HashAlgorithm_sgdsm3)
		{
			ResultCode = SecurityResultCode_incorrectTbsDataHashAlg;
			goto VerifyEnd;
		}
	}
//	SecurityResultCode_incorrectTbsDataItsAid	= 14,待签数据中 AID 信息非法
	if (pSecMsg->payload.choice.signedData.tbs.headerInfo.itsAid != nTxAid)
	{
		ResultCode = SecurityResultCode_incorrectTbsDataItsAid;
		goto VerifyEnd;
	}
//	SecurityResultCode_incorrectSignedMessageSignature	= 15安全消息中签名信息非法
	int ret = SecuredMessage_Verify(pSecMsg);
	if (ret != 0)
	{
		ResultCode = SecurityResultCode_incorrectSignedMessageSignature;
		goto VerifyEnd;
	}

	VerifyEnd: return ResultCode;
}
#endif
int SecurityVerify_SecBuf(uint8_t SecBuf[], int SecBufLen, int nTxAid)
{
#if SECMODE_DMD31_DT
	int nRet = 0;
	SecuredMessage_t *pSecMsg = ALLOC_OBJ(SecuredMessage_t, 1);
	asn_dec_rval_t Rval;
	Rval = Sec_OerDecode(pSecMsg, (char *) SecBuf, SecBufLen);
	if (Rval.code != RC_OK)
	{
		printf("@@@sec_oer_decode() failed!\n");
		printf("###sec_Broken decoding at byte %ld\n", (long) Rval.consumed);
		ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecMsg);
		return -1;
	}
	nRet = SecurityVerify(pSecMsg, nTxAid);
//	nRet = 0;
	ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecMsg);
	return nRet;
#endif

#if SECMODE_DMD31_IWALL_SOFT | SECMODE_AG15_IWALL_SOFT |SECMODE_AG15_IWALL_HARD
	if (SecBuf[0] != 0x02)
	{
		return 1;
	}
	int nBSMDataLen = 1024;
	uint8_t pbBSMData[1024] =
	{	0};
	uint64_t AID = 0;
	int nRet = IWALL_VerifySecuredMessage(SecBuf, SecBufLen, pbBSMData, &nBSMDataLen, &AID);
	printf("IWALL_VerifySecuredMessage nRet = %d \n", nRet);
	if (0 != nRet)
	{
		printf("^^^^IWALL_VerifySecuredMessage failed, nRet = %d \n", nRet);
	}
	return nRet;
#endif
}
//延签调用借口
//	int checkResult = SecurityVerify(pSecMsg, nTxAid);
