/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的基本公用函数的API接口.
 *
 * @file ComputeTool.h
 * @brief API file.
 *
 */
#ifndef ALGORITHM_COMPUTETOOL_H_
#define ALGORITHM_COMPUTETOOL_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include <math.h>
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

#define EARTH_RADIUS  6371004
#define PI 3.1415926

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
typedef struct point
{
    double x_;
    double y_;
} Point;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
/**
 * @brief 角度转化弧度值.
 * @param[in] p_dAangel  角度值.
 * @return  弧度值.
 */
double FG_Angletoradian(double p_dAangel);

/**
 * @brief 弧度转化值角度.
 * @param[in] p_dRadian 弧度值.
 * @return  角度值.
 */
double FG_RadiantoAngle(double p_dRadian);

/**
 * @brief 根据经纬度计算两车车距.
 * @param[in] p_dLatObj  自车纬度坐标.
 * @param[in] p_dlngObj  自车经度坐标.
 * @param[in] p_dLatTag  目标车纬度坐标.
 * @param[in] p_dlngTag  目标车经度坐标.
 * @return  车距.
 */
double FG_Getdistance(double p_dLatObj, double p_dlngObj, double p_dLatTag,
                      double p_dlngTag);

/**
 * @brief 根据经纬度计算目标车辆相对于自车的真方位角.
 * @param[in] p_dLatObj 自车纬度坐标.
 * @param[in] p_dlngObj 自车经度坐标.
 * @param[in] p_dLatTag 目标车纬度坐标.
 * @param[in] p_dlngTag 目标车经度坐标.
 * @return  真方位角（°）.
 */
double FG_GetTrueBear(double p_dLatObj, double p_dlngObj, double p_dLatTag,
                      double p_dlngTag);
/**
 * @brief 计算目标现对于车辆的相对方位.
 * @param[in] p_dLatObj 自车纬度坐标.
 * @param[in] p_dlngObj 自车经度坐标.
 * @param[in] p_dLatTag 目标车纬度坐标.
 * @param[in] p_dlngTag 目标车经度坐标.
 * @return  相对方位角（°）.
 */
double FG_GetRelBear(double p_dLatObj, double p_dlngObj, double p_dLatTag,
                     double p_dlngTag, double p_dCObj);

/**
 * @brief 根据经纬度计算目标车辆相对于自车的坐标值.
 * @param[in] host_longitude 自车经度坐标.
 * @param[in] host_latitude 自车纬度坐标.
 * @param[in] host_heading 自车车头朝向.
 * @param[in] other_longitude 目标车经度坐标.
 * @param[in] other_latitude 目标车纬度坐标.
 * @return  自车坐标结构体.
 */
Point gpsToRelativeLoc(double host_longitude, double host_latitude,
                       double host_heading, double other_longitude, double other_latitude);

/**
 * @brief 根据两点坐标计算距离.
 * @param[in] x1 点1横坐标.
 * @param[in] y1 点1纵坐标.
 * @param[in] x2 点2横坐标.
 * @param[in] y2 点2纵坐标.
 * @return  两点距离.
 */
double disOfPoints(double x1, double y1, double x2, double y2);

/**
 * @brief 计算三角形面积.
 * @param[in] A  三角形顶点A.
 * @param[in] B  三角形顶点B.
 * @param[in] C  三角形顶点C.
 * @return  三角形面积.
 */
double FG_GetTriangleArea(Point A, Point B, Point C);

/**
 * @brief 判断点D是否在三角形ABC内.
 * @param[in] A  三角形顶点A.
 * @param[in] B  三角形顶点B.
 * @param[in] C  三角形顶点C.
 * @param[in] D  目标点D.
 * @return  0--匹配成功， 1--匹配失败.
 */
int IsInTriangle(Point A, Point B, Point C, Point D);

/**
 * @brief 可调用地图匹配函数接口.
 * @param[in] pMapInfo  地图信息结构体指针.
 * @param[in] pVehInfo  车辆信息结构体指针.
 * @return  0--D不在三角形内， 1--D在三角形内.
 */
int IsAcutetriangle(double p_dLat1, double p_dLng1, double p_dLat2,
                    double p_dLng2, double p_dLat3, double p_dLng3);

/**
 * @brief 返回车辆位置点3到路段12的距离.
 * @param[in] p_dLat1  路段端点1纬度.
 * @param[in] p_dLng1  路段端点1经度.
 * @param[in] p_dLat2  路段端点2纬度.
 * @param[in] p_dLng2  路段端点2经度.
 * @param[in] p_dLat3  车辆位置点3纬度.
 * @param[in] p_dLng3  车辆位置点3经度.
 * @return 车辆到路段距离.
 */
double GetVerticalDis(double p_dLat1, double p_dLng1, double p_dLat2,
                      double p_dLng2, double p_dLat3, double p_dLng3);

double WheelAngle_Convert(double wheelAngle);
void PathPredict(double Time, double Interval, double heading, double speed, double wheelAngle,
		double wheelDis, Point sPoint, Point *pPathPoints);
#endif /* ALGORITHM_COMPUTETOOL_H_ */

/**
 * @}
 */
