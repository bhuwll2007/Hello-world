#include "TcpServer.h"

int GetData(uint8_t *p_cRecvBuf, uint32_t pos, int size)
{

	return 0;
}

void QueryAlgorithmParameter(uint8_t *buf, uint32_t *bufsize)
{
	uint8_t p_buf[MAXARR];
	uint32_t p_bufsize = 0;

	memcpy(&buf[p_bufsize], &g_V2V_Parameter.MapMatch_ConfidenceDis_toLink,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.isDanger_hv_warning_time,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.isDanger_hv_speed_low,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.VehLOC_SafeDis_Value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.ICW_TTC_SafeDisVeh,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.AVW_Speed_threshold_value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.FCW_TTC_threshold_value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.FCW_THW_threshold_value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.LCW_Blind_Spot_Angle,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.LCW_Blind_Spot_Width,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.LCW_Blind_Spot_ToMirror,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_V2V_Parameter.ICW_TTC_threshold_value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_Spat_Parameter.LightDir_Dis_Value,
			sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_Spat_Parameter.WarnCalcTime, sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_Spat_Parameter.SpeedLimit_High, sizeof(double));
	p_bufsize += sizeof(double);
	memcpy(&buf[p_bufsize], &g_Spat_Parameter.SpeedLimit_Low, sizeof(double));
	p_bufsize += sizeof(double);

	Encode_Frame((uint8_t) 0xF0, (uint8_t) 0x00, (uint8_t) 0x00, p_buf,
			p_bufsize, buf, bufsize);

}

uint8_t *GetPuint8_t(uint8_t *p_cRecvBuf, uint32_t *pos, int len)
{
	static uint8_t res[MAXARR];
	memset(res, 0, MAXARR);
	int i;
	int j = 0;

	for (i = len - 1; i >= 0; i--)
	{
		printf("pos = %d i = %d\n", *pos, i);
		if (p_cRecvBuf[*pos] == 0xFE && p_cRecvBuf[*pos + 1] == 0x00)
		{
			res[i] = 0xFE;
			*pos += 2;
		}
		else if (p_cRecvBuf[*pos] == 0xFE && p_cRecvBuf[*pos + 1] == 0x01)
		{
			res[i] = 0xFF;
			*pos += 2;
		}
		else
		{
			res[i] = p_cRecvBuf[(*pos)++];
		}
	}

	printf("***********************\n");
	for (j = 0; j < len; j++)
	{
		printf("%02x ", res[j]);
	}
	printf("\n***********************\n");
	return res;
}

