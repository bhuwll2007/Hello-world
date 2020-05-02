/*
 * ExtFunction.c
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */
#include "ExtFunction.h"

//tStateStore Store;

int MessageFrame_Memcpy(MessageFrame_t *psrcMsg, MessageFrame_t *pdesMsg)
{
	int BufLen = 1024;
	char pBuf[1024];

	//编码
	asn_enc_rval_t EncRVal; // Encoder return value
	EncRVal = Msg_UPEREncode_ToBuf(psrcMsg, pBuf, BufLen);

	if (EncRVal.encoded == -1)
	{
		printf("uper_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);

		printf("------------ MessageFrame_Memcpy ------------\n");
		xer_fprint(stdout, &asn_DEF_MessageFrame, psrcMsg);
		printf("------------ MessageFrame_Memcpy End --------\n");
		return -1;
	}
	//计算编码后数据长度
	BufLen = (EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0));

	//解码
	asn_dec_rval_t Rval;
	Rval = Msg_UPERDecode(pdesMsg, pBuf, BufLen);

	if (Rval.code != RC_OK)
	{
		return -2;
	}
	return 0;
}

int MessageFrame_Memcpy1(MessageFrame_t *psrcMsg, char pBuf[], int *BufLen1)
{
	int BufLen = 1024;
	//char pBuf[1024];

	//编码
	asn_enc_rval_t EncRVal; // Encoder return value
	EncRVal = Msg_UPEREncode_ToBuf(psrcMsg, pBuf, BufLen);

	if (EncRVal.encoded == -1)
	{
		printf("uper_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);

		printf("------------ MessageFrame_Memcpy ------------\n");
		xer_fprint(stdout, &asn_DEF_MessageFrame, psrcMsg);
		printf("------------ MessageFrame_Memcpy End --------\n");
		return -1;
	}
	//计算编码后数据长度
	BufLen = (EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0));
	*BufLen1 = BufLen;
	return 0;
	//解码
	//asn_dec_rval_t Rval;
	//Rval = Msg_UPERDecode(pdesMsg, pBuf, BufLen);

	//if (Rval.code != RC_OK)
	{
	//	return -2;
	}
	return 0;
}

asn_enc_rval_t Msg_UPEREncode_ToBuf(MessageFrame_t *pSendMsg, char *pBuf,
		size_t Size)
{

	asn_enc_rval_t EncRVal; // Encoder return value

	//Encode the MessageFrame type as UPER
	EncRVal = uper_encode_to_buffer(&asn_DEF_MessageFrame, NULL, pSendMsg, pBuf,
			Size);

	return EncRVal;
}

asn_dec_rval_t Msg_UPERDecode(MessageFrame_t *pRecvMsg, char *pBuf, size_t Size)
{

	asn_dec_rval_t RVal;

	// Decode the provided buffer as BasicSafetyMessage type

	RVal = uper_decode(0, &asn_DEF_MessageFrame, (void **) &pRecvMsg, pBuf,
			Size, 0, 0);

	return RVal;
}

int ExtNet_CallbackRegister(
		void (*pFunRecvData)(char *p_cRecvBuf, uint32_t pnRecvBufSize),
		char p_acIp[], int p_nPort, StcTcpClient **lStcTcpClient)
{

	int Res;

	//Init Tcp Client Socket
	Res = TcpClt_InitSoc(*lStcTcpClient);

	if (Res < 0)
	{
		Res = Tcp_ERR_Socket;
		goto Error;
	}

	//Start Link and Create Recv pthread
	Res = TcpClt_LinkSer(*lStcTcpClient, p_acIp, p_nPort);
	if (Res < 0)
	{
		Res = Tcp_ERR_connect;
		goto Error;
	}

	sleep(1);

	//Get CallBack Function Pointer
	(*lStcTcpClient)->pCallBackDealData = pFunRecvData;

	Error: return Res;
}

void sFunRecvData(char *p_cRecvBuf, int *pnRecvBufSize)
{
	int i = 0;
	printf("Recv Data Sucess!!!\n");
	if (*pnRecvBufSize > 0)
	{
		printf("Recv %d Bytes.\n", *pnRecvBufSize);
		for (i = 0; i < *pnRecvBufSize; i++)
		{
			printf("%02X ", p_cRecvBuf[i]);
		}
		printf("\n");
	}
}

