#ifndef __ENCODE_YT_CJSON_H_
#define __ENCODE_YT_CJSON_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>

#include <unistd.h>
#include <sys/shm.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "../../Algorithm/cJSON.h"

typedef struct local_info
{
	//浮点型 本车纬度 -90~90,(例如:23.4567891) 单位:度
	double latitude;
	//浮点型 本车经度 -180~180,(例如:43.1234567) 单位:度
	double longitude;
	//整型 本车行驶方向与正北方向的夹角 0~360,360表示未知 单位:度
	int heading;
	//浮点型 本车行驶速度 0~589.752,589.752为无效值 单位:km/h
	double speed;
} stclocal_info;
typedef struct remoteinfo
{
	//以string存储的十六进制数 车辆id 8字节(16进制)
	char ID[8];
	//整型 航向角 0-360,360表示未知 单位:度
	int heading;
	// 整型 相对于本车的方位 单位:度
	/*1:十字路口左前交叉
	 2:十字路口右前交叉
	 4:正前方同向同车道
	 8:左前方同向邻车道
	 16:右前方同向邻车道
	 32:左前方同向远车道
	 64:右前方同向远车道
	 128:正后方同向同车道
	 256:左后方同向邻车道
	 512:右后方同向邻车道
	 1024:左后方同向远车道
	 2048:右后方同向远车道
	 4096:正前方逆向同车道
	 8192:左前方逆向邻车道
	 16384:右前方逆向邻车道
	 32768:左前方逆向远车道
	 65536: 右前方逆向远车道
	 其它值:无效方向*/
	int direction;
	// 整型 相对于本车的距离  ≥ 0 单位:米
	int distance;
	// 浮点数 远车经度 -180~180,(例如:	43.1234567)	单位:度
	double lon;

	// 浮点型 远车纬度 -90~90,(例如:23.4567891)	单位:度
	double lat;
	// 浮点型 远车行驶速度 0~589.752,589.752为无效值 单位:km/h
	double speed;
	// 整型 远车类型1:普通车辆	2:紧急车辆	3:摩托车
	int type;
	//整型 远车海拔 整数:-4096~61439,-4096代表	无效值 单位:分米
	int elevation;
	//整型 远车宽度 整数:0~1023,0为无效值 单位:厘米
	int width;
	// 整型 远车长度 整数:0~4095,0为无效值 单位:厘米
	int length;
	// 整型 远车发送消息的时间戳
	/*前 10 字节:Seconds since the
	 Epoch.
	 后 10字节:Miliseconds since
	 the last Second*/
	int timestamp;
	// 整型 远车车灯状态
	/*0:所有灯关闭;
	 4:左转灯亮起;
	 8:右转灯亮起*/
	int lights;
	// 整型 远车车辆状态信息
	/*
	 0: 无事件
	 4: 制动防抱死系统功能触发引发危险
	 8:牵引力控制系统功能触发引发危险
	 16:车身稳定性系统功能触发引	发危险
	 64:车道偏移预警系统功能触发
	 引发危险
	 128: 刹车
	 2048: 车辆失控
	 */
	int events;

} stcremoteinfo;
typedef struct speed_advice
{
//	1 speed_advice_valu 浮点型 速度建议值 0~589.752, 589.752为无效值 单位:km/h
	double speed_advice_value;
//	2 speed_advice_type 整型 速度的建议类型描述 0~11 详见speed_advice_type
	int speed_advice_type;
}stcspeed_advice;
typedef struct light_info
{
//	1: light_type	整型 1:圆形	2:左转箭头	3:直行箭头	4:右转箭头	5:掉头	6:闪烁 当前信号灯的形状
	int light_type;
//	2: current_status 整型	当前信号灯状态	(红/绿/黄) 3:红灯	6:绿灯 7:黄灯
	int current_status;
//	3: current_left_time 整型当前信号灯状态剩余时间 ≥ 0 单位:秒
	int current_left_time;
//	4: speed_advice 对象 速度建议,车辆驶向有信号灯的路口时,提供建议车速,以便不停车地、舒适地通过路口 speed_advice结构 详见speed_advice说明
	stcspeed_advice mspeed_advice;

}stclight_info;
typedef struct Point2D
{
	double x;//浮点数 经度
	double y;//浮点数 纬度
}stcPoint2D;
typedef struct OutLine
{
	/**
	 * 经纬度,目标物外接矩形的四个顶点。大地坐标系下,逆时针排列4个点。
	 */
	stcPoint2D Point1;
	stcPoint2D Point2;
	stcPoint2D Point3;
	stcPoint2D Point4;
}stcOutLine;
typedef struct PTCObjs
{
/*	1 PTCType 整型 参与者类型 0 unkwon(未定义)
							1 Motor(机动车)
							2 Non-motor(非机动车)
							3 Pedestrian(行人)
	*/
	int PTCType;
	/*2 Source 整型 检测类型
	 * 	0 unkown(未定义)
		1 selfinfo(自车信息)
		2 LTE-V
		3 Video(视频)
		4 MicowaveRadar(微波雷达)
		5 Loop
	 */
	int Source;
//	3 PTCId 整型 参与者临时ID
	int PTCId;
//	4 Lonitude 浮点型 位置经度
	double Lonitude;
//	5 Latitude 浮点型 位置纬度
	double Latitude;
//	6 OutLine 对象 参与者轮廓,参看Point2Ds信息 Point2Ds结构
	stcOutLine mOutLine;
//	7 Speed 浮点数 速度 单位:km/h
	double Speed;
//	8 Heading 浮点数 方向 单位:度
	double Heading;
}stcPTCObjs;

