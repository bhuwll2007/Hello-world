/*
 * MercatorProj.h
 *
 *  Created on: Apr 18, 2019
 *      Author: wanji-hxy
 */

#ifndef ALGORITHM_MERCATORPROJ_H_
#define ALGORITHM_MERCATORPROJ_H_

#include "ComputeTool.h"
#include "DataSet/DataSet.h"


//把地球视为球体实现经纬度和墨卡托投影的函数
typedef struct Point
{
	double x;  //经度(正东X)
	double y;  //纬度(正北Y)
}WayPoint;


void SetB0(double b0);
void SetL0(double l0);
int LonLat2Mercator(double B, double L, WayPoint *pPoint_XY);
int Mercator2LonLat(double X, double Y, WayPoint *pPoint_BL);
int XYZ_To_BLH(WayPoint BLH_Origin, WayPoint XYZ_Move, double rotaionangle, WayPoint *BLH_Move);

#endif /* ALGORITHM_MERCATORPROJ_H_ */
