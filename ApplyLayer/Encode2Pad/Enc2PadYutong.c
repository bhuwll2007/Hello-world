#include "Enc2PadYutong.h"
#include "../Common/EncodeYT_cJSON.h"

#define RSMVehMaxNum 60

typedef struct _Result_Rsi_YuTong
{
	uint8_t wStr; /**< V2I应用场景. */

	///消息类型
	uint8_t MsgType;
	///参考位置
	nint32_t Latitude; /**< 纬度值. */
	nint32_t Longitude; /**< 经度值. */
	uint16_t AlertPosDir; /**< 图标基于路网的角度. */

	///从Rsi的Description中提取出的事件相关信息
	uint16_t TagVal;
	uint8_t AssisTag;
	uint8_t AssisVal;


}__attribute__ ((packed)) tResult_Rsi_YuTong;

int EncodeD4_YuTong(tVehData HV, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen)
{
	//数据转换
	tResult_Rsi_YuTong lResult_Rsi_YuTong;

//	lResult_Rsi_YuTong.wType = lResult_Rsi.wType; /**< 应用类型. */
	lResult_Rsi_YuTong.wStr = lResult_Rsi.wStr; /**< V2I应用场景. */
//	lResult_Rsi_YuTong.wPrio = lResult_Rsi.wPrio; /**< 优先级. */
//	lResult_Rsi_YuTong.WarningType = lResult_Rsi.WarningType; /**< 警告类型. */

	///消息类型
	lResult_Rsi_YuTong.MsgType = lResult_Rsi.MsgType;
	///参考位置（经纬海拔）
	lResult_Rsi_YuTong.Latitude = (nint32_t)(lResult_Rsi.AlertPos.Latitude * 10000000); /**< 纬度值. */
	lResult_Rsi_YuTong.Longitude = (nint32_t)(lResult_Rsi.AlertPos.Longitude * 10000000); /**< 经度值. */
	lResult_Rsi_YuTong.AlertPosDir = (lResult_Rsi.AlertPathList[0].AlertPosDir); /**< 图标基于路网的角度. */

	///从Rsi的Description中提取出的事件相关信息
	if(lResult_Rsi_YuTong.MsgType == 81 || lResult_Rsi_YuTong.MsgType == 82)
		lResult_Rsi_YuTong.TagVal = (uint16_t)(lResult_Rsi.DscpInfo.TagVal*100);
	else
		lResult_Rsi_YuTong.TagVal = (uint16_t)(lResult_Rsi.DscpInfo.TagVal);

	lResult_Rsi_YuTong.AssisTag = (uint16_t)(lResult_Rsi.DscpInfo.AssisTag);
	lResult_Rsi_YuTong.AssisVal = (uint16_t)(lResult_Rsi.DscpInfo.AssisVal);
	//if(lResult_Rsi_YuTong.MsgType == 81) printf("%d  %d  %d\n",lResult_Rsi_YuTong.TagVal, lResult_Rsi_YuTong.AssisTag,lResult_Rsi_YuTong.AssisVal);

	uint8_t buf[1000];
	uint32_t buflen = 0;

	memcpy(&buf[buflen], &lResult_Rsi_YuTong, sizeof(tResult_Rsi_YuTong));
	buflen += sizeof(tResult_Rsi_YuTong);

	//数据组帧
	Encode_Frame((uint8_t) 0xD4, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);
	return 0;
}

typedef struct _UpPhaseInfo_YuTong
{
	uint8_t Status; /**< 灯色状态. */
	uint8_t TimeLeft; /**< 剩余时间. */
	uint8_t Direction; /**< 行车方向. */
	uint8_t SuggSd_H; /**< 最高建议时速. */
	uint8_t SuggSd_L; /**< 最低建议时速. */
	uint8_t SuggSd; /**< 建议时速. */
}__attribute__ ((packed)) UpPhaseInfo_YuTong;

typedef struct _SpeedGudie_Input_YuTong_D6
{
	uint16_t HVLight; /**< 转向灯状态 详见定义. */
	nint32_t Latitude; /**< 信号灯纬度. */
	nint32_t Longitude; /**< 信号灯经度. */
//	uint8_t wType; /**< 应用类型. */
	uint8_t wStr; /**< 应用场景. */
	uint8_t wPrio; /**< 优先级. */
//	uint16_t Cycle; /**< 信号灯周期. */
//	nint16_t Distance; /**< 到红绿灯距离. */
//	nint16_t RelBearing; /**< 红绿灯方位角. */
	uint8_t PhaseCnt; /**< 前方相位个数. */
	UpPhaseInfo_YuTong PhaseInfo_YT[PHASEINFO_MAX_NUM]; /**< 包含相位信息结构体. */
}__attribute__ ((packed)) tResult_Spat_YuTong_D6;