/**
 * warning_point 结构
 */
typedef struct _warning_point
{
//	序号 成员名称 数据类型 描述	取值	备注
//	1 latitude 浮点型 交通事故发生位置纬度	-90~90,(例如:	23.4567891)单位:度
	double latitude;
//	2 longitude 浮点型 交通事故发生位置经度	-180~180,(例如:	43.1234567单位:度
	double longitude;
//	3 elevation 整型 交通事故发生位置海拔 整数:-4096~61439,-4096代表无效值单位:分米
	int elevation;
}stcwarning_point;

typedef struct Msg_101
{
	stclocal_info mlocal_info;
	int remote_num;
	stcremoteinfo mremoteinfo[16];
} stcMsg_101;

typedef struct Msg_102
{
	stclocal_info local_info;
	stcremoteinfo remote;
	int alert_type;
	int alert_level;
} stcMsg_102;
typedef struct Msg_103
{
	//序号 成员名称 数据类型 描述 取值 备注
	//1 local_info 对象 本车信息 local_info结构 参考localinfo结构
	stclocal_info local_info;
	//2 time 字符串	终端时间 YYYYMMDDHHMMSS(例如:20170314000000)
	char time[20];
//	3 spat_ID 字符串以 string存储的十六进制数 信号灯ID 8 字节(16进制数)
	char spat_ID[8];
//	4 spat_longitude 浮点型 信号灯的经度 -180~180,(例如:43.1234567)单位:度
	double spat_longitude;
//	5 spat_latitude 浮点型 信号灯的纬度 -90~90,(例如:23.4567891)单位:度
	double spat_latitude;
//	6 distance 整型 车辆与信号灯路口距离 ≥ 0 单位: 0.1米
	int distance;
//	7 allowed_maneuvers bit string 表示车辆所在车道各种允许的通行行为 12 字节 详见 allowed_maneuvers
	char allowed_maneuvers[12];
//	8 lights_number 整型 当前路口信号灯的个数 ≥ 0	对象中的成员顺序对应着	信号灯从左到右排列
	int lights_number;
//	9 lights_list 对象 当前路口信号灯信息 light_info结构
	stclight_info mlight_info[5];
//	10 red_light_violate 整型 闯红灯危险预警 0:无闯红灯危险	1:有 闯红灯危险,当无法获取到车道信息时,使用车辆转向灯信号进行判定
	int red_light_violate;
} stcMsg_103;
//typedef struct Msg_104
//{
//	stclocal_info local_info;
//	stcremoteinfo remote;
//	int alert_type;
//	int alert_level;
//} stcMsg_104;
/**
 * MsgType:105
车辆路边单元预警数据上报显示设备端
 */
