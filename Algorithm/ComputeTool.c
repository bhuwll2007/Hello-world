/*
 * ComputeTool.c
 *
 *  Created on: May 28, 2018
 *      Author: root
 */
#include "ComputeTool.h"

//卡尔曼滤波参数
 double Coffi_Q = 0.1;
 double Coffi_R = 5;

/**********************************************
 * 函数名  : double FG_Angletoradian(double p_dA)
 * 功能描述: 角度转化弧度值
 * 输入参数: p_dAangel  角度值
 * 输出参数: *
 * 返回值  :  弧度值
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 10:57:36
 **********************************************/
double FG_Angletoradian(double p_dAangel)
{
    return p_dAangel * PI / 180.0;
}

/**********************************************
 * 函数名 :  double FG_RadiantoAngle(double p_dR)
 * 功能描述: 弧度转化值角度
 * 输入参数: p_dRadian 弧度值
 * 输出参数: *
 * 返回值 :   角度值
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 10:58:55
 **********************************************/
double FG_RadiantoAngle(double p_dRadian)
{
    return p_dRadian * 180.0 / PI;
}

/**********************************************
 * 函数名 :  double FG_Getdistance(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
 * 功能描述: 根据经纬度计算两车车距
 * 输入参数: p_dLatObj 自车纬度坐标
		     p_dlngObj 自车经度坐标
		     p_dLatTag 目标车纬度坐标
		     p_dlngTag 目标车经度坐标
 * 输出参数: *
 * 返回值 : p_dDistance 车距
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 11:00:10
 **********************************************/
double FG_Getdistance(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
{
	double p_dDistance = 0;

    double radLat1 = FG_Angletoradian(p_dLatObj);
    double radLat2 = FG_Angletoradian(p_dLatTag);

    double a = radLat1 - radLat2;
    double b = FG_Angletoradian(p_dlngObj) - FG_Angletoradian(p_dlngTag);
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));
    dst = dst * EARTH_RADIUS/1000;

	p_dDistance = (dst * 10000000) / 10000;
    return p_dDistance;
}

/**********************************************
 * 函数名 :  double FG_GetTrueBear(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
 * 功能描述: 根据经纬度计算目标车辆相对于自车的真方位角
 * 输入参数: p_dLatObj 自车纬度坐标
		     p_dlngObj 自车经度坐标
		     p_dLatTag 目标车纬度坐标
		     p_dlngTag 目标车经度坐标
 * 输出参数: *
 * 返回值 :  p_dTrueBear  真方位角（°）
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 11:06:45
 * 修改记录： 170331  修改了asinA>1的情况
 **********************************************/
