/*
 * FunCommun.c
 *
 *  Created on: Aug 11, 2019
 *      Author: wanji
 */
#include "FunCommun.h"

tQueue_RevLst g_Queue_SecRecv;
//
asn_enc_rval_t Sec_OerEncode(SecuredMessage_t *pRecvMsg, char *pBuf,
		size_t Size)
{
	asn_enc_rval_t EncRVal; // Encoder return value

	//Encode the MessageFrame type as UPER
	EncRVal = oer_encode_to_buffer(&asn_DEF_SecuredMessage, NULL, pRecvMsg, pBuf,
			Size);

	return EncRVal;
}

asn_dec_rval_t Sec_OerDecode(SecuredMessage_t *pRecvMsg, char *pBuf,
		size_t Size)
{
	asn_dec_rval_t RVal;

	// Decode the provided buffer as BasicSafetyMessage type
	RVal = oer_decode(0, &asn_DEF_SecuredMessage, (void **) &pRecvMsg, pBuf,
			Size);

	return RVal;
}
int GetTimeStamp2004_US(uint64_t *lTimeStamp)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64_t ltv_sec = tv.tv_sec;
	uint64_t ltv_usec = tv.tv_usec;
	if (ltv_sec > 1072886400)
	{
		ltv_sec -= 1072886400;
	} else
	{
		ltv_sec = 0;
		ltv_usec = 0;
	}
	*lTimeStamp = (uint64_t) (ltv_sec * 1000000 + (uint64_t) (ltv_usec));
	if (ltv_sec == 0)
		return -1;
	else
		return 0;
}
int GetTimeStamp2004_S(uint32_t *lTimeStamp)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64_t ltv_sec = tv.tv_sec;
	if (ltv_sec > 1072886400)
	{
		ltv_sec -= 1072886400;
	} else
	{
		ltv_sec = 0;
	}

	*lTimeStamp = (uint32_t) ltv_sec;

	if (ltv_sec == 0)
		return -1;
	else
		return 0;
}

int GetTimeStamp_s_fmGenTime(Time64_t *genTime, uint64_t *lTimeStamp)
{
	uint64_t ltv_Time = 0;
	int i = 0;
	for (i = 0; i <= 7; i++)
	{
		ltv_Time = (ltv_Time * 256 + genTime->buf[i]);
		printf("genTime->buf[%d] = %02X ltv_Time = %llu\n", i, genTime->buf[i],
				ltv_Time);
	}
	*lTimeStamp = ltv_Time / 1000000;
	printf("GetTimeStamp_s_fmGenTime = %llu\n", *lTimeStamp);
	return 0;
}
int SetGenTime(Time64_t *genTime, uint64_t lTimeStamp)
{
	genTime->buf[0] = ((lTimeStamp >> 56) & 0xFF);
	genTime->buf[1] = ((lTimeStamp >> 48) & 0xFF);
	genTime->buf[2] = ((lTimeStamp >> 40) & 0xFF);
	genTime->buf[3] = ((lTimeStamp >> 32) & 0xFF);
	genTime->buf[4] = ((lTimeStamp >> 24) & 0xFF);
	genTime->buf[5] = ((lTimeStamp >> 16) & 0xFF);
	genTime->buf[6] = ((lTimeStamp >> 8) & 0xFF);
	genTime->buf[7] = (lTimeStamp & 0x7F);
	return 0;
}
int print_Info(char print_Info[], unsigned char buf[], int buflen)
{
#if SECLAYER_PRINTLOG
	printf("%s Size = %d\n", print_Info, buflen);
	int nTmpi = 0;
	for (nTmpi = 0; nTmpi < buflen; nTmpi++)
	{
		if ((nTmpi != 0) && (nTmpi % 32 == 0))
		printf("\n");
		printf("%02X ", 0xFF & buf[nTmpi]);
	}
	printf("\n");
#endif
	return 0;
}
int Create_Certificate_fromFile(Certificate_t *pCertificate, char filename[])
{
	FILE *fp = NULL;
	uint8_t pBuf[FileMaxSize] ;
	memset(pBuf, 0x00, sizeof(pBuf));
	fp = fopen(filename,"rb");
	int readFileSize = fread(&pBuf, sizeof(char), FileMaxSize, fp);
	fclose(fp);
#if SECLAYER_PRINTLOG
	int i = 0;
	printf("Size = %d\n",readFileSize);
	for(i = 0 ;i < readFileSize; i++)
	{
		printf("%02X ",0xFF & pBuf[i]);
	}
	printf("\n");
#endif
	asn_dec_rval_t Rval;
	Rval = oer_decode(0, &asn_DEF_Certificate, (void **) &pCertificate, pBuf,
			readFileSize);
	if (Rval.code != RC_OK)
	{
		printf("AppInfo_Process_uper_decode() failed!\n");
		printf("AppInfo_Process_Broken encoding at byte %ld\n",
				(long) Rval.consumed);
	} else
	{
		//uper_decode Success
#if SECLAYER_PRINTLOG
		printf("CallBack Information:\n");
		printf("uper_decode() Success!\n");
		xer_fprint(stdout, &asn_DEF_Certificate, pCertificate);
		printf("\n");
#endif
	}
	return 0;
}
int ReadFile(char filename[], unsigned char buf[], int *buflen)
{
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	int readFileSize = fread(buf, sizeof(char), FileMaxSize, fp);
	fclose(fp);
	fp = NULL;

	*buflen = readFileSize;
	print_Info(filename, buf, *buflen);
	return 0;
}
int Trans_Asic2Hex(unsigned char Asicbuf[], int lAsicbuflen)
{
	print_Info("ASIC_Buf", Asicbuf, lAsicbuflen);
	int i = 0;
	for (i = 0; i < lAsicbuflen; i++)
	{
		if ((Asicbuf[i] >= '0') && (Asicbuf[i] <= '9'))
			Asicbuf[i] -= '0';
		if ((Asicbuf[i] >= 'a') && (Asicbuf[i] <= 'f'))
		{
			Asicbuf[i] -= 'a';
			Asicbuf[i] += 0x0A;
		}
		if ((Asicbuf[i] >= 'A') && (Asicbuf[i] <= 'F'))
		{
			Asicbuf[i] -= 'A';
			Asicbuf[i] += 0x0A;
		}
	}
	print_Info("sk2", Asicbuf, lAsicbuflen);
	for (i = 0; i < lAsicbuflen / 2; i++)
	{
		Asicbuf[i] = ((Asicbuf[i * 2] << 4) & 0xF0)
				| (Asicbuf[i * 2 + 1] & 0x0F);
	}
	lAsicbuflen /= 2;
	print_Info("sk3", Asicbuf, lAsicbuflen);
	return lAsicbuflen;
}