int EncodeD6_2VehYuTong(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen)
{
	cJSON *cJMsg_103 = NULL;
	stcMsg_103 lstcMsg_103;
	cJMsg_103 = cJSON_CreateObject();
	memset(&lstcMsg_103, 0x00, sizeof(stcMsg_103));

	lstcMsg_103.local_info.latitude = HV.Latitude;
	lstcMsg_103.local_info.longitude = HV.Longitude;
	lstcMsg_103.local_info.heading = HV.Heading;
	lstcMsg_103.local_info.speed = HV.Speed * 3.6;

	memcpy(lstcMsg_103.time, "1234567890123456789", 19);
	if(pSpeedGudie_Input.IntersectionID == 1)
	{
		lstcMsg_103.spat_ID[0] = 0x12;
		lstcMsg_103.spat_ID[1] = 0x34;
		lstcMsg_103.spat_ID[2] = 0x56;
		lstcMsg_103.spat_ID[3] = 0x78;

		lstcMsg_103.spat_ID[4] = 0x12;
		lstcMsg_103.spat_ID[5] = 0x34;
		lstcMsg_103.spat_ID[6] = 0x56;
		lstcMsg_103.spat_ID[7] = 0x78;
	}
	lstcMsg_103.spat_longitude = pSpeedGudie_Input.Lat;
	lstcMsg_103.spat_latitude = pSpeedGudie_Input.Lng;
	lstcMsg_103.distance = Res.Distance;
	lstcMsg_103.allowed_maneuvers[11] = 1;
	lstcMsg_103.allowed_maneuvers[10] = 1;
	lstcMsg_103.allowed_maneuvers[9] = 1;
	lstcMsg_103.allowed_maneuvers[8] = 1;
	lstcMsg_103.allowed_maneuvers[6] = 1;
	//8 lights_number 整型 当前路口信号灯的个数 ≥ 0	对象中的成员顺序对应着	信号灯从左到右排列
	lstcMsg_103.lights_number = Res.PhaseCnt;
	lstcMsg_103.lights_number = 1;//Res.PhaseCnt;

	int i = 0;
	for (i = 0; i < lstcMsg_103.lights_number; i++)
	{
		lstcMsg_103.mlight_info[i].light_type = 1;
		if((Res.tUpPhaseInfo[i].Status == 5)||(Res.tUpPhaseInfo[i].Status == 6))//绿灯
			lstcMsg_103.mlight_info[i].current_status = 6;
		else if((Res.tUpPhaseInfo[i].Status == 7)||(Res.tUpPhaseInfo[i].Status == 8))//黄灯
			lstcMsg_103.mlight_info[i].current_status = 7;
		else if(Res.tUpPhaseInfo[i].Status == 3)//红灯
			lstcMsg_103.mlight_info[i].current_status = 3;
		lstcMsg_103.mlight_info[i].current_left_time = Res.tUpPhaseInfo[i].TimeLeft;
		lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_value = Res.tUpPhaseInfo[i].SuggestSpeed * 3.6;
		if (lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_value > lstcMsg_103.local_info.speed)
		{
			lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_type = 4;
		}
		else
		{
			lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_type = 3;
		}
		if(lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_value < 1)
			lstcMsg_103.mlight_info[i].mspeed_advice.speed_advice_type = 0;
	}
	// 10 red_light_violate 整型 闯红灯危险预警 0:无闯红灯危险	1:有闯红灯危险,当无法获取到车道信息时,使用车辆转向灯信号进行判定
	if (Res.wStr == RLVW_Waring)
		lstcMsg_103.red_light_violate = 1;
	else
		lstcMsg_103.red_light_violate = 0;

	nint8_t VID[20] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x31,0x30,0x32,0x33,0x34,0x35,0x36,0x37,0x38};
	Factory_Msg103(lstcMsg_103, cJMsg_103);
	nint8_t *buf = cJSON_PrintUnformatted(cJMsg_103);
	cJSON_Delete(cJMsg_103);
	// char buf1[3000];
	// memset(buf1, 0x00, sizeof(buf1));
	// memcpy(buf1,buf,strlen(buf));
	// if(buf)
	Encode_Frame_YT_Veh(103, 1, VID, (uint8_t *)buf, strlen(buf),
			desbuf, deslen);
	if(buf != NULL)
		free(buf);
	// printf("desbuf = %s\n",desbuf);
	return 0;
}