void SetAlgorithmParameter(uint8_t *p_cRecvBuf, uint8_t *buf, uint32_t *bufsize)
{
	uint8_t p_buf[MAXARR];
	uint32_t p_bufsize = 0;
	uint32_t pos = 0;
	char value[MAXARR];
	char *pInFileName = "./AlgorithmCfg.ini";

	pos = 8;
	g_V2V_Parameter.MapMatch_ConfidenceDis_toLink = *((double *) GetPuint8_t(
			p_cRecvBuf, &pos, sizeof(double)));
	sprintf(value, "%f", g_V2V_Parameter.MapMatch_ConfidenceDis_toLink);
	SetConfigStringValue(pInFileName, "V2V", "MapMatch_Dis", value);

	g_V2V_Parameter.isDanger_hv_warning_time = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.isDanger_hv_warning_time);
	SetConfigStringValue(pInFileName, "V2V", "isDanger_hv_warning_time", value);
	pos += sizeof(double);
	g_V2V_Parameter.isDanger_hv_speed_low = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.isDanger_hv_speed_low);
	SetConfigStringValue(pInFileName, "V2V", "isDanger_hv_speed_low", value);
	pos += sizeof(double);
	g_V2V_Parameter.VehLOC_SafeDis_Value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.VehLOC_SafeDis_Value);
	SetConfigStringValue(pInFileName, "V2V", "VehLOC_SafeDis_Value", value);
	pos += sizeof(double);
	g_V2V_Parameter.ICW_TTC_SafeDisVeh = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.ICW_TTC_SafeDisVeh);
	SetConfigStringValue(pInFileName, "V2V", "ICW_TTC_SafeDisVeh", value);
	pos += sizeof(double);
	g_V2V_Parameter.AVW_Speed_threshold_value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.AVW_Speed_threshold_value);
	SetConfigStringValue(pInFileName, "V2V", "AVW_Speed_threshold_value",
			value);
	pos += sizeof(double);
	g_V2V_Parameter.FCW_TTC_threshold_value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.FCW_TTC_threshold_value);
	SetConfigStringValue(pInFileName, "V2V", "FCW_TTC_threshold_value", value);
	pos += sizeof(double);
	g_V2V_Parameter.FCW_THW_threshold_value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.FCW_THW_threshold_value);
	SetConfigStringValue(pInFileName, "V2V", "FCW_THW_threshold_value", value);
	pos += sizeof(double);
	g_V2V_Parameter.LCW_Blind_Spot_Angle = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.LCW_Blind_Spot_Angle);
	SetConfigStringValue(pInFileName, "V2V", "LCW_Blind_Spot_Angle", value);
	pos += sizeof(double);
	g_V2V_Parameter.LCW_Blind_Spot_Width = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.LCW_Blind_Spot_Width);
	SetConfigStringValue(pInFileName, "V2V", "LCW_Blind_Spot_Width", value);
	pos += sizeof(double);
	g_V2V_Parameter.LCW_Blind_Spot_ToMirror = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.LCW_Blind_Spot_ToMirror);
	SetConfigStringValue(pInFileName, "V2V", "LCW_Blind_Spot_ToMirror", value);
	pos += sizeof(double);
	g_V2V_Parameter.ICW_TTC_threshold_value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_V2V_Parameter.ICW_TTC_threshold_value);
	SetConfigStringValue(pInFileName, "V2V", "ICW_TTC_threshold_value", value);
	pos += sizeof(double);
	g_Spat_Parameter.LightDir_Dis_Value = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_Spat_Parameter.LightDir_Dis_Value);
	SetConfigStringValue(pInFileName, "SPAT", "LightDir_Dis_Value", value);
	pos += sizeof(double);
	g_Spat_Parameter.WarnCalcTime = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_Spat_Parameter.WarnCalcTime);
	SetConfigStringValue(pInFileName, "SPAT", "WarnCalcTime", value);
	pos += sizeof(double);
	g_Spat_Parameter.SpeedLimit_High = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_Spat_Parameter.SpeedLimit_High);
	SetConfigStringValue(pInFileName, "SPAT", "SpeedLimit_High", value);
	pos += sizeof(double);
	g_Spat_Parameter.SpeedLimit_Low = *((double *) &p_cRecvBuf[pos]);
	sprintf(value, "%f", g_Spat_Parameter.SpeedLimit_Low);
	SetConfigStringValue(pInFileName, "SPAT", "SpeedLimit_Low", value);
	pos += sizeof(double);

	Encode_Frame((uint8_t) 0xF0, (uint8_t) 0x01, (uint8_t) 0x00, p_buf,
			p_bufsize, buf, bufsize);

}

