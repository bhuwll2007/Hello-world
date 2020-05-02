#include "EncodeYT_cJSON.h"
#define _PRINT_INFO_ 0

stcMsg_VeH g_stcMsg_VeH;

int Factory_String(char *src, int srclen, char *des)
{
	memset(des, 0x00, srclen * 2 + 1);
	int i = 0;
	for (i = 0; i < srclen; i++)
	{
		sprintf(&des[i * 2], "%02X", (unsigned char) src[i]);
#if _PRINT_INFO_
		printf("des = %s\n",des);
#endif
	}
	return 0;
}
int Factory_BitString(char *src, int srclen, char *des)
{
	memset(des, 0x00, srclen * 2 + 1);
	int i = 0;
	for (i = 0; i < srclen; i++)
	{
		sprintf(&des[i], "%01X", (unsigned char) src[i]);
	}
	return 0;
}
int Factory_local_info(stclocal_info plocal_info, cJSON *cJMsg_local_info)
{
	cJSON_AddNumberToObject(cJMsg_local_info, "latitude", plocal_info.latitude);
	cJSON_AddNumberToObject(cJMsg_local_info, "longitude",
			plocal_info.longitude);
	cJSON_AddNumberToObject(cJMsg_local_info, "heading", plocal_info.heading);
	cJSON_AddNumberToObject(cJMsg_local_info, "speed", plocal_info.speed);

	return 0;
}
int Factory_remote(stcremoteinfo premote, cJSON *cJMsg_remote)
{
	char des[100];
	Factory_String(premote.ID, 8, des);
	cJSON_AddStringToObject(cJMsg_remote, "ID", des);
	cJSON_AddNumberToObject(cJMsg_remote, "heading", premote.heading);
	cJSON_AddNumberToObject(cJMsg_remote, "direction", premote.direction);
	cJSON_AddNumberToObject(cJMsg_remote, "distance", premote.distance);
	cJSON_AddNumberToObject(cJMsg_remote, "lon", premote.lon);
	cJSON_AddNumberToObject(cJMsg_remote, "lat", premote.lat);
	cJSON_AddNumberToObject(cJMsg_remote, "speed", premote.speed);
	cJSON_AddNumberToObject(cJMsg_remote, "type", premote.type);
	cJSON_AddNumberToObject(cJMsg_remote, "elevation", premote.elevation);
	cJSON_AddNumberToObject(cJMsg_remote, "width", premote.width);
	cJSON_AddNumberToObject(cJMsg_remote, "length", premote.length);
	cJSON_AddNumberToObject(cJMsg_remote, "timestamp", premote.timestamp);
	cJSON_AddNumberToObject(cJMsg_remote, "lights", premote.lights);
	cJSON_AddNumberToObject(cJMsg_remote, "events", premote.events);
	return 0;
}
int Factory_OutLine(stcOutLine pOutLine, cJSON *cJOutLine)
{
	cJSON *cJPoint1;
	cJSON *cJPoint2;
	cJSON *cJPoint3;
	cJSON *cJPoint4;
	cJSON_AddItemToObject(cJOutLine, "Point1", cJPoint1 = cJSON_CreateObject());
	cJSON_AddNumberToObject(cJPoint1, "x", pOutLine.Point1.x);
	cJSON_AddNumberToObject(cJPoint1, "y", pOutLine.Point1.y);
	cJSON_AddItemToObject(cJOutLine, "Point2", cJPoint2 = cJSON_CreateObject());
	cJSON_AddNumberToObject(cJPoint2, "x", pOutLine.Point2.x);
	cJSON_AddNumberToObject(cJPoint2, "y", pOutLine.Point2.y);
	cJSON_AddItemToObject(cJOutLine, "Point3", cJPoint3 = cJSON_CreateObject());
	cJSON_AddNumberToObject(cJPoint3, "x", pOutLine.Point3.x);
	cJSON_AddNumberToObject(cJPoint3, "y", pOutLine.Point3.y);
	cJSON_AddItemToObject(cJOutLine, "Point4", cJPoint4 = cJSON_CreateObject());
	cJSON_AddNumberToObject(cJPoint4, "x", pOutLine.Point4.x);
	cJSON_AddNumberToObject(cJPoint4, "y", pOutLine.Point4.y);
	return 0;
}
int Factory_PTCObj(stcPTCObjs pPTCObj, cJSON *obj)
{
	cJSON_AddNumberToObject(obj, "PTCType", pPTCObj.PTCType);
	cJSON_AddNumberToObject(obj, "Source", pPTCObj.Source);
	cJSON_AddNumberToObject(obj, "PTCId", pPTCObj.PTCId);
	cJSON_AddNumberToObject(obj, "Lonitude", pPTCObj.Lonitude);
	cJSON_AddNumberToObject(obj, "Latitude", pPTCObj.Latitude);
//	6 OutLine 对象 参与者轮廓,参看Point2Ds信息 Point2Ds结构
	cJSON *cJOutLine = NULL;
	cJSON_AddItemToObject(obj, "OutLine", cJOutLine = cJSON_CreateObject());
	Factory_OutLine(pPTCObj.mOutLine, cJOutLine);
	cJSON_AddNumberToObject(obj, "Speed", pPTCObj.Speed);
	cJSON_AddNumberToObject(obj, "Heading", pPTCObj.Heading);
	return 0;
}
int Factory_light_info(stclight_info plight_info, cJSON *obj)
{
	//	1: light_type	整型 1:圆形	2:左转箭头	3:直行箭头	4:右转箭头	5:掉头	6:闪烁 当前信号灯的形状
//		int light_type;
	cJSON_AddNumberToObject(obj, "light_type", plight_info.light_type);
	//	2: current_status 整型	当前信号灯状态	(红/绿/黄) 3:红灯	6:绿灯 7:黄灯
//		int current_status;
	cJSON_AddNumberToObject(obj, "current_status", plight_info.current_status);
	//	3: current_left_time 整型当前信号灯状态剩余时间 ≥ 0 单位:秒
//		int current_left_time;
	cJSON_AddNumberToObject(obj, "current_left_time",
			plight_info.current_left_time);
	//	4: speed_advice 对象 速度建议,车辆驶向有信号灯的路口时,提供建议车速,以便不停车地、舒适地通过路口 speed_advice结构 详见speed_advice说明
//		stcspeed_advice mspeed_advice;

	cJSON *speed_advice = NULL;
	cJSON_AddItemToObject(obj, "speed_advice", speed_advice =
			cJSON_CreateObject());

	cJSON_AddNumberToObject(speed_advice, "speed_advice_value",
			plight_info.mspeed_advice.speed_advice_value);
	cJSON_AddNumberToObject(speed_advice, "speed_advice_type",
			plight_info.mspeed_advice.speed_advice_type);
	return 0;
}

