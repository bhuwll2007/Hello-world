/*
 * MercatorProj.c
 *墨卡托投影算法
 *  Created on: Apr 18, 2019
 *      Author: wanji-hxy
 */

#include "MercatorProj.h"

const double __IterativeValue = 10;  //反向转换程序中的迭代初始值
const double __A = 6378137;    //椭球体长半轴,米
const double __B = 6356752.3142;    //椭球体短半轴,米

double __B0;   //标准纬度,弧度（取激光雷达所在纬度）
double __L0;   //原点经度,弧度 (0°)


//设定__B0
void SetB0(double b0)
{
    if(b0<-PI/2||b0>PI/2)
    {
        return;
    }
    __B0=b0;
}
//设定__L0
void SetL0(double l0)
{
    if(l0<-PI||l0>PI)
    {
        return;
    }
    __L0=l0;
}


/*******************************************
投影正向转换程序(B,L)-->(X,Y)
double B: 维度,弧度
double L: 经度,弧度
WayPoint *pPoint_XY: XY坐标，米
*******************************************/
int LonLat2Mercator(double B, double L, WayPoint *pPoint_XY)
{
    double  e/*第一偏心率*/, e_/*第二偏心率*/, NB0/*卯酉圈曲率半径*/, K, dtemp;
//	double E = exp(1);
    if (L < -PI || L > PI || B < -PI / 2 || B > PI / 2)
    {
        return 1;
    }

    if (__A <= 0 || __B <= 0)
    {
        return 1;
    }

    dtemp = 1 - (__B / __A) * (__B / __A);
    if (dtemp < 0)
    {
        return 1;
    }
    e = sqrt(dtemp);

    dtemp = (__A / __B) * (__A / __B) - 1;
    if (dtemp < 0)
    {
        return 1;
    }
    e_ = sqrt(dtemp);

    NB0 = ((__A * __A) / __B) / sqrt(1 + e_ * e_ * cos(__B0) * cos(__B0));

    K = NB0 * cos(__B0);

    pPoint_XY->x = K * (L - __L0);

    pPoint_XY->y = K * log(tan(PI / 4 + B / 2) * pow((1 - e * sin(B)) / (1 + e * sin(B)), e / 2));

    return 0;
}


/*******************************************
投影反向转换程序
double X: 纵向直角坐标
double Y: 横向直角坐标
WayPoint *pPoint_BL:转换经纬度
*******************************************/
int Mercator2LonLat(double X, double Y, WayPoint *pPoint_BL)
{
    double e/*第一偏心率*/, e_/*第二偏心率*/, NB0/*卯酉圈曲率半径*/, K, dtemp;
    double E = exp(1);

    if (__A <= 0 || __B <= 0)
    {
        return 1;
    }

    dtemp = 1 - (__B / __A) * (__B / __A);
    if (dtemp < 0)
    {
        return 1;
    }

    e = sqrt(dtemp);

    dtemp = (__A / __B) * (__A / __B) - 1;
    if (dtemp < 0)
    {
        return 1;
    }
    e_ = sqrt(dtemp);

    NB0 = ((__A * __A) / __B) / sqrt(1 + e_ * e_ * cos(__B0) * cos(__B0));

    K = NB0 * cos(__B0);

    pPoint_BL->x = FG_RadiantoAngle(Y / K + __L0);
    double B = 0;
    int i;
    for (i = 0; i < __IterativeValue; i++)
    {
        B=PI/2-2*atan(pow(E, (-X/K)) * pow(E, (e/2)*log((1-e*sin(B))/(1+e*sin(B)))));

    }
    pPoint_BL->y = FG_RadiantoAngle(B);
    return 0;
}

/*
  XYZ_To_BLH()：特征物体XY坐标转经纬度
  输入：
  WayPoint BLH_Origin: 激光雷达原始经纬度，角度
  WayPoint XYZ_Move: 特征物体XY坐标，米
  double rotaionangle： 激光雷达坐标系Y轴相对于正北方向夹角(-180°～180°)
  输出：
  WayPoint *BLH_Move: 特征物体经纬度，角度
 */
int XYZ_To_BLH(WayPoint BLH_Origin, WayPoint XYZ_Move, double rotaionangle, WayPoint *BLH_Move)
{
    double RadAngle = FG_Angletoradian(rotaionangle);

    SetB0(FG_Angletoradian(BLH_Origin.y));
    SetL0(0);

    //激光器经纬度转墨卡托XY
    WayPoint LiDAR_XYZ;
    LonLat2Mercator(FG_Angletoradian(BLH_Origin.y), FG_Angletoradian(BLH_Origin.x), &LiDAR_XYZ);
//	printf("雷达墨卡托XY：%f, %f\n", LiDAR_XYZ.x, LiDAR_XYZ.y);

    //坐标轴旋转到正北方向,计算Move点墨卡托XY坐标
    WayPoint Move;
    Move.x = XYZ_Move.x * cos(RadAngle) + XYZ_Move.y * sin(RadAngle) + LiDAR_XYZ.x;
    Move.y = XYZ_Move.y * cos(RadAngle) - XYZ_Move.x * sin(RadAngle) + LiDAR_XYZ.y;

    Mercator2LonLat(Move.y, Move.x, BLH_Move);

    return 0;
}


