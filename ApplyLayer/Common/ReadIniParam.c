/*
 * ReadIniParam.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#include "ReadIniParam.h"
#define PRINT_DevInfo2WebParamInfo 0
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int ReadParam_V2XComm() {
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int ReadParam_Algorithm(char *pConfigFileName) {
	char pConfigString[30] = { 0 };

	//算法参数初始化
	InitV2VParameter();
	InitSpatParameter();
	InitV2IParameter();
	InitRSMParameter();

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "MapMatch_Dis",
			pConfigString)))
		g_V2V_Parameter.MapMatch_ConfidenceDis_toLink = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"isDanger_hv_warning_time", pConfigString)))
		g_V2V_Parameter.isDanger_hv_warning_time = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "isDanger_hv_speed_low",
			pConfigString)))
		g_V2V_Parameter.isDanger_hv_speed_low = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "VehLOC_SafeDis_Value",
			pConfigString)))
		g_V2V_Parameter.VehLOC_SafeDis_Value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "ICW_TTC_SafeDisVeh",
			pConfigString)))
		g_V2V_Parameter.ICW_TTC_SafeDisVeh = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"AVW_Speed_threshold_value", pConfigString)))
		g_V2V_Parameter.AVW_Speed_threshold_value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"FCW_TTC_threshold_value", pConfigString)))
		g_V2V_Parameter.FCW_TTC_threshold_value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"FCW_THW_threshold_value", pConfigString)))
		g_V2V_Parameter.FCW_THW_threshold_value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "LCW_Blind_Spot_Angle",
			pConfigString)))
		g_V2V_Parameter.LCW_Blind_Spot_Angle = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V", "LCW_Blind_Spot_Width",
			pConfigString)))
		g_V2V_Parameter.LCW_Blind_Spot_Width = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"LCW_Blind_Spot_ToMirror", pConfigString)))
		g_V2V_Parameter.LCW_Blind_Spot_ToMirror = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "V2V",
			"ICW_TTC_threshold_value", pConfigString)))
		g_V2V_Parameter.ICW_TTC_threshold_value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "LightDir_Dis_Value",
			pConfigString)))
		g_Spat_Parameter.LightDir_Dis_Value = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "WarnCalcTime",
			pConfigString)))
		g_Spat_Parameter.WarnCalcTime = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "SpeedLimit_High",
			pConfigString)))
		g_Spat_Parameter.SpeedLimit_High = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "SpeedLimit_Low",
			pConfigString)))
		g_Spat_Parameter.SpeedLimit_Low = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "SPATWarnDis",
			pConfigString)))
		g_Spat_Parameter.SPATWarnDis = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "SPAT", "DistoCrossRdMid",
			pConfigString)))
		g_Spat_Parameter.DistoCrossRdMid = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "RSI", "IVSSpeakDis",
			pConfigString)))
		g_V2I_Parameter.IVSSpeakDis = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "RSI", "IVSWarnDis",
			pConfigString)))
		g_V2I_Parameter.IVSWarnDis = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "MAP", "IsMapMode",
			pConfigString)))
		g_IsMapMode = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "RSM", "Laser_Display",
			pConfigString)))
		g_RSM_Parameter.Laser_Display = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "RSM", "Laser_Lat",
			pConfigString)))
		g_RSM_Parameter.Laser_Lat = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "RSM", "Laser_Long",
			pConfigString)))
		g_RSM_Parameter.Laser_Long = atof(pConfigString);

	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int ReadParam_V2XRun(char *pConfigFileName) {
	//chushihua
	memset(&g_tAPP_RunParam, 0x00, sizeof(tAPP_RunParam));
	g_tAPP_RunParam.nInterval_UpD1 = 1000;
	g_tAPP_RunParam.nInterval_UpD0 = 100;
	g_tAPP_RunParam.nInterval_UpD9 = 200;

	int ConfigVal = 0;
	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Switch_UpD1",
			&ConfigVal)))
		g_tAPP_RunParam.nSwitch_UpD1 = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Switch_UpD0",
			&ConfigVal)))
		g_tAPP_RunParam.nSwitch_UpD0 = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Switch_UpD9",
			&ConfigVal)))
		g_tAPP_RunParam.nSwitch_UpD9 = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Interval_UpD1",
			&ConfigVal)))
		g_tAPP_RunParam.nInterval_UpD1 = ConfigVal;
	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Interval_UpD0",
			&ConfigVal)))
		g_tAPP_RunParam.nInterval_UpD0 = ConfigVal;

	if (!(GetConfigIntValue(pConfigFileName, "AppUpData", "Interval_UpD9",
			&ConfigVal)))
		g_tAPP_RunParam.nInterval_UpD9 = ConfigVal;

	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int ReadParam_V2XApp(char *pConfigFileName) {
	char pConfigString[30] = { 0 };

	int ConfigVal = 0;
	if (!(GetConfigIntValue(pConfigFileName, "SPATTx", "IntersectionId",
			&ConfigVal)))
		g_nIntersectionId = ConfigVal;

	//清空全局变量
	memset(&g_ReadHisDataBase.u32SaveTxBsmEnable, 0x00,
			sizeof(ReadHisDataBase));

	if (!(GetConfigStringValue(pConfigFileName, "DataBase", "SaveTxBsmEnable",
			pConfigString)))
		g_ReadHisDataBase.u32SaveTxBsmEnable = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_BSM", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_BSM = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_RSI", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_RSI = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_RSM", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_RSM = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_MAP", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_MAP = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_SPAT", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_SPAT = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_APP_ALL", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_APP_ALL = atof(pConfigString);

	if (!(GetConfigStringValue(pConfigFileName, "DataBase",
			"HisDataBaseIndex_RX_ALL", pConfigString)))
		g_ReadHisDataBase.u32HisDataBaseIndex_RX_ALL = atof(pConfigString);

	return 0;
}

int ReadParam_YuTongIni(char *pConfigFileName) 
{
	char pConfigString[30] = { 0 };

	//初始化
	memset(&g_tYuTong_SpatInfo, 0x00, sizeof(tYuTong_SpatInfo));
	memset(&g_tYuTong_RsuBaseInfo, 0x00, sizeof(tYuTong_RsuBaseInfo));

	//读取宇通公交优先通行的相关参数
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_BusFirst",
			"BusFirst_BeginDis", pConfigString)))
		g_tYuTong_SpatInfo.BusFirst_BeginDis = atoi(pConfigString);

	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_BusFirst", "RegionID",
			pConfigString)))
		g_tYuTong_SpatInfo.RegionID = atoi(pConfigString);

	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_BusFirst",
			"IntersectionID", pConfigString)))
		g_tYuTong_SpatInfo.IntersectionID = atoi(pConfigString);

	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_BusFirst", "SpatSelId",
			pConfigString)))
		memcpy(g_tYuTong_SpatInfo.SpatSelId, pConfigString,
				strlen(pConfigString));
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_BusFirst", "RsmSelId",
			pConfigString)))
		memcpy(g_tYuTong_SpatInfo.RsmSelId, pConfigString,
				strlen(pConfigString));

	//读取 路侧设备相关参数
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"Is_Send2Cloud", pConfigString)))
		g_tYuTong_RsuBaseInfo.Is_Send2Cloud = atoi(pConfigString);

	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"RsuSelId", pConfigString)))
		memcpy(g_tYuTong_RsuBaseInfo.RsuSelId, pConfigString,
				strlen(pConfigString));
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo", "RsuName",
			pConfigString)))
		memcpy(g_tYuTong_RsuBaseInfo.RsuName, pConfigString,
				strlen(pConfigString));
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"RsuIPAddr", pConfigString)))
		memcpy(g_tYuTong_RsuBaseInfo.RsuIPAddr, pConfigString,
				strlen(pConfigString));
	memset(pConfigString, 0x00, sizeof(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"ProjectId", pConfigString)))
		memcpy(g_tYuTong_RsuBaseInfo.ProjectId, pConfigString,
				strlen(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"CloudIpAdd", pConfigString)))
		memcpy(g_tYuTong_RsuBaseInfo.CloudIpAdd, pConfigString,
				strlen(pConfigString));
	if (!(GetConfigStringValue(pConfigFileName, "YuTong_RsuBaseInfo",
			"CloudPort", pConfigString)))
		g_tYuTong_RsuBaseInfo.CloudPort = atoi(pConfigString);
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int ReadParam() {
	int nRet = ReadParam_V2XComm();
	if (nRet < 0) {
		return -1;
	}
	if ((access("./AlgorithmCfg.ini", F_OK)) == 0) {
		nRet = ReadParam_Algorithm("./AlgorithmCfg.ini");
		if (nRet < 0) {
			return -2;
		}
	}
	if ((access("./HostInfo.ini", F_OK)) == 0) {
		nRet = ReadParam_V2XRun("./HostInfo.ini");
		if (nRet < 0) {
			return -3;
		}
	}
	if ((access("./HostInfo.ini", F_OK)) == 0) {
		nRet = ReadParam_V2XApp("./HostInfo.ini");
		if (nRet < 0) {
			return -4;
		}
	}

	if ((access("./YuTong_0412.ini", F_OK)) == 0) 
	{
		nRet = ReadParam_YuTongIni("./YuTong_0412.ini");
		if (nRet < 0) 
		{
			return -6;
		}
	}
	return 0;
}