void HostInfo_LoadConfig(tStateStore *pStore, char *pConfigFileName)
{
	int ConfigVal = 0;
	char pConfigString[100] =
	{ 0 };
	char szHostIp[30];
	char cCfgFileName[50];

	//Get WAVE Set Information
	if (!(GetConfigIntValue(pConfigFileName, "WaveConfig", "OptionIndicator",
			&ConfigVal)))
		pStore->WaveConfig.OptionIndicator = ConfigVal;

	if (!(GetConfigIntValue(pConfigFileName, "WaveConfig", "AID", &ConfigVal)))
		pStore->WaveConfig.AID = ConfigVal;

	//Get Host Equipment Attribute Information
	if (!(GetConfigStringValue(pConfigFileName, "HostAttriInfo", "Host_ID",
			pConfigString)))
		memcpy(&pStore->HostAttriInfo.ID[0], pConfigString, sizeof(char) * 8);

	if (!(GetConfigIntValue(pConfigFileName, "HostAttriInfo", "Host_Type",
			&ConfigVal)))
		pStore->HostAttriInfo.present = ConfigVal;

	//Get the OBU Vehicle information
	if (ConfigVal == 0)
	{
		if (!(GetConfigIntValue(pConfigFileName, "HostAttriInfo", "Veh_Length",
				&ConfigVal)))
			pStore->HostAttriInfo.choice.OBUInfo.VehLenght = ConfigVal;

		if (!(GetConfigIntValue(pConfigFileName, "HostAttriInfo", "Veh_Width",
				&ConfigVal)))
			pStore->HostAttriInfo.choice.OBUInfo.VehWidth = ConfigVal;

		if (!(GetConfigIntValue(pConfigFileName, "HostAttriInfo", "Veh_Height",
				&ConfigVal)))
			pStore->HostAttriInfo.choice.OBUInfo.VehHeight = ConfigVal;

		if (!(GetConfigIntValue(pConfigFileName, "HostAttriInfo",
				"Veh_Classification", &ConfigVal)))
			pStore->HostAttriInfo.choice.OBUInfo.VehClassification = ConfigVal;
	}

	//Get Rx Enable Setting
	if (!(GetConfigIntValue(pConfigFileName, "HostComInfo", "RxEnable",
			&ConfigVal)))
		pStore->HostComInfo.RxEnable = ConfigVal;

	if (!(GetConfigStringValue(pConfigFileName, "HostComInfo", "RxAidList",
			pConfigString)))
		memcpy(pStore->HostComInfo.RxAidLst, pConfigString,
				strlen(pConfigString));

	//Get BSM Tx Setting
	if (!(GetConfigIntValue(pConfigFileName, "BSMTx", "TxEnable", &ConfigVal)))
		pStore->HostComInfo.BsmTx.TxEnable = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "BSMTx", "TxInterval", &ConfigVal)))
		pStore->HostComInfo.BsmTx.TxInterval = ConfigVal;

	//Get Rsi Tx Setting
	if (!(GetConfigIntValue(pConfigFileName, "RSITx", "TxEnable", &ConfigVal)))
		pStore->HostComInfo.RsiTx.TxEnable = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "RSITx", "TxInterval", &ConfigVal)))
		pStore->HostComInfo.RsiTx.TxInterval = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "RSITx", "RSIMsgCnt", &ConfigVal)))
		pStore->HostComInfo.RsiTx.RsiCnt = ConfigVal;

	int nTmpi = 0;
	char CfgPath[50];
	if(pStore->HostComInfo.RsiTx.TxEnable == 1)
	{
		for (nTmpi = 0; nTmpi < pStore->HostComInfo.RsiTx.RsiCnt; nTmpi++)
		{
			memset(CfgPath, 0x00, sizeof(CfgPath));

			sprintf(CfgPath, "CfgPath%d", nTmpi);
			if (!(GetConfigStringValue(pConfigFileName, "RSITx", CfgPath,
					cCfgFileName)))
			{
				memcpy(pStore->HostComInfo.RsiTx.CfgFileName[nTmpi], cCfgFileName,
						strlen(cCfgFileName));
				printf("pStore->HostComInfo.RsiTx.CfgFileName[%d]= %s\n",nTmpi,pStore->HostComInfo.RsiTx.CfgFileName[nTmpi]);
			}
		}
	}
	else
	{
		pStore->HostComInfo.RsiTx.RsiCnt = 0;
	}

	//Get RSM Tx Setting
	if (!(GetConfigIntValue(pConfigFileName, "RSMTx", "TxEnable", &ConfigVal)))
		pStore->HostComInfo.RsmTx.TxEnable = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "RSMTx", "TxInterval", &ConfigVal)))
		pStore->HostComInfo.RsmTx.TxInterval = ConfigVal;

	if (!(GetConfigStringValue(pConfigFileName, "RSMTx", "ServerIp", szHostIp)))
		memcpy(pStore->HostComInfo.RsmTx.ServerIp, szHostIp, strlen(szHostIp));

	if (!(GetConfigIntValue(pConfigFileName, "RSMTx", "ServerPort", &ConfigVal)))
		pStore->HostComInfo.RsmTx.ServerPort = ConfigVal;

	//Get Map Tx Setting
	if (!(GetConfigIntValue(pConfigFileName, "MAPTx", "TxEnable", &ConfigVal)))
		pStore->HostComInfo.MapTx.TxEnable = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "MAPTx", "TxInterval", &ConfigVal)))
		pStore->HostComInfo.MapTx.TxInterval = ConfigVal;
	if (!(GetConfigStringValue(pConfigFileName, "MAPTx", "CfgPath",
			cCfgFileName)))
		memcpy(pStore->HostComInfo.MapTx.CfgFileName, cCfgFileName,
				strlen(cCfgFileName));

	//Get Spat Tx Setting
	if (!(GetConfigIntValue(pConfigFileName, "SPATTx", "TxEnable", &ConfigVal)))
		pStore->HostComInfo.SpatTx.TxEnable = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "SPATTx", "TxInterval", &ConfigVal)))
		pStore->HostComInfo.SpatTx.TxInterval = ConfigVal;
	if (!(GetConfigStringValue(pConfigFileName, "SPATTx", "ServerIp", szHostIp)))
		memcpy(pStore->HostComInfo.SpatTx.ServerIp, szHostIp, strlen(szHostIp));

	if (!(GetConfigIntValue(pConfigFileName, "SPATTx", "ServerPort", &ConfigVal)))
		pStore->HostComInfo.SpatTx.ServerPort = ConfigVal;

	//Get Can Data From
	if (!(GetConfigIntValue(pConfigFileName, "CANDATA", "CanFrom", &ConfigVal)))
		pStore->CANFlag = ConfigVal;