typedef struct Msg_105
{
//	序号 成员名称 数据类型 描述 取值 备注
	//1 local_info 对象 本车信息 local_info 结构 参考local_info结构
	stclocal_info local_info;
//	2 time 字符串 终端时间 YYYYMMDDHHMMSS (例 如	20170314000000)
	char time[20];
//	3 RSI_ID 以string存储的十六进制数 路边单元 ID 8字节(16进制数)
	char RSI_ID[20];




	/**4 RSI_type	整型		路边单元预警类型。 Type = 0代表直
			接向用户显示 desecrption字段中的
			字符串。 0:未知类型
			2:限速预警
			3:道路施工
			4:前方桥梁
			5:前方隧道
			6:道路湿滑
			7:速度引导
			8:超速预警
			23:前方急转弯
			24:前方较深积水
			25:前方路面有深坑
			26:前方拥堵提醒
			30:前方落石
			31:连续弯路
			32:禁止超车
			33:限制高度详见alert_type中类别
	是V2I RSI的预警类型*/

	int RSI_type;
/**	5 value	整型	RSI 预警对应值 若RSA的预警类型为前方道
	路拥堵,则1:畅通
	2:基本畅通
	3:轻度拥堵
	4:中度拥堵
	5:严重拥堵
	若为其他预警类型,则
	value代表速度值,单位
	为km/h
*/
	int value;
//	6 distance 整型 距离报警位置的距离	≥ 0	单位 :米
	int distance;
//	7 RSI_latitude 浮点型 交通事件发生位置的纬度	-90~90,( 例如23.4567891)	单位:度
	double RSI_latitude;
//	8 RSI_longitude 浮点型 交通事件发生位置的经度	-180~180,(例如:43.1234567)	单位:度
	double RSI_longitude;
//	9 description 字符串 进一步描述预警信息,当RSI_type =0时,必须填写此字段
	char description[100];
//	10 nr_point 整型 表示 有 多 少 个warning_point	数组	整数:2~32
	int nr_point;
//	11	warning_point_list 用于描述事件相关的道路范围,可描	述非直线范围。由多warning_point组成的数组序列,该序列以多个在真实道路上标 定 的warning_point 点位刻画一段和该事件相关的路径范围多个 warning_point 结构组成的数组 参考warning_point 结构
	stcwarning_point warning_point_list[32];
} stcMsg_105;
/**
 * 	MsgType:106
 *  预警取消数据上报显示设备端
 */
typedef struct Msg_106
{
//	1 local_info 对象 本车信息 local_info 结构 参考local_info结构
	stclocal_info local_info;
//	2 remote_ID 以string存储的十六进制数 周边预警设备ID 8 字节(16进制数)
	char remote_ID[8];
//	3 alert_type 整型 路边单元预警类型	详见 alert_type结构
	int alert_type;
} stcMsg_106;
typedef struct Msg_107
{
//	1 local_info 对象 本车信息 local_info 结构 参考local_info结构
	stclocal_info local_info;
//	2 person_ID 以string存储的十六进制数 行人ID 8 字节(16进制数)
	char person_ID[8];
//	3 alert_distance 整型 与行人之间的距离 ≥ 0 单位:厘米
	int alert_distance;
	/*4	alert_direction	整型	告警行人方向
	0:未知;
	1:正前方;
	2:左前方邻车道;
	3:左前方远车道;
	4:右前方邻车道;
	5:右前方远车道;*/
	int alert_direction;
} stcMsg_107;
/**
 * MsgType:108
 * 路侧盲区预警信息上报至车载智能控制器
 */
typedef struct Msg_108
{
//	1 LidarID 以 string存储的十六进制数 雷达ID 8 字节(16进制数)
	char LidarID[8];
//	2 PTCNum 整型 PTCObjs元素数量
	int PTCNum;
//	3 PTCObjs 对象数组 参看PTCObj信息	PTCObj结构数组
	stcPTCObjs mPTCObjs[30];
} stcMsg_108;

typedef struct Msg_VeH
{
	double lon;
	double lat;
	double theta;
} stcMsg_VeH;
extern stcMsg_VeH g_stcMsg_VeH;
int Factory_Msg101(stcMsg_101 pMsg_101, cJSON *cJMsg_101);
int Factory_Msg102(stcMsg_102 pMsg_102, cJSON *cJMsg_102);
int Factory_Msg103(stcMsg_103 pMsg_103, cJSON *cJMsg_103);
//int Factory_Msg104(stcMsg_104 pMsg_104, cJSON *cJMsg_104);
int Factory_Msg105(stcMsg_105 pMsg_105, cJSON *cJMsg_105);
int Factory_Msg106(stcMsg_106 pMsg_106, cJSON *cJMsg_106);
int Factory_Msg107(stcMsg_107 pMsg_107, cJSON *cJMsg_107);
int Factory_Msg108(stcMsg_108 pMsg_108, cJSON *cJMsg_108);
#endif