int Factory_warning_point(stcwarning_point pwarning_point, cJSON *cJobj)
{
	//	序号 成员名称 数据类型 描述	取值	备注
	//	1 latitude 浮点型 交通事故发生位置纬度	-90~90,(例如:	23.4567891)单位:度
	cJSON_AddNumberToObject(cJobj, "latitude", pwarning_point.latitude);
	//	2 longitude 浮点型 交通事故发生位置经度	-180~180,(例如:	43.1234567单位:度
	cJSON_AddNumberToObject(cJobj, "longitude", pwarning_point.longitude);
	//	3 elevation 整型 交通事故发生位置海拔 整数:-4096~61439,-4096代表无效值单位:分米
	cJSON_AddNumberToObject(cJobj, "elevation", pwarning_point.elevation);
	return 0;
}
int Factory_Msg101(stcMsg_101 pMsg_101, cJSON *cJMsg_101)
{
	cJSON *local_info = NULL;
	cJSON *remote_list = NULL;
	cJSON_AddItemToObject(cJMsg_101, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_101.mlocal_info, local_info);

	cJSON_AddNumberToObject(cJMsg_101, "remote_num", pMsg_101.remote_num);

	cJSON_AddItemToObject(cJMsg_101, "remote_list", remote_list =
			cJSON_CreateArray());
	int i = 0;
	for (i = 0; i < pMsg_101.remote_num; i++)
	{
		cJSON * obj = NULL;
		cJSON_AddItemToArray(remote_list, obj = cJSON_CreateObject());
		Factory_remote(pMsg_101.mremoteinfo[i], obj);
	}

	return 0;
}
int Factory_Msg102(stcMsg_102 pMsg_102, cJSON *cJMsg_102)
{
	cJSON *local_info = NULL;
	cJSON *remote = NULL;
	cJSON_AddItemToObject(cJMsg_102, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_102.local_info, local_info);

	cJSON_AddItemToObject(cJMsg_102, "remote", remote = cJSON_CreateObject());
	Factory_remote(pMsg_102.remote, remote);

	cJSON_AddNumberToObject(cJMsg_102, "alert_type", pMsg_102.alert_type);
	cJSON_AddNumberToObject(cJMsg_102, "alert_level", pMsg_102.alert_level);
	return 0;
}

