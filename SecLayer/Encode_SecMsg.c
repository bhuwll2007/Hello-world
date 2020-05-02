#define __EXTENSIONS__
#include "Encode_SecMsg.h"
#include "IWALL_V2X_API.h"
#define ALLOC_OBJ(type, size) \
  (type *)calloc(1, (sizeof(type)*size))

int GetCertificate_fromFile(Certificate_t *pCertificate, uint8_t filename[])
{
	uint8_t *pBuf = ALLOC_OBJ(uint8_t, FileMaxSize);
	int readFileSize = 0;
	ReadFile((nint8_t*)filename, pBuf, &readFileSize);

	asn_dec_rval_t Rval;
	Rval = oer_decode(0, &asn_DEF_Certificate, (void **) &pCertificate, pBuf,
			readFileSize);
	if (Rval.code != RC_OK)
	{
		printf("oer_decode() failed!\n");
		printf("oer_decode encoding at byte %ld\n", (long) Rval.consumed);
	} else
	{
		//uper_decode Success
#if SECLAYER_PRINTLOG
		printf("oer_decode() Success!\n");
		asn_fprint(stdout, &asn_DEF_Certificate, pCertificate);
#endif
	}
	free(pBuf);
	pBuf = NULL;
	return 0;
}
#if SECMODE_DMD31_DT
int ReadCertandSk(uint8_t pPCHashName[], uint8_t pcSkey[])
{
	uint32_t lTimeStamp1 = 0;
	GetTimeStamp2004_S(&lTimeStamp1);
	int KeyIdIndex = ((lTimeStamp1 / 60) % 60) / 5;
	if ((KeyIdIndex >=20) || (KeyIdIndex < 0))
	{
		KeyIdIndex = 0;
	}
	char acIwallFileName[100] = "/usr/local/config/IwallConfig.ini";
	char pConfigString[100];
	char KeyIdKeyName[100];
	char KeyIdPcHashItem[100];
	char KeyIdPcHashName[100];
	char acPrivateKey[20] = "PrivateKey";
	sprintf(KeyIdKeyName, "%d_verKey", KeyIdIndex + 1);
	sprintf(KeyIdPcHashItem, "Cert_%d_hash", KeyIdIndex + 1);
	if (GetConfigStringValue(acIwallFileName, acPrivateKey, KeyIdKeyName,
			pConfigString) == 0)
	{
		memcpy(pcSkey, pConfigString, strlen(pConfigString));
		Trans_Asic2Hex(pcSkey, strlen(pConfigString) - 1);
	}

	if (GetConfigStringValue(acIwallFileName, "PA", KeyIdPcHashItem,
			pConfigString) == 0)
	{
		memcpy(KeyIdPcHashName, pConfigString, strlen(pConfigString));
		KeyIdPcHashName[strlen(pConfigString) - 1] = 0x00;
		sprintf((nint8_t*)pPCHashName, "/usr/local/config/Cert/%s.cert",
				KeyIdPcHashName);
	}
	return 0;
}
int Encode_SecuredMessage(int SubjectType, int CAOrgan, int nAid,
		uint8_t *signingData, int signingDatalen,
		unsigned char *pbSecuredMessage, int *nSecuredMessageLen)
{
	uint8_t acPcFileName[100];
	uint8_t acSKeyValue[100];
	int nRet = ReadCertandSk(acPcFileName,acSKeyValue);
	if(nRet != 0)
	{
		printf("ReadCertandSk failed ! nRet = %d\n",nRet);
		return -1;
	}
	else
	{
//		printf("acPcFileName = %s\n",acPcFileName);
	}
	SecuredMessage_t *pSecuredMessage = ALLOC_OBJ(SecuredMessage_t, 1);
	asn_enc_rval_t EncRVal;
	int nTmpi;
	int res;
	//版本	Uint8_t	 version;
	pSecuredMessage->version = 2;

	//负载信息	Payload_t	 payload;
	Payload_t* payload = &(pSecuredMessage->payload);
	payload->present = Payload_PR_signedData;
	//证书字段	SignerInfo_t	 signer;
	payload->choice.signedData.signer.present = SignerInfo_PR_certificateList;

	Certificate_t *lCertificate = ALLOC_OBJ(Certificate_t, 1);
	GetCertificate_fromFile(lCertificate, acPcFileName);
	res = ASN_SEQUENCE_ADD(
			&payload->choice.signedData.signer.choice.certificateList,
			lCertificate);
	assert(res == 0);
	payload->choice.signedData.tbs.headerInfo.itsAid = nAid;
	//	HashAlgorithm_t	*hashAlg	/* OPTIONAL */;
	payload->choice.signedData.tbs.headerInfo.hashAlg = ALLOC_OBJ(
			HashAlgorithm_t, 1);

	//	Time64_t	*genTime	/* OPTIONAL */;
	*(payload->choice.signedData.tbs.headerInfo.hashAlg) = HashAlgorithm_sgdsm3;
	payload->choice.signedData.tbs.headerInfo.genTime = ALLOC_OBJ(Time64_t, 1);
	payload->choice.signedData.tbs.headerInfo.genTime->buf = ALLOC_OBJ(uint8_t,
			8);
	payload->choice.signedData.tbs.headerInfo.genTime->size = 8;
	uint64_t lTimeStamp = 0;
	GetTimeStamp2004_US(&lTimeStamp);
	SetGenTime(payload->choice.signedData.tbs.headerInfo.genTime, lTimeStamp);
	//	Time64_t	*expiryTime	/* OPTIONAL */;
	payload->choice.signedData.tbs.headerInfo.expiryTime = NULL;
	//	struct ThreeDLocation	*location	/* OPTIONAL */;
	payload->choice.signedData.tbs.headerInfo.location = NULL;
	//	HashedId3_t	*digest	/* OPTIONAL */;
	payload->choice.signedData.tbs.headerInfo.digest = NULL;
	//	struct PublicEncryptionKey	*encKey	/* OPTIONAL */;
	payload->choice.signedData.tbs.headerInfo.encKey = NULL;

//	OCTET_STRING_t	*data	/* OPTIONAL */;
//	payload->choice.signedData.tbs.data = NULL;
	payload->choice.signedData.tbs.data = ALLOC_OBJ(OCTET_STRING_t, 1);
	payload->choice.signedData.tbs.data->buf = ALLOC_OBJ(uint8_t,
			signingDatalen);
	payload->choice.signedData.tbs.data->size = signingDatalen;
	memcpy(payload->choice.signedData.tbs.data->buf, &signingData[0],
			signingDatalen);

//	OCTET_STRING_t	*extHash	/* OPTIONAL */;
	payload->choice.signedData.tbs.extHash = NULL;

	//	Signature_t	 sign;
	//	EccCurve_t	 curve;
	payload->choice.signedData.sign.curve = EccCurve_sgdsm2;
	//	ECCPoint_t	 r;
	payload->choice.signedData.sign.r.present = ECCPoint_PR_x_only;
	payload->choice.signedData.sign.r.choice.x_only.buf = ALLOC_OBJ(uint8_t,
			32);
	payload->choice.signedData.sign.r.choice.x_only.size = 32;
//	payload->choice.signedData.sign.r.present = ECCPoint_PR_fill;

	//	OCTET_STRING_t	 s;
	payload->choice.signedData.sign.s.buf = ALLOC_OBJ(uint8_t, 32);
	payload->choice.signedData.sign.s.size = 32;

	TBSData_t *ptbs = &payload->choice.signedData.tbs;
	unsigned char pBuf_tbs[1000];
	int nBuf_tbsSize = 1000;

	EncRVal = oer_encode_to_buffer(&asn_DEF_TBSData, NULL, ptbs, pBuf_tbs,
			nBuf_tbsSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
	} else
	{
		nBuf_tbsSize = EncRVal.encoded;
	}

	unsigned char pBuf_cer[1000];
	int nBuf_cerSize = 1000;

	Certificate_t *pCert =
			payload->choice.signedData.signer.choice.certificateList.list.array[0];
	EncRVal = oer_encode_to_buffer(&asn_DEF_Certificate, NULL, pCert, pBuf_cer,
			nBuf_cerSize);
	if (EncRVal.encoded == -1)
	{
		printf("oer_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
	} else
	{
		nBuf_cerSize = EncRVal.encoded;
	}

	//计算hash值
	unsigned char pBuf_tbs_hsah[1000];
	int nBuf_tbs_hsahSize = 1000;
	unsigned char pBuf_cer_hsah[1000];
	int nBuf_cer_hsahSize = 1000;
	memset(pBuf_tbs_hsah, 0x00, 1000);
	memset(pBuf_cer_hsah, 0x00, 1000);
	int ret = SM3Hash(pBuf_tbs, nBuf_tbsSize, pBuf_tbs_hsah, nBuf_tbs_hsahSize);
	if (ret == 0)
		;
	ret = SM3Hash(pBuf_cer, nBuf_cerSize, pBuf_cer_hsah, nBuf_cer_hsahSize);
	if (ret == 0)
		;
	memcpy(&pBuf_tbs_hsah[32], pBuf_cer_hsah, 32);

	//读取私钥
	unsigned char sk[32];
	int skLen = 32;
	memcpy(sk, acSKeyValue, skLen);
	//签名
	unsigned char sign[100];
	int sign_len;
	ret = SM2SignMessage(pBuf_tbs_hsah, 64, sk, sign, &sign_len);

	//赋值
	for (nTmpi = 0; nTmpi < 32; nTmpi++)
		payload->choice.signedData.sign.r.choice.x_only.buf[nTmpi] =
				sign[nTmpi];

	for (nTmpi = 0; nTmpi < 32; nTmpi++)
		payload->choice.signedData.sign.s.buf[nTmpi] = sign[nTmpi + 32];
	memcpy(payload->choice.signedData.sign.r.choice.x_only.buf, sign, 32);
	memcpy(payload->choice.signedData.sign.s.buf, &sign[32], 32);
#if SECLAYER_PRINTLOG
	asn_fprint(stdout, &asn_DEF_SecuredMessage, pSecuredMessage);
	printf("\n");
#endif

	//OER打包
	EncRVal = Sec_OerEncode(pSecuredMessage, (char *)pbSecuredMessage, *nSecuredMessageLen);
	if (EncRVal.encoded == -1)
	{
		printf("Sec_OerEncode() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);
	} else
	{
		*nSecuredMessageLen = EncRVal.encoded;
		print_Info("SecuredMessage Sec_OerEncode", pbSecuredMessage,
				*nSecuredMessageLen);
	}
	ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecuredMessage);
	return 0;
}
#endif

#if SECMODE_DMD31_IWALL_SOFT | SECMODE_AG15_IWALL_SOFT |SECMODE_AG15_IWALL_HARD
int Encode_SecuredMessage(int SubjectType, int CAOrgan, int nAid,
		uint8_t *signingData, int signingDatalen,
		unsigned char *pbSecuredMessage, int *nSecuredMessageLen)
{
	int nRet = 0;
	int nSecuredMsgLen = 1024;
	uint8_t pbSecuredMsg[1024] =
	{	0};
// 1、生成签名消息
	nRet = IWALL_SignSecuredMessage(nAid, signingData, signingDatalen,
			pbSecuredMsg, &nSecuredMsgLen);
	if (0 != nRet)
	{
		printf("IWALL_SignSecuredMessage failed, nRet = %d \n", nRet);
		return -1;
	}
	printf("IWALL_SignSecuredMessage success, AID = %d \n", nAid);
	memcpy(pbSecuredMessage, pbSecuredMsg, nSecuredMsgLen);
	*nSecuredMessageLen = nSecuredMsgLen;
#if SECLAYER_PRINTLOG & 0
	//打印一下
	print_Info("print", pbSecuredMsg, nSecuredMsgLen);
	asn_dec_rval_t Rval;
	SecuredMessage_t *pSecuredMessage = ALLOC_OBJ(SecuredMessage_t, 1);
	Rval = Sec_OerDecode(pSecuredMessage, (nint8_t*) pbSecuredMsg,
			nSecuredMsgLen);
	if (Rval.code != RC_OK)
	{
		printf("sec_uper_decode() failed!\n");
		printf("sec_Broken decoding at byte %ld\n", (long) Rval.consumed);
	}
	asn_fprint(stdout, &asn_DEF_SecuredMessage, pSecuredMessage);
	printf("\n");
	ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecuredMessage);
#endif
	return 0;
}
#endif