int EncodeD6_YuTong(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen)
{
	tResult_Spat_YuTong_D6 lSpatRes_YT;

	int i;
	uint8_t buf[1000];
	uint32_t buflen = 0;

	//结构转换 赋值
	lSpatRes_YT.HVLight = HV.Light;
	lSpatRes_YT.Latitude = (nint32_t)(pSpeedGudie_Input.Lat* 10000000); /**< 信号灯纬度.*/
	lSpatRes_YT.Longitude = (nint32_t)(pSpeedGudie_Input.Lng* 10000000); /**< 信号灯经度.*/
	// lSpatRes_YT.wType = Res.wType; /**< 应用类型.*/
	lSpatRes_YT.wStr = Res.wStr; /**< 应用场景.*/
	lSpatRes_YT.wPrio = Res.wPrio; /**< 优先级.*/
	// lSpatRes_YT.Cycle = Res.Cycle; /**< 信号灯周期.*/
	// lSpatRes_YT.Distance = Res.Distance; /**< 到红绿灯距离.*/
	// lSpatRes_YT.RelBearing = Res.RelBearing; /**< 红绿灯方位角.*/
	lSpatRes_YT.PhaseCnt = Res.PhaseCnt; /**< 前方相位个数. */
	for (i = 0; i < lSpatRes_YT.PhaseCnt; i++)
	{
		/**< 灯色状态. */
		lSpatRes_YT.PhaseInfo_YT[i].Status = Res.tUpPhaseInfo[i].Status;
		/**< 剩余时间. */
		lSpatRes_YT.PhaseInfo_YT[i].TimeLeft = Res.tUpPhaseInfo[i].TimeLeft;
		/**< 行车方向. */
		lSpatRes_YT.PhaseInfo_YT[i].Direction = Res.tUpPhaseInfo[i].Direction;
		/**< 最高建议时速. */
		lSpatRes_YT.PhaseInfo_YT[i].SuggSd_H = Res.tUpPhaseInfo[i].SuggestSpeed_High;
		/**< 最低建议时速. */
		lSpatRes_YT.PhaseInfo_YT[i].SuggSd_L =
				Res.tUpPhaseInfo[i].SuggestSpeed_Low;
		/**< 建议时速. */
		lSpatRes_YT.PhaseInfo_YT[i].SuggSd = Res.tUpPhaseInfo[i].SuggestSpeed;
	}
	//赋值 循环之前
	memcpy(&buf[buflen], &lSpatRes_YT,
			sizeof(uint16_t) + sizeof(uint32_t) * 2 + sizeof(uint8_t) * 3);
	buflen += sizeof(uint16_t) + sizeof(uint32_t) * 2 + sizeof(uint8_t) * 3;

	//赋值 循环数组
	for (i = 0; i < pSpeedGudie_Input.PhaseCnt; i++)
	{
		memcpy(&buf[buflen], &lSpatRes_YT.PhaseInfo_YT[i].Status,
				sizeof(UpPhaseInfo_YuTong));
		buflen += sizeof(UpPhaseInfo_YuTong);
	}

	Encode_Frame((uint8_t) 0xD6, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

	return 0;
}

typedef struct _AnalysisInfo_YT
{
	uint32_t AnalysisInfoId; /** 识别物ID. */
	uint8_t VehicleClass; /**< 车辆类型. */
	uint8_t Release1; /**< 预留. */
	nint32_t Latitude; /**< 纬度 度. */
	nint32_t Longitude; /**< 经度 度. */
	uint16_t Speed; /**< 速度 米每秒. */
	uint16_t Heading;/**< 航向角 度. */

	uint16_t VehicleWidth; /**< 车宽. 单位 厘米*/
	uint16_t VehicleLength; /**< 车长. 单位 厘米*/
	uint16_t VehicleHight; /**< 车高. 单位 厘米*/
	uint16_t Release2; /**< 预留. */
}__attribute__ ((packed)) AnalysisInfo_YT;

typedef struct _SpeedGudie_Input_YuTong_D5
{
	nint32_t HVLatitude; /**< 自车纬度. */
	nint32_t HVLongitude; /**< 自车经度. */
	nint32_t LaserLatitude; /**< 激光纬度. */
	nint32_t LaserLongitude; /**< 激光经度. */
	uint8_t nAnalysisCount; /**< 识别物个数. */
	AnalysisInfo_YT AnalysisInfo[RSMVehMaxNum];
}__attribute__ ((packed)) tResult_Spat_YuTong_D5;

int EncodeD5_2VehYuTong(double Laser_Lat, double Laser_Long, tVehData HV,
		tVehData *RV, uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen)
{
	cJSON *cJMsg_108 = NULL;
	stcMsg_108 lstcMsg_108;
	cJMsg_108 = cJSON_CreateObject();
	memset(&lstcMsg_108, 0x00, sizeof(stcMsg_108));
	lstcMsg_108.LidarID[0] = 0xC1;
	lstcMsg_108.LidarID[1] = 0xC2;
	lstcMsg_108.LidarID[2] = 0xC3;
	lstcMsg_108.LidarID[3] = 0xC4;
	lstcMsg_108.LidarID[4] = 0xC5;
	lstcMsg_108.LidarID[5] = 0xC6;
	lstcMsg_108.LidarID[6] = 0xC7;
	lstcMsg_108.LidarID[7] = 0xC8;
	lstcMsg_108.PTCNum = nRVCont;
	int i = 0;
	for (i = 0; i < nRVCont; i++)
	{
		lstcMsg_108.mPTCObjs[i].PTCType = RV[i].VehicleClass;
		lstcMsg_108.mPTCObjs[i].Source = 4;
		lstcMsg_108.mPTCObjs[i].PTCId = (int)RV[i].Id;
		lstcMsg_108.mPTCObjs[i].Lonitude = RV[i].Longitude;//4 + i * 10;
		lstcMsg_108.mPTCObjs[i].Latitude = RV[i].Latitude;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point1.x = 6.1 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point1.y = 6.2 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point2.x = 6.3 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point2.y = 6.4 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point3.x = 6.5 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point3.y = 6.6 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point4.x = 6.7 + i * 10;
		lstcMsg_108.mPTCObjs[i].mOutLine.Point4.y = 6.8 + i * 10;
		lstcMsg_108.mPTCObjs[i].Speed = RV[i].Speed * 3.6;
		lstcMsg_108.mPTCObjs[i].Heading = RV[i].Heading;
	}
	Factory_Msg108(lstcMsg_108, cJMsg_108);
	//nint8_t *buf =  cJSON_Print(cJMsg_108);
	nint8_t *buf =  cJSON_PrintUnformatted(cJMsg_108);
	nint8_t VID[20] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x31,0x30,0x32,0x33,0x34,0x35,0x36,0x37,0x38};

	Encode_Frame_YT_Veh(201, 1, VID, (uint8_t *)buf, strlen(buf),
			desbuf, deslen);

	if(buf != NULL)
		free(buf);
	return 0;
}
int EncodeD5_YuTong(double Laser_Lat, double Laser_Long, tVehData HV,
		tVehData *RV, uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t buf[10240];
	uint32_t buflen = 0;
	int nTmpi = 0;

	tResult_Spat_YuTong_D5 lResult_Spat_YuTong;
	lResult_Spat_YuTong.HVLatitude = (nint32_t) (HV.Latitude * 10000000); /**< 自车纬度. */
	lResult_Spat_YuTong.HVLongitude = (nint32_t) (HV.Longitude * 10000000); /**< 自车经度. */
	lResult_Spat_YuTong.LaserLatitude = (nint32_t) (Laser_Lat * 10000000); /**< 激光纬度. */
	lResult_Spat_YuTong.LaserLongitude = (nint32_t) (Laser_Long * 10000000); /**< 激光经度. */
	lResult_Spat_YuTong.nAnalysisCount = nRVCont; /**< 识别物个数. */
	for (nTmpi = 0; nTmpi < nRVCont; nTmpi++)
	{
		memcpy(&(lResult_Spat_YuTong.AnalysisInfo[nTmpi].AnalysisInfoId), RV[nTmpi].Id, 4);/** 识别物ID. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].VehicleClass = RV[nTmpi].VehicleClass; /**< 车辆类型. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Release1 = 0; /**< 预留. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Latitude = (nint32_t) (RV[nTmpi].Latitude * 10000000); /**< 纬度 度. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Longitude = (nint32_t) (RV[nTmpi].Longitude * 10000000); /**< 经度 度. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Speed = (uint16_t)(RV[nTmpi].Speed*3.6); /**< 速度 米每秒. */
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Heading = (uint16_t)(RV[nTmpi].Heading);/**< 航向角 度. */

		lResult_Spat_YuTong.AnalysisInfo[nTmpi].VehicleWidth = (uint16_t)(RV[nTmpi].VehicleLength * 100); /**< 车宽. 单位 厘米*/
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].VehicleLength = (uint16_t)(RV[nTmpi].VehicleWidth * 100); /**< 车长. 单位 厘米*/
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].VehicleHight = (uint16_t)(RV[nTmpi].VehicleHight * 100); /**< 车高. 单位 厘米*/
		lResult_Spat_YuTong.AnalysisInfo[nTmpi].Release2 = 0; /**< 预留. */
	}
	memcpy(&buf[buflen], &lResult_Spat_YuTong.HVLatitude,
			sizeof(uint8_t) + sizeof(uint32_t) * 4);
	buflen += sizeof(uint8_t) + sizeof(uint32_t) * 4;

	for (nTmpi = 0; nTmpi < nRVCont; nTmpi++)
	{
		memcpy(&buf[buflen],
				&lResult_Spat_YuTong.AnalysisInfo[nTmpi].AnalysisInfoId,
				sizeof(AnalysisInfo_YT));
		buflen += sizeof(AnalysisInfo_YT);
	}
	Encode_Frame((uint8_t) 0xD5, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

	return 0;
}
//与EncodeD3_V1 处理一样
int EncodeD3_YuTong(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen)
{
	uint8_t buf[1000];
	uint32_t buflen = 0;

	memcpy(&buf[buflen], &HV, sizeof(tVehData));
	buflen += sizeof(tVehData);
	memcpy(&buf[buflen], &RV, sizeof(tVehData));
	buflen += sizeof(tVehData);
	memcpy(&buf[buflen], &lRes, sizeof(tResult_Bsm));
	buflen += sizeof(tResult_Bsm);

	Encode_Frame((uint8_t) 0xD3, (uint8_t) 0x00, (uint8_t) 0x00, buf, buflen,
			desbuf, deslen);

	return 0;
}