void DisposeMessage(uint32_t fd, nint8_t *p_cRecvBuf)
{
	uint8_t xmlFileName[MAXARR] = {0};
	FILE *fp = NULL;
	int nLen = 0;
	uint8_t buf[MAXARR];
	uint32_t bufsize;

	uint16_t u16Light;
	uint32_t u16Break;
	uint32_t VehClass;
	uint32_t AuxBreakSys;

	//查询算法参数
	if (p_cRecvBuf[3] == 0xE0 && p_cRecvBuf[4] == 0x00)
	{
		bufsize = 0;
		memset(buf, 0, MAXARR);
		QueryAlgorithmParameter(buf, &bufsize);
		TcpSer_Send(fd, (nint8_t *) buf, bufsize);
	}
	//设置算法参数
	if (p_cRecvBuf[3] == 0xE0 && p_cRecvBuf[4] == 0x01)
	{
		bufsize = 0;
		memset(buf, 0, MAXARR);
		SetAlgorithmParameter((uint8_t *) p_cRecvBuf, buf, &bufsize);
		TcpSer_Send(fd, (nint8_t *) buf, bufsize);
	}
	//查询CAN设备数据
//	if (p_cRecvBuf[3] == 0xE2 && p_cRecvBuf[4] == 0x00){
//		bufsize = 0;
//		memset(buf, 0, MAXARR);
//		QueryCANData(buf, &bufsize);
//		TcpSer_Send(fd, buf, bufsize);
//	}
	//设置CAN设备数据
//	if (p_cRecvBuf[3] == 0xE2 && p_cRecvBuf[4] == 0x00){
//		bufsize = 0;
//		memset(buf, 0, MAXARR);
//		SetCANData(buf, &bufsize);
//		TcpSer_Send(fd, buf, bufsize);
//	}
	if (p_cRecvBuf[3] == 0xF2)
	{
		memcpy(&u16Light, &p_cRecvBuf[20], sizeof(uint16_t));
		memcpy(&u16Break, &p_cRecvBuf[22], sizeof(uint32_t));

		memcpy(&AuxBreakSys, &p_cRecvBuf[46], sizeof(uint32_t));
		memcpy(&VehClass, &p_cRecvBuf[50], sizeof(uint32_t));

		printf("u16Light = %d,u16Break = %d\n", u16Light, u16Break);

		SetConfigIntValue("./HostInfo.ini", "CANDATA", "Light", u16Light);
		SetConfigIntValue("./HostInfo.ini", "CANDATA", "Break", u16Break);
		SetConfigIntValue("./HostInfo.ini", "HostAttriInfo",
				"Veh_Classification", VehClass);
		SetConfigIntValue("./HostInfo.ini", "CANDATA", "AuxBreakSys",
				AuxBreakSys);

		Store.HostAttriInfo.choice.OBUInfo.VehClassification = VehClass;
	}
	if (p_cRecvBuf[3] == 0xF4)
	{
		printf("1nLen = %d xmlFileName= %s \n",nLen,xmlFileName);
		if(xmlFileName[0] == 0x00)
			memcpy(xmlFileName, "Xml_NoName.xml", strlen("Xml_NoName.xml"));
		fp = fopen((char *) xmlFileName,"a+");
		if(fp > 0)
		{
			memset(buf, 0x00, sizeof(buf));
			nLen = (int )(p_cRecvBuf[6]&0xFF) + (int)((p_cRecvBuf[7] << 8)&0xFF00);
			memcpy(buf, &p_cRecvBuf[10], nLen);
			fprintf(fp, "%s\n", buf);
		}
		fclose(fp);
	}
	if (p_cRecvBuf[3] == 0xF5)
	{
		memset(xmlFileName, 0x00, sizeof(xmlFileName));
		nLen = (int )(p_cRecvBuf[6]&0xFF) + (int)((p_cRecvBuf[7] << 8)&0xFF00);
		printf("2nLen = %d xmlFileName= %s \n",nLen,xmlFileName);
		memcpy(xmlFileName, &p_cRecvBuf[8], nLen);
		printf("3nLen = %d xmlFileName= %s \n",nLen,xmlFileName);
	}
}