//	//Get DSA Tx Setting
//	if (!(GetConfigIntValue(pConfigFileName, "DSATx", "TxEnable", &ConfigVal)))
//		pStore->HostComInfo.m_DsaTx.TxEnable = ConfigVal;
//	if (!(GetConfigIntValue(pConfigFileName, "DSATx", "TxInterval", &ConfigVal)))
//		pStore->HostComInfo.m_DsaTx.TxInterval = ConfigVal;
//	if (!(GetConfigIntValue(pConfigFileName, "DSATx", "OptionIndicator",
//			&ConfigVal)))
//		pStore->HostComInfo.m_DsaTx.OptionIndicator = ConfigVal;
//	if (!(GetConfigIntValue(pConfigFileName, "DSATx", "HdrExtCnt", &ConfigVal)))
//		pStore->HostComInfo.m_DsaTx.HdrExtCnt = ConfigVal;
//	if (!(GetConfigStringValue(pConfigFileName, "DSATx", "ElmtId",
//			pConfigString)))
//		memcpy(pStore->HostComInfo.m_DsaTx.ElmtId, pConfigString,
//				strlen(pConfigString));

	//初始化
	pthread_mutex_init(&mutex_StoreBSM, NULL);
	pthread_mutex_init(&mutex_StoreRSM, NULL);
	pthread_mutex_init(&mutex_StoreRSI, NULL);
	pthread_mutex_init(&mutex_StoreMAP, NULL);
	pthread_mutex_init(&mutex_StoreSPAT, NULL);
}

void SimCANInfo_LoadCfg(CanData *pCanData, char *pConfigFileName)
{
	int ConfigVal = 0;
	if (!(GetConfigIntValue(pConfigFileName, "CANDATA", "Light", &ConfigVal)))
		pCanData->ExteriorLights = ConfigVal;

	if (!(GetConfigIntValue(pConfigFileName, "CANDATA", "Break", &ConfigVal)))
		pCanData->Breaks.BrakePedalStatus = ConfigVal;

	if (!(GetConfigIntValue(pConfigFileName, "CANDATA", "AuxBreakSys",
			&ConfigVal)))
		pCanData->Breaks.AuxiliaryBrakeStatus = ConfigVal;
}

long GetDSecond()
{

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	uint32_t TimeS = (int) tv.tv_sec;
	uint32_t TimeuS = (int) tv.tv_usec;

	uint32_t time_S = TimeS - (TimeS / 100) * 100;
	if (time_S > 59)
		time_S = time_S - 60;
	uint32_t time_ms = TimeuS / 1000;

	return (long) (1000 * time_S + time_ms);
}

//void msTimeDelay(struct timeval delay, int usTxInterval)
//{
//	delay.tv_sec = 0;
//	delay.tv_usec = usTxInterval * 1000;
//	select(0, NULL, NULL, NULL, &delay);
//}

void WriteXml(MessageFrame_t *pSendMsg, char *pConfigFileName)
{
	FILE *stream = fopen(pConfigFileName, "wb+");

	xer_fprint(stream, &asn_DEF_MessageFrame, pSendMsg);
	fclose(stream);
}

void ReadXml(MessageFrame_t **pSendMsg, char *pConfigFileName)
{
	char *xer_buf = NULL;
	int fsize = 0;
	FILE *pxerfp;

	pxerfp = fopen(pConfigFileName, "r");

	if(pxerfp == NULL)
	{
		return ;
	}
	//Read all bytes in RSI config file
	fseek(pxerfp, 0, SEEK_END);
	fsize = ftell(pxerfp);

	xer_buf = calloc(1, fsize);

	rewind(pxerfp);
	fread(xer_buf, fsize, 1, pxerfp);
	fclose(pxerfp);

	//printf("xer_buf = %s\n",xer_buf);
	asn_dec_rval_t rval;
	rval = xer_decode(0, &asn_DEF_MessageFrame, (void **) pSendMsg, xer_buf,
			fsize);
//	assert(rval.code == RC_OK);

	if(rval.code != RC_OK)
	{
		//printf("xer_decode Error!\n");
	}
	if (xer_buf != NULL)
	{
		free(xer_buf);
		xer_buf = NULL;
	}
}
