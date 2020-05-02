/*
 * RsmDeal.c
 *
 *  Created on: Sep 17, 2018
 *      Author: root
 */
#include "RsmDeal.h"
#define _PRINTF_RSM_DEALTIME 0 //print rsmdata deal time
int MsgLength = 0;
void FG_GetCompleteFrame(uint8_t *p_pucInBuf, int p_nInCnt, uint8_t *p_pucOutBuf, int *p_pnDestCnt,
		int *p_pnReadStatus, int * p_pnFirstReadFlag)
{
	int i;

	for (i = 0; i < p_nInCnt; i++)
	{
		if (p_pucInBuf[i] == 0xFF && p_pucInBuf[i + 1] == 0xFF && p_pucInBuf[i + 3] == 0xE1)
		{
			p_pucOutBuf[(*p_pnDestCnt)++] = p_pucInBuf[i];
			*p_pnReadStatus = 1;
			MsgLength = ((p_pucInBuf[i + 7] & 0xFF) << 8) + p_pucInBuf[i + 6];
			continue;
		}
		if (*p_pnReadStatus == 1)
		{
			p_pucOutBuf[(*p_pnDestCnt)++] = p_pucInBuf[i];
		}

		if (*p_pnDestCnt == MsgLength)
		{
			*p_pnReadStatus = 0;
			*p_pnFirstReadFlag = 0;
			*p_pnDestCnt = 0;
			memset(p_pucInBuf, 0, sizeof(*p_pucInBuf));
			continue;
		}
	}
}

typedef struct _ParticipantInfo_Laser
{
	uint16_t ID;
	uint8_t ParticType;
	uint8_t Confidence;
	uint8_t Colour;                      //***20190823**add**zhz**//
	uint8_t InfoSourse;				     //***20190823**add**zhz**//
	uint16_t Release0;					 //***20190823**add**zhz**//
	nint16_t Latitude; 	//度 或者 cm
	nint16_t Longitude; //度 或者 cm
	nint16_t Altitude; 	//度 或者 cm
	uint16_t Speed; 	//m/s
	uint16_t Heading; 	//度
	nint16_t ObjLength; //cm
	nint16_t ObjWidth; 	//cm
	nint16_t ObjHeight; //cm
	uint32_t Release1;
} tParticipantInfo_Laser;

typedef struct _ParticipantInfo_latlng
{
	uint16_t ID;
	uint8_t ParticType;
	uint8_t Confidence;
	uint8_t Colour;                      //***20190823**add**zhz**//
	uint8_t InfoSourse;				     //***20190823**add**zhz**//
	uint16_t Release0;					 //***20190823**add**zhz**//
	uint32_t Longitude; //度
	uint32_t Latitude; 	//度
	uint16_t Altitude; 	//度
	uint16_t Speed; 	//cm/s
	uint16_t Heading; 	//度
	nint16_t ObjLength; //cm
	nint16_t ObjWidth; 	//cm
	nint16_t ObjHeight; //cm
	uint32_t Release;
} tParticipantInfo_latlng;

typedef struct _ParticipantInfo_EarThd
{
	uint16_t ID;
	uint8_t ParticType;
	uint8_t Confidence;
	uint8_t Colour;                      //***20190823**add**zhz**//
	uint8_t InfoSourse;				     //***20190823**add**zhz**//
	uint16_t Release0;					 //***20190823**add**zhz**//
	uint32_t Longitude; //度
	uint32_t Latitude; 	//度
	nint16_t Altitude; 	//cm
	uint16_t Speed; 	//cm/s
	uint16_t Heading; 	//度 0~360
	nint16_t ObjLength; //cm
	nint16_t ObjWidth; 	//cm
	nint16_t ObjHeight; //cm
	nint16_t X_Axis;    //X轴坐标     //***20190823**add**zhz**//
	nint16_t Y_Axis;    //Y轴坐标		//***20190823**add**zhz**//
	nint16_t Z_Axis;    //Z轴坐标		//***20190823**add**zhz**//
	uint16_t Release;
} tParticipantInfo_EarThd;
tRsmMsg RsmInfo;