double FG_GetTrueBear(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
{
	double p_dTrueBear = 0, Azi = 0, azi=0;
	double cosc = 0, cosc2 = 0;
	double sinc = 0, asinA = 0;

	//角度转弧度
	double radLat1 = FG_Angletoradian(p_dLatObj);
	double radLng1 = FG_Angletoradian(p_dlngObj);
	double radLat2 = FG_Angletoradian(p_dLatTag);
    double radLng2 = FG_Angletoradian(p_dlngTag);

	//算法求解
	double DeltaLat = p_dLatTag - p_dLatObj;
	double DeltaLng = p_dlngTag - p_dlngObj;

	cosc = cos(PI/2 - radLat2) * cos(PI/2 - radLat1) + sin(PI/2 - radLat2) * sin(PI/2 - radLat1) * cos(radLng2 - radLng1);
	cosc2 = pow(cosc, 2);
	sinc = pow(1 - cosc2, 0.5);
	asinA = sin(PI/2 - radLat2) * sin(radLng2 - radLng1) / sinc;
	if(asinA > 1)
	{
		asinA = 1;
	}
	azi = asin(asinA);
	//位置判断
	if(DeltaLng ==0)          //两车在同一经度
	{
		if(DeltaLat >0)
			Azi = 0;
		if(DeltaLat <0)
			Azi = PI;
	}
	else if(DeltaLat ==0)	 //两车在同一纬度
	{
		if(DeltaLng >0)
			Azi = PI/2;
		if(DeltaLng <0)
			Azi = PI * 3/2;
	}
	else
	{
		if(DeltaLat <0)			//目标车B在自车A南方
			Azi = PI - azi;
		if(DeltaLat >0)
		{
			if(DeltaLng >0)     //B在A东北方
				Azi = azi;
			if(DeltaLng <0)     //B在A西北方
				Azi = 2 * PI + azi;
		}
	}

	p_dTrueBear = FG_RadiantoAngle(Azi);
	return p_dTrueBear;
}

/*******************************************************
 * 函数名 :  double FG_GetRelBear(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag, double p_dCObj)
 * 功能描述: 计算目标现对于车辆的相对方位
 * 输入参数:
 * 输出参数:
 * 返回值 : double
 * 作者 : 黄翔宇
 * 创建日期: 2017/6/7 14:19:28
 * 修改记录 :
 ********************************************************/
double FG_GetRelBear(double p_dLatObj, double p_dlngObj, double p_dLatTag,
		double p_dlngTag, double p_dCObj) {

	double p_dTrueBear = FG_GetTrueBear(p_dLatObj, p_dlngObj, p_dLatTag,
			p_dlngTag);

	//计算相对方位
	double p_RelBear = p_dTrueBear - p_dCObj;
	if (p_RelBear < 0)
		p_RelBear += 360;

	return p_RelBear;
}

/**********************************************
 * 函数名 :  gpsToRelativeLoc()
 * 功能描述: 根据经纬度计算目标车辆相对于自车的坐标值
 **********************************************/
Point gpsToRelativeLoc(double host_longitude, double host_latitude, double host_heading,
                       double other_longitude, double other_latitude)
{
	Point RelPoint;
    double LatiRatio = 111712.69150641056;   //6371004*PI/180
    double LonRatio = LatiRatio*cos(host_latitude/180.0*PI);

    double gps_x = (other_longitude  - host_longitude) * LonRatio;
    double gps_y = (other_latitude - host_latitude) * LatiRatio;

    double angle = - host_heading * PI/180.0;

    /// Rotate center is (0, 0)
    RelPoint.x_ = (gps_x - 0) * cos(angle) +
            (gps_y - 0) * sin(angle) + 0;
    RelPoint.y_ = -(gps_x - 0) * sin(angle) +
            (gps_y - 0) * cos(angle) + 0;

    return RelPoint;
}

/**********************************************
 * 函数名 :  disOfPoints()
 * 功能描述: 根据两点坐标计算距离
 **********************************************/
double disOfPoints(double x1, double y1, double x2, double y2)
{
    return (double)sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

/**********************************************
 * 函数名 :  FG_GetTriangleArea()
 * 功能描述: 计算三角形面积
 **********************************************/
double FG_GetTriangleArea(Point A, Point B, Point C){
	double p_dDis12 = disOfPoints(A.x_, A.y_, B.x_, B.y_);
	double p_dDis23 = disOfPoints(B.x_, B.y_, C.x_, C.y_);
	double p_dDis13 = disOfPoints(A.x_, A.y_, C.x_, C.y_);

	double p = (p_dDis12 + p_dDis13 + p_dDis23) / 2;
	return pow(p * (p - p_dDis12) * (p - p_dDis13) * (p - p_dDis23), 0.5);
}

/**********************************************
 * 函数名 :  IsInTriangle()
 * 功能描述: 判断点D是否在三角形ABC内
 **********************************************/
int IsInTriangle(Point A, Point B, Point C, Point D){
	double S_abc = FG_GetTriangleArea(A, B, C);
	double S_adb = FG_GetTriangleArea(A, D, B);
	double S_bdc = FG_GetTriangleArea(B, D, C);
	double S_adc = FG_GetTriangleArea(A, D, C);

	double Sum_Aera = S_adb + S_bdc + S_adc;
	double Div  = S_abc - Sum_Aera;
	if(-0.001 < Div && Div < 0.001)
		return 0;
	else
		return 1;
}

/**********************************************
 * 函数名 :  IsAcutetriangle()
 * 功能描述: 判断车辆位置点3与路段12构成的三角形是否是锐角三角形
 * 判断车辆位置点3是否在道路12所构成的切面内
 * ---------------------------------------------
 * =====================1=======================
 * =====================|=======================
 * =====================|=======================
 * =============================================
 * =============================================
 * =============================================
 * =============================================
 * =============================================
 * =====================2=======================
 * ---------------------------------------------
 *
 *
 **********************************************/
int IsAcutetriangle(double p_dLat1, double p_dLng1, double p_dLat2,
		double p_dLng2, double p_dLat3, double p_dLng3) {
	double p_dDis12 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat2, p_dLng2);
	double p_dDis23 = FG_Getdistance(p_dLat2, p_dLng2, p_dLat3, p_dLng3);
	double p_dDis13 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat3, p_dLng3);

	//printf("p_dDis12 = %f,p_dDis23= %f, p_dDis13 = %f\n",p_dDis12,p_dDis23,p_dDis13);
	double temp1 = p_dDis12 * p_dDis12 + p_dDis13 * p_dDis13;
	double temp2 = p_dDis12 * p_dDis12 + p_dDis23 * p_dDis23;

	if ((temp1 < (p_dDis23 * p_dDis23)) || (temp2 < (p_dDis13 * p_dDis13))) {
		return 1;
	} else {
		return 0;  //锐角三角形
	}
}