typedef struct _stcSelVeh_YuTong
{
	uint8_t UEID[4];			//自己的车辆UEID
	nint32_t nSelfLat;			//自己的坐标的纬度 单位：1e-7度
	nint32_t nSelfLng;			//自己的坐标的经度 单位：1e-7度
	uint16_t nSelfSpeed;		//自己的速度	单位：km/h
	uint16_t nLimitHighSpeed;	//当前道路限制的最高速度	无限速时默认200，单位：km/h
	nint16_t nSelfTrack;		//自己的方向	单位：度
	nint16_t nSpatDistence;		//距离信号机的距离		单位：米
}__attribute__ ((packed))stcSelVeh_YuTong;

typedef struct _stcArdVeh_YuTong
{
	uint8_t UEID[4];			//对方车辆UEID
	nint16_t IsLive;			//该车存在状态 有车时默认设置为 100（100ms 减小一次）；
								//<5时 显示端删除即可
								//等于0时，设备端复用该位置结构体存储新车
	nint16_t IsSafe;			//该车安全状态 安全时默认设置为 -1；
								//>0时 代表当前车辆的危险状态
	nint32_t nDuiLat;			//对方的坐标的纬度	 单位：1e-7度
	nint32_t nDuiLng;			//对方的坐标的经度	 单位：1e-7度
	uint16_t nDistance;			//本机与对方的车距 单位：米
	uint16_t nDuiSpeed;			//对方速度	单位：km/h
	nint16_t nDuiTrack;			//对方的方向	单位：度
}__attribute__ ((packed)) stcArdVeh_YuTong;
int EncodeD0_YuTong(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
{
	uint8_t srcbuf[1024];
	uint32_t srclen = 0;
	uint8_t ntmpi = 0;
	stcSelVeh_YuTong lstcSelVeh_YuTong;
	memcpy(lstcSelVeh_YuTong.UEID, pUpD0Data.SlfVehData.UEID, 4);

	lstcSelVeh_YuTong.nSelfLat = (nint32_t) (pUpD0Data.SlfVehData.dSelfLat * 10000000);			//自己的坐标的纬度 单位：1e-7度
	lstcSelVeh_YuTong.nSelfLng = (nint32_t) (pUpD0Data.SlfVehData.dSelfLng * 10000000);			//自己的坐标的经度 单位：1e-7度
	lstcSelVeh_YuTong.nSelfSpeed = pUpD0Data.SlfVehData.nSelfSpeed;		//自己的速度	单位：km/h
	lstcSelVeh_YuTong.nLimitHighSpeed = pUpD0Data.SlfVehData.nLimitHighSpeed;	//当前道路限制的最高速度	无限速时默认200，单位：km/h
	lstcSelVeh_YuTong.nSelfTrack = pUpD0Data.SlfVehData.nSelfTrack;		//自己的方向	单位：度
	lstcSelVeh_YuTong.nSpatDistence = pUpD0Data.SlfVehData.nSpatDistence;		//距离信号机的距离		单位：米

	stcArdVeh_YuTong lstcArdVeh_YuTong[MAXAROUNDVEHNUM];
	for(ntmpi = 0; ntmpi < pUpD0Data.nAroundVehNum; ntmpi ++)
	{
		memcpy(lstcArdVeh_YuTong[ntmpi].UEID, pUpD0Data.ArdVehData[ntmpi].UEID, 4);/** 识别物ID. */
		lstcArdVeh_YuTong[ntmpi].IsLive = pUpD0Data.ArdVehData[ntmpi].IsLive;
		lstcArdVeh_YuTong[ntmpi].IsSafe = pUpD0Data.ArdVehData[ntmpi].IsSafe;
		//printf("pUpD0Data.ArdVehData[ntmpi].dDuiLat = %f ",pUpD0Data.ArdVehData[ntmpi].dDuiLat);
		//printf("pUpD0Data.ArdVehData[ntmpi].dDuiLng = %f ",pUpD0Data.ArdVehData[ntmpi].dDuiLng);
		lstcArdVeh_YuTong[ntmpi].nDuiLat = (nint32_t) (pUpD0Data.ArdVehData[ntmpi].dDuiLat * 10000000);
		lstcArdVeh_YuTong[ntmpi].nDuiLng = (nint32_t) (pUpD0Data.ArdVehData[ntmpi].dDuiLng * 10000000);
		lstcArdVeh_YuTong[ntmpi].nDistance = pUpD0Data.ArdVehData[ntmpi].nDistance;
		lstcArdVeh_YuTong[ntmpi].nDuiSpeed = pUpD0Data.ArdVehData[ntmpi].nDuiSpeed;
		lstcArdVeh_YuTong[ntmpi].nDuiTrack = pUpD0Data.ArdVehData[ntmpi].nDuiTrack;
	}
	memcpy(&srcbuf[srclen], lstcSelVeh_YuTong.UEID, sizeof(stcSelVeh_YuTong));
	srclen += sizeof(stcSelVeh_YuTong);
	memcpy(&srcbuf[srclen], &pUpD0Data.nAroundVehNum, sizeof(uint16_t));
	srclen += sizeof(uint16_t);

	for (ntmpi = 0; ntmpi < pUpD0Data.nAroundVehNum; ntmpi++)
	{
		memcpy(&srcbuf[srclen], &lstcArdVeh_YuTong[ntmpi],
				sizeof(stcArdVeh_YuTong));
		srclen += sizeof(stcArdVeh_YuTong);
	}

	Encode_Frame((uint8_t) 0xD0, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf, srclen,
			desbuf, deslen);
	return 0;
}

int EncodeD0_2VehYuTong(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
{
	cJSON *cJMsg_101 = NULL;
	stcMsg_101 lstcMsg_101;
	cJMsg_101 = cJSON_CreateObject();
	memset(&lstcMsg_101, 0x00, sizeof(stcMsg_101));
	lstcMsg_101.mlocal_info.latitude = pUpD0Data.SlfVehData.dSelfLat;
	lstcMsg_101.mlocal_info.longitude = pUpD0Data.SlfVehData.dSelfLng;
	lstcMsg_101.mlocal_info.heading = pUpD0Data.SlfVehData.nSelfTrack;
	lstcMsg_101.mlocal_info.speed = pUpD0Data.SlfVehData.nSelfSpeed;

	lstcMsg_101.remote_num = 0;

	Factory_Msg101(lstcMsg_101, cJMsg_101);
	nint8_t *buf = cJSON_PrintUnformatted(cJMsg_101);
	cJSON_Delete(cJMsg_101);
	nint8_t VID[20] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x31,0x30,0x32,0x33,0x34,0x35,0x36,0x37,0x38};
	Encode_Frame_YT_Veh(101, 1, VID, (uint8_t *)buf, strlen(buf),
			desbuf, deslen);
	if(buf != NULL)
		free(buf);
	// Encode_Frame((uint8_t) 0xD0, (uint8_t) 0x00, (uint8_t) 0x00, srcbuf, srclen,
			// desbuf, deslen);
	return 0;
}