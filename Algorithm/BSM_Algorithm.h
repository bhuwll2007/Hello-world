/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的主动安全相关的API接口.
 *
 * @file BSM_Algorithm.h
 * @brief API file.
 *
 */
#ifndef ALGORITHM_BSM_ALGORITHM_H_
#define ALGORITHM_BSM_ALGORITHM_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "Appconfigure.h"
#include "ComputeTool.h"
#include "DataSet/DataSet.h"
#include "Type.h"
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/**
 * @brief 矩形车辆模型顶点坐标.
 */
typedef struct _STCrectangle
{
	double UL[1][2];
	double UR[1][2];
	double LL[1][2];
	double LR[1][2];
}__attribute__ ((packed)) rectangle;

/**
 * @brief 投影轴方向向量.
 */
typedef struct _STCAxis
{
	double x;
	double y;
}__attribute__ ((packed)) Axis;

struct PosLane
{
	int ForwordNodeId;
	int BehideNodeId;
};
struct GnssPoint
{
	double Lat;
	double Lng;
	double Heading;
	double Speed;
};

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
/**
 * @brief 主动安全应用算法接口.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[in] Mode 0地图模式/1非地图模式.
 * @param[out] pResult  识别结果结构体.
 */
tResult_Bsm BSM_VehicleActiveSafety(tVehData HV, tVehData RV, uint8_t Mode) ;

/**
 * @brief 同车道前向碰撞预警识别.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_FCW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);

/**
 * @brief 同车道后方碰撞预警识别.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_BCW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);
/**
 * @brief 不同车道交汇碰撞预警.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
int V2V_ICW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);

/**
 * @brief 逆向超车预警.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_DNPW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);

/**
 * @brief 变道预警.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[in] RV_Coord  两车驶向的同一节点.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_LCW_Section(tVehData HV, tVehData RV, Point RV_Coord,
		tResult_Bsm *pResult);
void V2V_LCW_Section_Rectangle(tVehData HV, tVehData RV, Point RV_Coord,
                     tResult_Bsm *pResult);

/**
 * @brief 异常车辆提醒.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_AVW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);

/**
 * @brief 紧急车辆提醒.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_EVW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult);

/**
 * @brief 失控车辆提醒.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[out] pResult  识别结果结构体.
 */
void V2V_CLW_Warning(tVehData HV, tVehData RV, tResult_Bsm *pResult);


/**
 * @brief 一点在投影轴上的标量.
 * @param[in] axis	投影轴.
 * @param[in] point	点坐标.
 * @return  标量值.
 */
double PointScalar(Axis axis, double point[1][2]);

/**
 * @brief 计算矩形四个顶点在投影轴上的标量，输出最大值和最小值.
 * @param[in] axis   投影轴向量结构体.
 * @param[in] Rec    矩形顶点坐标结构体.
 * @param[out] max	 标量最大值.
 * @param[out] min	 标量最小值.
 * @return  0--成功，非0--失败.
 */
void ScalarCompare(Axis axis, rectangle Rec, double *max, double *min);

/**
 * @brief 判断交汇最近点时两车是否碰撞.
 * @param[in] p_dDistance	   当前车距.
 * @param[in] DCPA             最近点时车距.
 * @param[out] p_dRelBearing   相对方位角.
 * @param[out] p_dRelCourse	   相对方向角.
 * @return  0--最近点时两车没有碰撞， 1--最近点时两车会碰撞.
 */
int IntersctionWarn(double p_dDistance, double DCPA, double p_dRelBearing,
		double p_dRelCourse, tVehData HV, tVehData RV);

/**
 * @brief 获取交汇两车交汇最近点坐标.
 * @param[in] D  车距.
 * @param[in] R  最近点距离DCPA.
 * @param[in] Angle  相对方位.
 * @param[in] Course 相对行驶方向.
 * @param[out] X  坐标x值.
 * @param[out] Y  坐标y值.
 * @return  0--成功，非0--失败.
 */
void GetCPA(double D, double R, double Angle, double Course, double *X,
		double *Y);

/**
 * @brief 根据HV当前车速时间和车速，判断RV是否在危险范围内.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @return  0--进行危险识别， 1--安全 不做处理.
 */
int IsDangerPossible(tVehData HV, tVehData RV);

/**
 * @brief 基于RV相对于HV的位置进行危险类型判断: 前方---异常车辆、
 * 								          正前方---前向碰撞、
 * 								          后方---紧急车辆、
 * 								          侧后方---盲区提示
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[in] RV_Coord  两车驶向的同一节点.
 * @return  远车所在位置代码.
 */
int CaseDetect(tVehData HV, tVehData RV, Point RV_Coord);

/**
 * @brief 判断RV相对于HV的位置：正前方、左前方、右前方、正后方、左边后方、右后方.
 * @param[in] HV  自车信息结构体.
 * @param[in] RV  远车信息结构体.
 * @param[in] RV_Coord  两车驶向的同一节点.
 * @return  远车所在位置代码.
 */
int CalVehicleLocation(tVehData HV, tVehData RV, Point RV_Coord);

/**
 * @brief 前方车辆时间车距计算.
 * @param[in] p_dVObj     自车车速.
 * @param[in] p_dDistance 自车与前车车距.
 * @param[in] HV_Length   自车长度.
 * @param[in] RV_Length   远车长度.
 * @return  车距时间.
 */
double FG_GetTHW(double p_dVObj, double p_dDistance, double HV_Length,
		double RV_Length);

/**
 * @brief 与前方车辆碰撞时间计算.
 * @param[in] p_dVObj      自车车速.
 * @param[in] p_dVtag      目标车车速.
 * @param[in] p_dDistance  自车与前车车距.
 * @param[in] HV_Length    自车长度.
 * @param[in] RV_Length    远车长度.
 * @return  碰撞时间.
 */
double FG_GetTTC(double p_dVobj, double p_dVtag, double p_dDistance,
		double HV_Length, double RV_Length);

/**
 * @brief 计算两车到达最近点的时间和距离.
 * @param[in] p_dLatObj  自车纬度坐标.
 * @param[in] p_dlngObj  自车经度坐标.
 * @param[in] p_dLatTag  目标车纬度坐标.
 * @param[in] p_dlngTag  目标车经度坐标.
 * @param[in] p_dVObj	 自车车速.
 * @param[in] p_dVTag	 目标车车速.
 * @param[in] p_dCObj    自车航向角.
 * @param[in] p_dCTag	 目标车航向角.
 * @param[out] CPA[]     CPA[0] 两车到达最近点的距离. CPA[1] 两车到达最近点的时间. CPA[2] 相对行驶方向.
 * @return  0.
 */
int FG_GetClosePoint(double p_dLatObj, double p_dlngObj, double p_dVObj,
		double p_dCObj, double p_dLatTag, double p_dlngTag, double p_dVTag,
		double p_dCTag, double CPA[]);

/**
 * @brief 获取警告级别.
 * @param[in] TTC.
 * @param[in] Type.
 * @return  警告级别结构体.
 */
WARN_LVL FG_GetWarnLvl(double TTC, char Type);

/**
 * @brief 获取两车之间的相对方位角
 * @param[in] HV. 自车信息
 * @param[in] RV. 对方车信息
 * @return  对方车处于自车的相对方位角.
 */
double FG_GetRelCourse(tVehData HV, tVehData RV);

#endif /* ALGORITHM_BSM_ALGORITHM_H_ */

/**
 * @}
 */