int Data_Analysis_E100(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
{
	memset(&RsmInfo, 0x00, sizeof(tRsmMsg));

	int nIndex = 0;
	int i;

    //Equipment ID
	nIndex += 2;

	//Reserve
	nIndex += 2;

	//点云数据帧号
	nIndex += 4;

	//时间戳秒
	nIndex += 4;

	//时间戳微秒
	nIndex += 4;

	//激光器原点经纬度
#if 0
	//double
	double lat = 0;
	memcpy(&lat, &p_cRecvBuf[nIndex], 8);
	RsmInfo.RefPosLat = lat * 10000000;
	nIndex += 8;

	double lng = 0;
	memcpy(&lng, &p_cRecvBuf[nIndex], 8);
	RsmInfo.RefPosLng = lng * 10000000;
	nIndex += 8;
#else
	//int
	memcpy(&RsmInfo.RefPosLng, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;

	memcpy(&RsmInfo.RefPosLat, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;
#endif

	//x轴与正北方夹角
	memcpy(&RsmInfo.RefPosAngel, &p_cRecvBuf[nIndex], 2);
	nIndex += 2;

	printf("Index = %d\n", nIndex);
	//交通参与者个数
	RsmInfo.ParticipantCnt = p_cRecvBuf[nIndex];
	nIndex += 1;

	//Reserve bit 5
	nIndex += 5;

	printf("RsmInfo.ParticipantCnt  = %d\n", RsmInfo.ParticipantCnt);

	RsmInfo.pParticipantInfo = calloc(RsmInfo.ParticipantCnt,
			sizeof(tParticipantInfo));

	tParticipantInfo_Laser lParticipantInfo_Laser;

	WayPoint BLH_Move, BLH_LIDAR, XYZ_Move;

	//激光雷达经纬度赋值
	BLH_LIDAR.x = RsmInfo.RefPosLng / 10000000.0;
	BLH_LIDAR.y = RsmInfo.RefPosLat / 10000000.0;
	//		BLH_LIDAR.x = 116.2886930;
	//		BLH_LIDAR.y = 40.
	for (i = 0; i < RsmInfo.ParticipantCnt; i++)
	  {
		memset(&lParticipantInfo_Laser, 0x00, sizeof(tParticipantInfo_Laser));

		//提取与数据，
		memcpy(&lParticipantInfo_Laser.ID, &p_cRecvBuf[nIndex],
				sizeof(tParticipantInfo_Laser));
		nIndex += sizeof(tParticipantInfo_Laser);

		//X轴与正北夹角转换为Y轴与正北夹角
		RsmInfo.RefPosAngel = RsmInfo.RefPosAngel - 90;

		//XY转换经纬度
		XYZ_Move.x = lParticipantInfo_Laser.Latitude / 100.0;
		XYZ_Move.y = lParticipantInfo_Laser.Longitude / 100.0;
		XYZ_To_BLH(BLH_LIDAR, XYZ_Move, RsmInfo.RefPosAngel, &BLH_Move);

#if 1
		printf("X = %f, Y = %f\n", XYZ_Move.x, XYZ_Move.y);
		double Distance_BLH = FG_Getdistance(BLH_LIDAR.y, BLH_LIDAR.x,BLH_Move.y, BLH_Move.x);
		double Distance_XYZ = sqrt(XYZ_Move.x * XYZ_Move.x + XYZ_Move.y * XYZ_Move.y);
		printf("XYZ距离:%f, BLH距离:%f\n", Distance_XYZ, Distance_BLH);
		double TrueAngle_XY = atan(XYZ_Move.x / XYZ_Move.y) * 180 / PI;
		double TrueAngle = FG_GetTrueBear(BLH_LIDAR.y, BLH_LIDAR.x, BLH_Move.y,BLH_Move.x);
		printf("XYZ角度：%f, 经纬度计算真方位:%f\n", TrueAngle_XY, TrueAngle);
#endif
		//赋值
		RsmInfo.pParticipantInfo[i].ID = lParticipantInfo_Laser.ID;
		RsmInfo.pParticipantInfo[i].ParticType = lParticipantInfo_Laser.ParticType;

		RsmInfo.pParticipantInfo[i].Latitude = lParticipantInfo_Laser.Latitude / 100.0;
		RsmInfo.pParticipantInfo[i].Longitude = lParticipantInfo_Laser.Longitude / 100.0;

		RsmInfo.pParticipantInfo[i].Latitude = BLH_Move.y;
		RsmInfo.pParticipantInfo[i].Longitude = BLH_Move.x;

		RsmInfo.pParticipantInfo[i].Altitude = lParticipantInfo_Laser.Altitude / 100.0;
		RsmInfo.pParticipantInfo[i].Speed = lParticipantInfo_Laser.Speed / 100.0;
		RsmInfo.pParticipantInfo[i].Heading = lParticipantInfo_Laser.Heading;
		RsmInfo.pParticipantInfo[i].ObjLength = lParticipantInfo_Laser.ObjLength / 100.0;
		RsmInfo.pParticipantInfo[i].ObjWidth = lParticipantInfo_Laser.ObjWidth / 100.0;
		RsmInfo.pParticipantInfo[i].ObjHeight = lParticipantInfo_Laser.ObjHeight / 100.0;
		//数据来源，来自微波雷达
//		RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;

	//Add by zhz 20190823 //
		RsmInfo.pParticipantInfo[i].SourecType = lParticipantInfo_Laser.InfoSourse;
		if(RsmInfo.pParticipantInfo[i].SourecType == 0)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 1)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_video;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 2)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_lidar;
		else
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_unknown;

//		RsmInfo.pParticipantInfo[i].Colour = lParticipantInfo_Laser.Colour;
//		RsmInfo.pParticipantInfo[i].Confidence = lParticipantInfo_Laser.Confidence;

#ifdef DEBUG
		printf("RsmInfo ID = %d\n", RsmInfo.pParticipantInfo[i].ID);
		printf("RsmInfo  ParticType= %d \n",
				RsmInfo.pParticipantInfo[i].ParticType);
		printf("RsmInfo  Latitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Latitude);
		printf("RsmInfo  Longitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Longitude);
		printf("RsmInfo  Altitude= %f (m)\n",
				RsmInfo.pParticipantInfo[i].Altitude);
		printf("RsmInfo  Speed= %f (m/s)\n", RsmInfo.pParticipantInfo[i].Speed);
		printf("RsmInfo  Heading= %f ((°))\n",
				RsmInfo.pParticipantInfo[i].Heading);
		printf("RsmInfo  ObjLength = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjLength);
		printf("RsmInfo  ObjWidth = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjWidth);
		printf("RsmInfo  ObjHeight= %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjHeight);
		printf("RsmInfo  SourecType= %d \n",
				RsmInfo.pParticipantInfo[i].SourecType);
#endif
		//赋值时的异常处理
		{
			//类型
			if (RsmInfo.pParticipantInfo[i].ParticType > 6 /* 3*/)      //***20190823**zhz**//
				RsmInfo.pParticipantInfo[i].ParticType = 0;
			//纬度
			if ((RsmInfo.pParticipantInfo[i].Latitude >= 90)
					|| (RsmInfo.pParticipantInfo[i].Latitude <= -90))
				RsmInfo.pParticipantInfo[i].Latitude = 0;
			//经度
			if ((RsmInfo.pParticipantInfo[i].Longitude >= 180)
					|| (RsmInfo.pParticipantInfo[i].Longitude <= -180))
				RsmInfo.pParticipantInfo[i].Longitude = 0;
			//海拔
			if ((RsmInfo.pParticipantInfo[i].Altitude >= 6000)
					|| (RsmInfo.pParticipantInfo[i].Altitude <= -400))
				RsmInfo.pParticipantInfo[i].Altitude = 0;

			//速度
			if (RsmInfo.pParticipantInfo[i].Speed > 160)
				RsmInfo.pParticipantInfo[i].Speed = 160;
			//方向
			if (RsmInfo.pParticipantInfo[i].Heading >= 360)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			if (RsmInfo.pParticipantInfo[i].Heading < 0)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			//长度
			if (RsmInfo.pParticipantInfo[i].ObjLength > 40.95)
				RsmInfo.pParticipantInfo[i].ObjLength = 40.95;
			//宽度
			if (RsmInfo.pParticipantInfo[i].ObjWidth > 10.23)
				RsmInfo.pParticipantInfo[i].ObjWidth = 10.23;
			//高度
			if (RsmInfo.pParticipantInfo[i].ObjHeight > 6.35)
				RsmInfo.pParticipantInfo[i].ObjHeight = 6.35;

		    //Add by zhz :20190823**//
			//颜色
//			if (RsmInfo.pParticipantInfo[i].Colour > 7)
//				RsmInfo.pParticipantInfo[i].Colour = PartiColor_unknown;

			//信息来源
			if (RsmInfo.pParticipantInfo[i].SourecType > 3)
				RsmInfo.pParticipantInfo[i].SourecType = SourceType_unknown;
		}

	}

	//		Rsm_UptoWeb(p_cRecvBuf, pnRecvBufSize, RsmInfo);

	if ((RsmInfo.ParticipantCnt >= 1) && (RsmInfo.ParticipantCnt <= 100))
	{
		FG_RsmTxCreateMsg(RsmInfo);
	}

	free(RsmInfo.pParticipantInfo);
	return 0;
}
int Data_Analysis_E101(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
{
	memset(&RsmInfo, 0x00, sizeof(tRsmMsg));

	int nIndex = 0;
	int i;
	//Equipment ID
	nIndex += 2;

	//Reserve
	nIndex += 2;

	//点云数据帧号
	nIndex += 4;

	//时间戳秒
	nIndex += 4;

	//时间戳微秒
	nIndex += 4;

	//激光器原点经纬度
	memcpy(&RsmInfo.RefPosLng, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;

	memcpy(&RsmInfo.RefPosLat, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;

	//x轴与正北方夹角
	memcpy(&RsmInfo.RefPosAngel, &p_cRecvBuf[nIndex], 2);
	nIndex += 2;

	//交通参与者个数
	RsmInfo.ParticipantCnt = p_cRecvBuf[nIndex];
	nIndex += 1;
	printf("RsmInfo.ParticipantCnt  = %d\n", RsmInfo.ParticipantCnt);

	//交通参与者信息
	RsmInfo.pParticipantInfo = calloc(RsmInfo.ParticipantCnt, sizeof(tParticipantInfo));

	tParticipantInfo_latlng lParticipantInfo_latlng;

	for (i = 0; i < RsmInfo.ParticipantCnt; i++)
	{
		memset(&lParticipantInfo_latlng, 0x00, sizeof(tParticipantInfo_latlng));

		//提取与数据，
		memcpy(&lParticipantInfo_latlng.ID, &p_cRecvBuf[nIndex],
				sizeof(tParticipantInfo_latlng));
		nIndex += sizeof(tParticipantInfo_latlng);

		//赋值
		RsmInfo.pParticipantInfo[i].ID = lParticipantInfo_latlng.ID;
		RsmInfo.pParticipantInfo[i].ParticType = lParticipantInfo_latlng.ParticType;

		RsmInfo.pParticipantInfo[i].Latitude = lParticipantInfo_latlng.Latitude / 10000000.0;
		RsmInfo.pParticipantInfo[i].Longitude =lParticipantInfo_latlng.Longitude / 10000000.0;
		RsmInfo.pParticipantInfo[i].Altitude = lParticipantInfo_latlng.Altitude / 100.0;

		RsmInfo.pParticipantInfo[i].Speed = lParticipantInfo_latlng.Speed/ 100.0;
		RsmInfo.pParticipantInfo[i].Heading = lParticipantInfo_latlng.Heading;
		RsmInfo.pParticipantInfo[i].ObjLength =	lParticipantInfo_latlng.ObjLength / 100.0;
		RsmInfo.pParticipantInfo[i].ObjWidth = lParticipantInfo_latlng.ObjWidth	/ 100.0;
		RsmInfo.pParticipantInfo[i].ObjHeight = lParticipantInfo_latlng.ObjHeight / 100.0;

		//数据来源，来自微波雷达
		RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;

      //Add by zhz :20190823 //
		RsmInfo.pParticipantInfo[i].SourecType = lParticipantInfo_latlng.InfoSourse;
		if(RsmInfo.pParticipantInfo[i].SourecType == 0)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 1)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_video;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 2)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_lidar;
		else
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_unknown;

//		RsmInfo.pParticipantInfo[i].Colour = lParticipantInfo_latlng.Colour;
//		RsmInfo.pParticipantInfo[i].Confidence = lParticipantInfo_latlng.Confidence;

		//printf("%f ",RsmInfo.pParticipantInfo[i].Heading);
		//赋值时的异常处理
		{
			//类型
			if (RsmInfo.pParticipantInfo[i].ParticType > 6 /*3*/)
				RsmInfo.pParticipantInfo[i].ParticType = 0;
			//纬度
			if ((RsmInfo.pParticipantInfo[i].Latitude >= 90)
					|| (RsmInfo.pParticipantInfo[i].Latitude <= -90))
				RsmInfo.pParticipantInfo[i].Latitude = 0;
			//经度
			if ((RsmInfo.pParticipantInfo[i].Longitude >= 180)
					|| (RsmInfo.pParticipantInfo[i].Longitude <= -180))
				RsmInfo.pParticipantInfo[i].Longitude = 0;
			//海拔
			if ((RsmInfo.pParticipantInfo[i].Altitude >= 6000)
					|| (RsmInfo.pParticipantInfo[i].Altitude <= -400))
				RsmInfo.pParticipantInfo[i].Altitude = 0;

			//速度
			if (RsmInfo.pParticipantInfo[i].Speed > 160)
				RsmInfo.pParticipantInfo[i].Speed = 160;
			//方向
			if (RsmInfo.pParticipantInfo[i].Heading >= 360)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			if (RsmInfo.pParticipantInfo[i].Heading < 0)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			//长度
			if (RsmInfo.pParticipantInfo[i].ObjLength > 40.95)
				RsmInfo.pParticipantInfo[i].ObjLength = 40.95;
			//宽度
			if (RsmInfo.pParticipantInfo[i].ObjWidth > 10.23)
				RsmInfo.pParticipantInfo[i].ObjWidth = 10.23;
			//高度
			if (RsmInfo.pParticipantInfo[i].ObjHeight > 6.35)
				RsmInfo.pParticipantInfo[i].ObjHeight = 6.35;

		}

#ifndef DEBUG
		printf("RsmInfo ID = %d\n", RsmInfo.pParticipantInfo[i].ID);
		printf("RsmInfo  ParticType= %d \n",
				RsmInfo.pParticipantInfo[i].ParticType);
		printf("RsmInfo  Latitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Latitude);
		printf("RsmInfo  Longitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Longitude);
		printf("RsmInfo  Altitude= %f (m)\n",
				RsmInfo.pParticipantInfo[i].Altitude);
		printf("RsmInfo  Speed= %f (m/s)\n",
				RsmInfo.pParticipantInfo[i].Speed);
		printf("RsmInfo  Heading= %f ((°))\n",
				RsmInfo.pParticipantInfo[i].Heading);
		printf("RsmInfo  ObjLength = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjLength);
		printf("RsmInfo  ObjWidth = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjWidth);
		printf("RsmInfo  ObjHeight= %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjHeight);
		printf("RsmInfo  SourecType= %d \n",
				RsmInfo.pParticipantInfo[i].SourecType);
#endif
	}

	return 0;
}

int Data_Analysis_E102(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
{
	memset(&RsmInfo, 0x00, sizeof(tRsmMsg));

	int nIndex = 0;
	int i;
	//Equipment ID
	nIndex += 2;

	//Reserve
	nIndex += 2;

	//点云数据帧号
	nIndex += 4;

	//时间戳秒
	nIndex += 4;

	//时间戳微秒
	nIndex += 4;

	//激光器原点经纬度
	memcpy(&RsmInfo.RefPosLng, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;

	memcpy(&RsmInfo.RefPosLat, &p_cRecvBuf[nIndex], 4);
	nIndex += 4;

	//x轴与正北方夹角
	memcpy(&RsmInfo.RefPosAngel, &p_cRecvBuf[nIndex], 2);
	nIndex += 2;

	//交通参与者个数
	RsmInfo.ParticipantCnt = p_cRecvBuf[nIndex];
	nIndex += 1;
	printf("RsmInfo.ParticipantCnt  = %d\n", RsmInfo.ParticipantCnt);

	//交通参与者信息
	RsmInfo.pParticipantInfo = calloc(RsmInfo.ParticipantCnt, sizeof(tParticipantInfo));

	tParticipantInfo_EarThd lParticipantInfo_EarThd;

	for (i = 0; i < RsmInfo.ParticipantCnt; i++)
	  {
		memset(&lParticipantInfo_EarThd, 0x00, sizeof(tParticipantInfo_EarThd));

		//提取与数据，
		memcpy(&lParticipantInfo_EarThd.ID, &p_cRecvBuf[nIndex], sizeof(tParticipantInfo_EarThd));
		nIndex += sizeof(tParticipantInfo_EarThd);

		//赋值
		RsmInfo.pParticipantInfo[i].ID = lParticipantInfo_EarThd.ID;
		RsmInfo.pParticipantInfo[i].ParticType = lParticipantInfo_EarThd.ParticType;

		RsmInfo.pParticipantInfo[i].Latitude = lParticipantInfo_EarThd.Latitude / 10000000.0;
		RsmInfo.pParticipantInfo[i].Longitude =lParticipantInfo_EarThd.Longitude / 10000000.0;
		RsmInfo.pParticipantInfo[i].Altitude = lParticipantInfo_EarThd.Altitude / 100.0;

		RsmInfo.pParticipantInfo[i].Speed = lParticipantInfo_EarThd.Speed/ 100.0;
		RsmInfo.pParticipantInfo[i].Heading = lParticipantInfo_EarThd.Heading;
		RsmInfo.pParticipantInfo[i].ObjLength =	lParticipantInfo_EarThd.ObjLength / 100.0;
		RsmInfo.pParticipantInfo[i].ObjWidth = lParticipantInfo_EarThd.ObjWidth	/ 100.0;
		RsmInfo.pParticipantInfo[i].ObjHeight = lParticipantInfo_EarThd.ObjHeight / 100.0;
		//数据来源，来自微波雷达
		RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;

      //Add by zhz :20190823 //
		RsmInfo.pParticipantInfo[i].SourecType = lParticipantInfo_EarThd.InfoSourse;
		if(RsmInfo.pParticipantInfo[i].SourecType == 0)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_microwaveRadar;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 1)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_video;
		else if (RsmInfo.pParticipantInfo[i].SourecType == 2)
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_lidar;
		else
			RsmInfo.pParticipantInfo[i].SourecType = SourceType_unknown;

//		RsmInfo.pParticipantInfo[i].Colour = lParticipantInfo_EarThd.Colour;
//		RsmInfo.pParticipantInfo[i].Confidence = lParticipantInfo_EarThd.Confidence;
//
//		RsmInfo.pParticipantInfo[i].X_Axis = lParticipantInfo_EarThd.X_Axis;
//		RsmInfo.pParticipantInfo[i].Y_Axis = lParticipantInfo_EarThd.Y_Axis;
//		RsmInfo.pParticipantInfo[i].Z_Axis = lParticipantInfo_EarThd.Z_Axis;

		//printf("%f ",RsmInfo.pParticipantInfo[i].Heading);
		//赋值时的异常处理
		{
			//类型
			if (RsmInfo.pParticipantInfo[i].ParticType > 6)
				RsmInfo.pParticipantInfo[i].ParticType = 0;
			//纬度
			if ((RsmInfo.pParticipantInfo[i].Latitude >= 90)
					|| (RsmInfo.pParticipantInfo[i].Latitude <= -90))
				RsmInfo.pParticipantInfo[i].Latitude = 0;
			//经度
			if ((RsmInfo.pParticipantInfo[i].Longitude >= 180)
					|| (RsmInfo.pParticipantInfo[i].Longitude <= -180))
				RsmInfo.pParticipantInfo[i].Longitude = 0;
			//海拔
			if ((RsmInfo.pParticipantInfo[i].Altitude >= 6000)
					|| (RsmInfo.pParticipantInfo[i].Altitude <= -400))
				RsmInfo.pParticipantInfo[i].Altitude = 0;

			//速度
			if (RsmInfo.pParticipantInfo[i].Speed > 160)
				RsmInfo.pParticipantInfo[i].Speed = 160;
			//方向
			if (RsmInfo.pParticipantInfo[i].Heading >= 360)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			if (RsmInfo.pParticipantInfo[i].Heading < 0)
				RsmInfo.pParticipantInfo[i].Heading = 0;
			//长度
			if (RsmInfo.pParticipantInfo[i].ObjLength > 40.95)
				RsmInfo.pParticipantInfo[i].ObjLength = 40.95;
			//宽度
			if (RsmInfo.pParticipantInfo[i].ObjWidth > 10.23)
				RsmInfo.pParticipantInfo[i].ObjWidth = 10.23;
			//高度
			if (RsmInfo.pParticipantInfo[i].ObjHeight > 6.35)
				RsmInfo.pParticipantInfo[i].ObjHeight = 6.35;

		    //Add by zhz :20190823**//
			//颜色
//			if (RsmInfo.pParticipantInfo[i].Colour > 7)
//				RsmInfo.pParticipantInfo[i].Colour = PartiColor_unknown;

			//信息来源
			if (RsmInfo.pParticipantInfo[i].SourecType > 3)
				RsmInfo.pParticipantInfo[i].SourecType = SourceType_unknown;
		}

#ifndef DEBUG
		printf("RsmInfo ID = %d\n", RsmInfo.pParticipantInfo[i].ID);
		printf("RsmInfo  ParticType= %d \n",
				RsmInfo.pParticipantInfo[i].ParticType);
		printf("RsmInfo  Latitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Latitude);
		printf("RsmInfo  Longitude= %f (° or m)\n",
				RsmInfo.pParticipantInfo[i].Longitude);
		printf("RsmInfo  Altitude= %f (m)\n",
				RsmInfo.pParticipantInfo[i].Altitude);
		printf("RsmInfo  Speed= %f (m/s)\n",
				RsmInfo.pParticipantInfo[i].Speed);
		printf("RsmInfo  Heading= %f ((°))\n",
				RsmInfo.pParticipantInfo[i].Heading);
		printf("RsmInfo  ObjLength = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjLength);
		printf("RsmInfo  ObjWidth = %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjWidth);
		printf("RsmInfo  ObjHeight= %f (m)\n",
				RsmInfo.pParticipantInfo[i].ObjHeight);
		printf("RsmInfo  SourecType= %d \n",
				RsmInfo.pParticipantInfo[i].SourecType);
#endif
	}
	if ((RsmInfo.ParticipantCnt >= 1) && (RsmInfo.ParticipantCnt <= 100))
	{
		FG_RsmTxCreateMsg(RsmInfo);
	}

	free(RsmInfo.pParticipantInfo);
	return 0;
}
/******************************************************************
* 函数名称: RsmTx_CallBack_back
* 函数功能: 收到消息的处理函数
* 函数参数: 收到消息的码流和长度
* 参数名称: p_cRecvBuf pnRecvBufSize
* 返 回 值: 无
* 函数说明: 处理收到的消息
* 修改日期:        版本号       修改人 修改内容(局限在本函数内的缺陷修改)
* -----------------------------------------------------------------
* 2020/01/15   v1.0     黄赛        修改函数
* 2020/03/10   v1.1     柴志军		修改函数
* 修改内容：解析激光雷达结构体由全局变量改为局部变量，并调用FG_RecvFromLaserMsgSet填充
* 将原来操作激光雷达结构体变量的互斥变量搬到RsmConfig接口中
******************************************************************/
void FG_RsmTxCallBackback(nint8_t *p_pcRecvBuf, uint32_t p_unRecvBufSize)
 {
//	int i = 0;
//	printf("000000000000000000000000000000000000000000000000000000000000\n");
//	for (i = 0; i < p_unRecvBufSize; i++) {
//		printf("%02x ", p_cRecvBuf[i]);
//	}
//	printf("RSM_RecvSize%d\n",p_unRecvBufSize);
    if ((0 >= p_unRecvBufSize) || (NULL == p_pcRecvBuf))
    {
        printf("--%s %s file %s line:%d\n",
               __DATE__,__TIME__,__FILE__,__LINE__);
        return;
	}
	uint32_t u32i = 0, u32j = 0;

	//局部变量解析激光雷达数据包
	rectMSGLaserToV2X_t *l_prectMsgRecvFromLaser = calloc(1, (sizeof(rectMSGLaserToV2X_t)));
	if(NULL == l_prectMsgRecvFromLaser)
	{
		printf("--%s %s file %s line:%d\n",
			__DATE__,__TIME__,__FILE__,__LINE__);
		return;
	}

	if ((0xFF == p_pcRecvBuf[0]) && (0xFF == p_pcRecvBuf[1])	\
				&& (0xFF == p_pcRecvBuf[p_unRecvBufSize-1]) && (0xE1 == p_pcRecvBuf[3]))
	{
		l_prectMsgRecvFromLaser->m_usStartByte = htons(
				*(uint16_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 2;
		l_prectMsgRecvFromLaser->m_ucSerNum = *(p_pcRecvBuf + u32i);
		u32i = u32i + 1;
		l_prectMsgRecvFromLaser->m_ucMainCmd = *(p_pcRecvBuf + u32i);
		u32i = u32i + 1;
		l_prectMsgRecvFromLaser->m_ucEnumSubCmd = *(p_pcRecvBuf + u32i);
		u32i = u32i + 1;
		l_prectMsgRecvFromLaser->m_ucStatusByte = *(p_pcRecvBuf + u32i);
		u32i = u32i + 1;
		l_prectMsgRecvFromLaser->m_usMessageLength = htons(
				*(uint16_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 2;
		l_prectMsgRecvFromLaser->m_usDevId = htons(*(uint16_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 2;
		l_prectMsgRecvFromLaser->m_usExtension = htons(
				*(uint16_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 2;
		l_prectMsgRecvFromLaser->m_unFrame = htonl(*(uint32_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 4;
		l_prectMsgRecvFromLaser->m_ulTimeSec = INT64_TO_NET(
				*(uint64_t*)(p_pcRecvBuf + u32i));
		u32i = u32i + 8;
		l_prectMsgRecvFromLaser->m_unLat = htonl(*(uint32_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 4;
		l_prectMsgRecvFromLaser->m_unLon = htonl(*(uint32_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 4;
		l_prectMsgRecvFromLaser->m_usAngle = htons(*(uint16_t*) (p_pcRecvBuf + u32i));
		u32i = u32i + 2;
		l_prectMsgRecvFromLaser->m_ucParticipantsNum = *(p_pcRecvBuf + u32i);
		u32i = u32i + 1;
		/* 5个扩展字段 */
		u32i = u32i + 5;
		if (LaserRawData == l_prectMsgRecvFromLaser->m_ucEnumSubCmd)
		{
			for (u32j = 0; u32j < l_prectMsgRecvFromLaser->m_ucParticipantsNum;
					u32j++) {
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usId = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_ucEnumtTrafficType =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_ucConfidenceLevel =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_ucEnumMyColor =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_ucEnumFrom =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				/* 2个扩展字段 */
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usCoordinate_X =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usCoordinate_Y =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usCoordinate_Z =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usSpeed = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usDirection = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usLongth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usWidth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub1[u32j].m_usHeight = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				/* 4个扩展字段      */
				u32i = u32i + 4;
			}
		}
		else if (GeodeticLatLon == l_prectMsgRecvFromLaser->m_ucEnumSubCmd)
		{
			for (u32j = 0; u32j < l_prectMsgRecvFromLaser->m_ucParticipantsNum;
					u32j++) {
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usId = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_ucEnumtTrafficType =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_ucConfidenceLevel =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_ucEnumMyColor =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_ucEnumFrom =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				/* 2个扩展字段 */
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_unLat = htonl(
						*(uint32_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 4;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_unLon = htonl(
						*(uint32_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 4;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usAltitude = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usSpeed = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usHeadAnagle =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usLongth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usWidth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub2[u32j].m_usHeight = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				/* 4个扩展字段      */
				u32i = u32i + 4;
			}
		}
		else if (LaserAndGeodetic == l_prectMsgRecvFromLaser->m_ucEnumSubCmd)
		{
			for (u32j = 0; u32j < l_prectMsgRecvFromLaser->m_ucParticipantsNum;
					u32j++) {

				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usId = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_ucEnumtTrafficType =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_ucConfidenceLevel =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_ucEnumMyColor =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_ucEnumFrom =
						*(p_pcRecvBuf + u32i);
				u32i = u32i + 1;
				/* 2个扩展字段 */
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_unLat = htonl(
						*(uint32_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 4;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_unLon = htonl(
						*(uint32_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 4;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usAltitude = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usSpeed = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usHeadAnagle =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usLongth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usWidth = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usHeight = htons(
						*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usCoordinate_X =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usCoordinate_Y =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				l_prectMsgRecvFromLaser->choice.sub3[u32j].m_usCoordinate_Z =
						htons(*(uint16_t*) (p_pcRecvBuf + u32i));
				u32i = u32i + 2;
				/* 2个扩展字段      */
				u32i = u32i + 2;
			}
		}
		else
		{
			printf("--%s %s file %s line:%d\n",
			__DATE__, __TIME__, __FILE__, __LINE__);
		}

	}
	//填充接收到的激光雷达数据结构变量
	FG_RecvFromLaserMsgSet(l_prectMsgRecvFromLaser);
	RsmFrame.In++;
	RsmFrame.In %= SHARE_MAXNUM;
	free(l_prectMsgRecvFromLaser);
    return;
}
//void CallBack_Listen_Rsm(StcUdpClient *g_UdpClient)
//{
//	g_UdpClient->pCallBackDealData = RsmTx_CallBack_back;
//	return;
//}
//int Rsm_UdpSer_Init(int nSerPort) {
//	int nRet = 0;
//	g_StcUdpServer_Rsm.m_selfPort = nSerPort;
//	UdpSrv_InitSoc(&g_StcUdpServer_Rsm);
//	g_StcUdpServer_Rsm.pCallBackDealData = RsmTx_CallBack_back;
//	nRet = UdpSrv_LinkSer(&g_StcUdpServer_Rsm);
//	if (nRet < 0) {
//		printf("nRet = %d \n", nRet);
//		return -1;
//	}
//	return nRet;
//}

int FG_RsmUdpSerInit(int nSerPort) {
	int nRet = 0;
	g_UdpClient_Rsm.m_selfPort = nSerPort;
	UdpClt_InitSoc(&g_UdpClient_Rsm);
	g_UdpClient_Rsm.pCallBackDealData = FG_RsmTxCallBackback;
	nRet = UdpClt_LinkSer(&g_UdpClient_Rsm,"192.168.48.3",5001);
	if (nRet < 0) {
		printf("nRet = %d \n", nRet);
		return -1;
	}
	return nRet;
}
