#ifndef __ALGORITHM_DATASET_H_
#define __ALGORITHM_DATASET_H_
#include "../Include/BaseInterface_Include.h"
#include "../Include/FaciLayer_Include.h"
#include "../Include/MsgFrame_Include.h"
//主动安全配置参数
typedef struct _V2V_Parameter
{
	//地图匹配参数
	double MapMatch_ConfidenceDis_toLink;     //路段阈值  m
	//危险筛选
	double isDanger_hv_warning_time;			//危险警告时间 s
	double isDanger_hv_speed_low;				//危险识别最小车速 m/s
	double VehLOC_SafeDis_Value;				//横向安全车距
	double ICW_TTC_SafeDisVeh;					//交叉安全车距
	//场景算法参数
	double AVW_Speed_threshold_value;			//异常车速差
	double FCW_TTC_threshold_value;				//前向碰撞时间
	double FCW_THW_threshold_value;				//前向车速时间
	double LCW_Blind_Spot_Angle;				//盲区角度
	double LCW_Blind_Spot_Width;				//盲区宽度
	double LCW_Blind_Spot_ToMirror;				//后视镜距离
	double ICW_TTC_threshold_value;				//交叉碰撞时间
} tV2V_Parameter;

typedef struct _Spat_Parameter
{
	double LightDir_Dis_Value;  				//红绿灯的下游节点的横向偏移值大小
	double DistoCrossRdMid;						//红绿灯路口到车的距离
	double SPATWarnDis;							//红绿灯速度引导预警距离

	/// Parameter for SpeedGuide()
	double WarnCalcTime;						//预警时间阈值，该字段用于计算播报距离之前的时间差
	double SpeedLimit_High;						//建议时速的最大值
	double SpeedLimit_Low;						//建议时速的最小值
} tSpat_Parameter;

typedef struct _V2I_Parameter
{
	/// Parameter for IVS()
	double IVSWarnDis;							//RSI事件预警计算的距离值
	double IVSSpeakDis;							//RSI事件播报的距离值
} tV2I_Parameter;

typedef struct _RSM_Parameter
{
	double Laser_Lat;							//激光坐标维度
	double Laser_Long;							//激光坐标经度
	double Laser_Display;						//激光数据显示距离值
} tRSM_Parameter;

// -------------------- split line ------------------------
extern tV2V_Parameter g_V2V_Parameter;
extern tSpat_Parameter g_Spat_Parameter;
extern tV2I_Parameter g_V2I_Parameter;
extern tRSM_Parameter g_RSM_Parameter;


#endif /*__ALGORITHM_DATASET_H_*/