/**********************************************
 * 函数名 :  IsAcutetriangle()
 * 功能描述: 返回车辆位置点3到路段12的距离
 **********************************************/
double GetVerticalDis(double p_dLat1, double p_dLng1, double p_dLat2,
		double p_dLng2, double p_dLat3, double p_dLng3) {
	double p_dDis12 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat2, p_dLng2);
	double p_dDis23 = FG_Getdistance(p_dLat2, p_dLng2, p_dLat3, p_dLng3);
	double p_dDis13 = FG_Getdistance(p_dLat1, p_dLng1, p_dLat3, p_dLng3);

	double p = (p_dDis12 + p_dDis13 + p_dDis23) / 2;
	double S = pow(p * (p - p_dDis12) * (p - p_dDis13) * (p - p_dDis23), 0.5);

	double p_dDis = 2 * S / p_dDis12;

	return p_dDis;
}


/**********************************************
 * 函数名 :  double Kalman_Filter(double x_measure)
 * 功能描述: 卡尔曼滤波函数
 * 输入参数: x_measure 测量原始值
 * 输出参数: *
 * 返回值 :   滤波值
 * 作者 : 黄翔宇
 * 创建日期: 2017/3/20 15:11:55
 **********************************************/
int nFlag1 = 0;
double x_last1 = 0;
double p_last1 = 0;
double Kalman_Filter(double x_measure)
{
	double x_now = 0;
	double p_now = 0;

	if (nFlag1 == 0) {
		x_last1 = x_measure;
		p_last1 = 1;
		nFlag1 = 1;
	}

	double x_mid = x_last1;
	double p_mid = p_last1 + Coffi_Q;
	double kg = p_mid / (p_mid + Coffi_R);

	x_now = x_mid + kg * (x_measure - x_mid);		//估计出的最优值
	p_now = (1 - kg) * p_mid;						//最优值对应的covariance
	p_last1 = p_now;								//更新covariance值
	x_last1 = x_now;								//更新系统状态值

	return x_now;
}


Point NextPoint(Point sPoint, double length, double angle)
{
    angle = angle * PI /180.0;
    double xDis = length * sin(angle);
    double yDis = length * cos(angle);

    Point ePoint = {sPoint.x_ + xDis, sPoint.y_ + yDis};
    return ePoint;
}


double WheelAngle_Convert(double wheelAngle)
{
	int Flag;
	if (wheelAngle >= 0)
		Flag = 1;
	else
		Flag = 4;

	switch(Flag)
	{
	case 1:  //左转、左轮
		return 0.0543 * wheelAngle - 0.116;
	case 2:  //左转、右轮
		return 0.0613 * wheelAngle - 0.658;
	case 3:  //右转、左轮
		return 0.0615 * wheelAngle + 0.7;
//		return -0.0615 * wheelAngle - 0.7;
	case 4:  //右转、右轮
		return 0.0543 * wheelAngle + 0.128;
//		return -0.0543 * wheelAngle - 0.128;
	default:
		return 0;

	}
}


void PathPredict(double Time, double Interval, double heading, double speed, double wheelAngle,
		double wheelDis, Point sPoint, Point *pPathPoints)
{
	int i;
    Point startPoint = sPoint;
    Point nextPoint;

	double arrowLength = speed * Interval;
	double arrowNum = round(Time/Interval);

    double r =  wheelDis/(sin(wheelAngle*PI/180)+0.000001);
    double lengthAngle = arrowLength/r;
    double length = fabs(r * sin(lengthAngle/2)*2);
    double rotateAngle = lengthAngle/2/PI*180;

    double curHeading;
    for (i=1; i<=arrowNum; i++)
    {
        if(fabs(rotateAngle * i) > 180)
            break;
        curHeading = heading - rotateAngle * i;
        nextPoint = NextPoint(startPoint, length, curHeading);

        pPathPoints[i-1].x_ = nextPoint.x_;
        pPathPoints[i-1].y_ = nextPoint.y_;
        startPoint = nextPoint;
    }
}