int Factory_Msg103(stcMsg_103 pMsg_103, cJSON *cJMsg_103)
{
	char des[100];
	cJSON *local_info = NULL;
//	cJSON *remote = NULL;
	cJSON_AddItemToObject(cJMsg_103, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_103.local_info, local_info);

	cJSON_AddStringToObject(cJMsg_103, "time", pMsg_103.time);

	Factory_String(pMsg_103.spat_ID, 8, des);
	cJSON_AddStringToObject(cJMsg_103, "spat_ID", des);
//	double spat_longitude;
	cJSON_AddNumberToObject(cJMsg_103, "spat_longitude",
			pMsg_103.spat_longitude);
	//	5 spat_latitude 浮点型 信号灯的纬度 -90~90,(例如:23.4567891)单位:度
	cJSON_AddNumberToObject(cJMsg_103, "spat_latitude", pMsg_103.spat_latitude);
	//	6 distance 整型 车辆与信号灯路口距离 ≥ 0 单位: 0.1米
	cJSON_AddNumberToObject(cJMsg_103, "distance", pMsg_103.distance);
	//	7 allowed_maneuvers bit string 表示车辆所在车道各种允许的通行行为 12 字节 详见 allowed_maneuvers
	Factory_BitString(pMsg_103.allowed_maneuvers, 12, des);
	cJSON_AddStringToObject(cJMsg_103, "allowed_maneuvers", des);
	//	8 lights_number 整型 当前路口信号灯的个数 ≥ 0	对象中的成员顺序对应着	信号灯从左到右排列
//		int lights_number;
	cJSON_AddNumberToObject(cJMsg_103, "lights_number", pMsg_103.lights_number);
	//	9 lights_list 对象 当前路口信号灯信息 light_info结构
//	stclight_info mlight_info[5];
	cJSON *mlight_infos = NULL;
	cJSON_AddItemToObject(cJMsg_103, "lights_list", mlight_infos =
			cJSON_CreateArray());
	int i = 0;
	for (i = 0; i < pMsg_103.lights_number; i++)
	{
		cJSON *obj = NULL;
		cJSON_AddItemToArray(mlight_infos, obj = cJSON_CreateObject());
		Factory_light_info(pMsg_103.mlight_info[i], obj);
	}
	//	10 red_light_violate 整型 闯红灯危险预警 0:无闯红灯危险	1:有 闯红灯危险,当无法获取到车道信息时,使用车辆转向灯信号进行判定
	cJSON_AddNumberToObject(cJMsg_103, "red_light_violate",
			pMsg_103.red_light_violate);
	return 0;
}
int Factory_Msg105(stcMsg_105 pMsg_105, cJSON *cJMsg_105)
{
	char des[100];
	//1 local_info 对象 本车信息 local_info 结构 参考local_info结构
	cJSON *local_info = NULL;
	cJSON_AddItemToObject(cJMsg_105, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_105.local_info, local_info);

	//	2 time 字符串 终端时间 YYYYMMDDHHMMSS (例 如	20170314000000)
	cJSON_AddStringToObject(cJMsg_105, "time", pMsg_105.time);
	//	3 RSI_ID 以string存储的十六进制数 路边单元 ID 8字节(16进制数)

	Factory_String(pMsg_105.RSI_ID, 8, des);
	cJSON_AddStringToObject(cJMsg_105, "RSI_ID", des);

	/**4 RSI_type	整型		路边单元预警类型。 Type = 0代表直接向用户显示 desecrption字段中的字符串。 0:未知类型
	 2:限速预警	 3:道路施工	 4:前方桥梁	 5:前方隧道	 6:道路湿滑	 7:速度引导	 8:超速预警
	 23:前方急转弯	 24:前方较深积水	 25:前方路面有深坑	 26:前方拥堵提醒
	 30:前方落石	 31:连续弯路	 32:禁止超车	 33:限制高度	详见alert_type中类别 是V2I RSI的预警类型*/
	cJSON_AddNumberToObject(cJMsg_105, "RSI_type", pMsg_105.RSI_type);
	/**	5 value	整型	RSI 预警对应值 若RSA的预警类型为前方道路拥堵,则1:畅通2:基本畅通3:轻度拥堵4:中度拥堵5:严重拥堵若为其他预警类型,则value代表速度值,单位	为km/h
	 */
	cJSON_AddNumberToObject(cJMsg_105, "value", pMsg_105.value);
	//	6 distance 整型 距离报警位置的距离	≥ 0	单位 :米
	cJSON_AddNumberToObject(cJMsg_105, "distance", pMsg_105.distance);
	//	7 RSI_latitude 浮点型 交通事件发生位置的纬度	-90~90,( 例如23.4567891)	单位:度
	cJSON_AddNumberToObject(cJMsg_105, "RSI_latitude", pMsg_105.RSI_latitude);
	//	8 RSI_longitude 浮点型 交通事件发生位置的经度	-180~180,(例如:43.1234567)	单位:度
	cJSON_AddNumberToObject(cJMsg_105, "RSI_longitude", pMsg_105.RSI_longitude);
	//	9 description 字符串 进一步描述预警信息,当RSI_type =0时,必须填写此字段
	cJSON_AddStringToObject(cJMsg_105, "description", pMsg_105.description);
	//	10 nr_point 整型 表示 有 多 少 个warning_point	数组	整数:2~32
	cJSON_AddNumberToObject(cJMsg_105, "nr_point", pMsg_105.nr_point);
	//	11	warning_point_list 用于描述事件相关的道路范围,可描	述非直线范围。由多warning_point组成的数组序列,该序列以多个在真实道路上标 定 的warning_point 点位刻画一段和该事件相关的路径范围多个 warning_point 结构组成的数组 参考warning_point 结构
	cJSON *mwarning_points = NULL;
	cJSON_AddItemToObject(cJMsg_105, "warning_point_list", mwarning_points =
			cJSON_CreateArray());
	int i = 0;
	for (i = 0; i < pMsg_105.nr_point; i++)
	{
		cJSON * obj = NULL;
		cJSON_AddItemToArray(mwarning_points, obj = cJSON_CreateObject());
		Factory_warning_point(pMsg_105.warning_point_list[i], obj);
	}

	return 0;
}
int Factory_Msg106(stcMsg_106 pMsg_106, cJSON *cJMsg_106)
{
	char des[100];
	cJSON *local_info = NULL;
	cJSON_AddItemToObject(cJMsg_106, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_106.local_info, local_info);

	Factory_String(pMsg_106.remote_ID, 8, des);
	cJSON_AddStringToObject(cJMsg_106, "remote_ID", des);
	cJSON_AddNumberToObject(cJMsg_106, "alert_type", pMsg_106.alert_type);

	return 0;
}
int Factory_Msg107(stcMsg_107 pMsg_107, cJSON *cJMsg_107)
{
	char des[100];
	cJSON *local_info = NULL;
	cJSON_AddItemToObject(cJMsg_107, "local_info", local_info =
			cJSON_CreateObject());
	Factory_local_info(pMsg_107.local_info, local_info);

	Factory_String(pMsg_107.person_ID, 8, des);
	cJSON_AddStringToObject(cJMsg_107, "person_ID", des);
//	cJSON_AddStringToObject(cJMsg_107, "person_ID", pMsg_107.person_ID);
	cJSON_AddNumberToObject(cJMsg_107, "alert_distance",
			pMsg_107.alert_distance);
	cJSON_AddNumberToObject(cJMsg_107, "alert_direction",
			pMsg_107.alert_direction);
	return 0;
}
int Factory_Msg108(stcMsg_108 pMsg_108, cJSON *cJMsg_108)
{
	char des[100];
	Factory_String(pMsg_108.LidarID, 8, des);
	cJSON_AddStringToObject(cJMsg_108, "LidarID", des);
	cJSON_AddNumberToObject(cJMsg_108, "PTCNum", pMsg_108.PTCNum);

	cJSON *mPTCObjs = NULL;
	cJSON_AddItemToObject(cJMsg_108, "PTCObjs", mPTCObjs = cJSON_CreateArray());
	int i = 0;
	for (i = 0; i < pMsg_108.PTCNum; i++)
	{
		cJSON * obj = NULL;
		cJSON_AddItemToArray(mPTCObjs, obj = cJSON_CreateObject());
		Factory_PTCObj(pMsg_108.mPTCObjs[i], obj);
	}

	return 0;
}