int SplicingChar(nint8_t *buf, uint32_t *len, nint8_t *pbuf, uint32_t *plen)
{
	printf("len is %d\n", *len);
	int pstart = 0, pend;

	if (*len == 0)
	{
		return 0;
	}

	while (!((buf[pstart] == 0xFF) && (buf[pstart + 1] == 0xFF)
			&& (buf[pstart + 2] != 0xFF)) && (pstart + 2 < *len))
	{
		pstart++;
	}
	printf("pstart is %d\tlen is %d\n", pstart, *len);
	if (pstart == (*len - 2))
	{
		*len = 0;
		printf("memset\n");
		memset(buf, 0, MAXARR);
//		printf("\n\t");
//		for(i = 0;i < *len; i++){
//			printf("%02X ", buf[i]);
//		}
		return 0;
	}

	pend = pstart + 2;
	while (!((buf[pend - 1] == 0xFF) && (buf[pend - 2] != 0xFF))
			&& ((pend - 1) <= (*len)))
	{
		pend++;
	}
	printf("pend is %d\tlen is %d\n", pend, *len);
	if (pend - 1 > *len)
	{
		return 0;
	}

	*plen = pend - pstart;
	memcpy(pbuf, buf + pstart, *plen);

	*len -= pend;
	memcpy(buf, buf + pend, MAXARR - pend);
	return 1;
}

//接收数据回调函数
void CallBack_TcpServerRecvData(uint32_t fd, nint8_t *p_cRecvBuf,
		uint32_t pnRecvBufSize)
{
	static nint8_t buf[MAXARR] =
	{ 0 };
	static uint32_t len = 0;
	nint8_t pbuf[MAXARR] =
	{ 0 };
	uint32_t plen = 0;
	int i = 0;

#if 0
		struct timeval tv1;
		gettimeofday(&tv1, NULL);
		printf("Time TcpRecv: tv.tv_sec = %ld,tv.tv_usec = %ld\n", tv1.tv_sec,
				tv1.tv_usec);
#endif
	memcpy(buf + len, p_cRecvBuf, pnRecvBufSize);
	len += pnRecvBufSize;
	if (pnRecvBufSize > 0)
	{
		printf("TcpService :Recv %d Bytes.from fd = %d\n", pnRecvBufSize, fd);
		for (i = 0; i < len; i++)
		{
			printf("%02X ", buf[i]);
		}
		printf("\n");
	}

	while (SplicingChar(buf, &len, pbuf, &plen))
	{
//		for (i = 0; i < plen; i++) {
//			printf("%02X ", pbuf[i]);
//		}
//		printf("\n");
//		sleep(1);
		DisposeMessage(fd, pbuf);
	}
	//测试接收到设置CAN信息，写入配置文件中
//	if(p_cRecvBuf[3] == 0xF2)
//	{
//		uint16_t a;
//		uint32_t b;
//		memcpy(&a, &p_cRecvBuf[20],sizeof(uint16_t));
//		memcpy(&b, &p_cRecvBuf[22],sizeof(uint32_t));
//		printf("a = %d,b = %d\n",a,b);
//		SetConfigIntValue("./HostInfo.ini","CANDATA","Light",a);
//		SetConfigIntValue("./HostInfo.ini","CANDATA","Break",b);
//	}
//	printf("33333-----\n");
//	TcpSer_Send(fd, p_cRecvBuf, pnRecvBufSize);

}
void CallBack_Listen(StcTcpCln *pstcTcpCln, int nIndex)
{
	pstcTcpCln->pCallBackDealData = CallBack_TcpServerRecvData;
	return;
}
int InitTcpServer(int nSerPort)
{
	int nRet = 0;
//	StcTcpServer pStcTcpServer;
//	StcUdpService pUdpService;
//	pUdpService.m_selfPort = 100;
//	UdpSrv_InitSoc(&pUdpService);
	g_StcTcpServer.m_TcpSerPort = nSerPort;
	g_StcTcpServer.pCallBack_Listen = CallBack_Listen;
	nRet = TcpSer_InitSoc(&g_StcTcpServer);
	if (nRet < 0)
	{
		printf("nRet = %d \n", nRet);
		return -1;
	}
	return 0;
}
