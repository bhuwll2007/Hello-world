/*
 * BSM_Algorithm.c
 *
 *  Created on: May 28, 2018
 *      Author: root
 */
#include "BSM_Algorithm.h"
#define PRINFT_OPT 0

tResult_Bsm BSM_VehicleActiveSafety(tVehData HV, tVehData RV, uint8_t Mode)
{
    tResult_Bsm CResult = { Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT,
                            LVL_DEFAULT, -1, -1, -1
                          };
#if PRINFT_OPT
    printf("\n");
    printf("//***********************BSM_VehicleActiveSafety********************************//\n");
#endif

#if 0
    //自车航向角卡尔曼滤波
    if (fabs(HV.pointHistory.Heading - HV.Heading) < 2)
    {
        HV.Heading = Kalman_Filter(HV.Heading);
    }
#endif

    //设定安全范围
    if (IsDangerPossible(HV, RV) != 0)
    {
        goto SAFE;
    }

    //计算RV相对于HV为圆心的坐标值
    Point RV_Coord = gpsToRelativeLoc(HV.Longitude, HV.Latitude, HV.Heading,
                                      RV.Longitude, RV.Latitude);

    CResult.wType = Type_BSM;
    CResult.Location = CalVehicleLocation(HV, RV, RV_Coord);

#if PRINFT_OPT
    printf("RV Location : x = %f, y = %f\n", RV_Coord.x_, RV_Coord.y_);
#endif


    //采用无地图模式
    if (Mode == 1)
    {
        double dRelCourse = FG_GetRelCourse(HV, RV);

        //前方同路段
        V2V_EVW_Section(HV, RV, &CResult);	//紧急车辆
        V2V_CLW_Warning(HV, RV, &CResult);	//失控车辆

        //后向碰撞
        if (CResult.Location == BACK_LOC)
            V2V_BCW_Section(HV, RV, &CResult);


        if (dRelCourse < 30)        {
            //同方向
            switch (CaseDetect(HV, RV, RV_Coord))
            {
            case FRONT_LOC:
                V2V_AVW_Section(HV, RV, &CResult);
                break;
            case FRONT_LOC_DIRECT:
                V2V_FCW_Section(HV, RV, &CResult);
                break;
            case BACK_LOC:
                break;
            case BACK_LOC_SIDE:
            	//配置参数LCW_Blind_Spot_Angle大于20是三角形盲区，单位°
            	//否则是矩形盲区单位m
            	if (g_V2V_Parameter.LCW_Blind_Spot_Angle > 20)
            		V2V_LCW_Section(HV, RV, RV_Coord, &CResult);
            	else
            		V2V_LCW_Section_Rectangle(HV, RV, RV_Coord, &CResult);
                break;
            default:
                break;
            }
        }
        else if (dRelCourse > 170 && dRelCourse < 190)
        {
            //逆向
            V2V_DNPW_Section(HV, RV, &CResult);
        }
        else
        {
            //交叉
            V2V_ICW_Section(HV, RV, &CResult);
        }
    }

    //采用有地图模式
    if (Mode == 0)
    {
        V2V_EVW_Section(HV, RV, &CResult);
        V2V_CLW_Warning(HV, RV, &CResult);

        if (CResult.Location == BACK_LOC)
            V2V_BCW_Section(HV, RV, &CResult);


        //根据HV、RV路段匹配信息，进行潜在危险类型识别
        if (HV.ForwardNode == RV.ForwardNode && HV.BackNode == RV.BackNode)
        {
            //相同路段驶向同一节点

            switch (CaseDetect(HV, RV, RV_Coord))
            {
            case FRONT_LOC:
                V2V_AVW_Section(HV, RV, &CResult);
                break;
            case FRONT_LOC_DIRECT:
                V2V_FCW_Section(HV, RV, &CResult);
                break;
            case BACK_LOC:

                break;
            case BACK_LOC_SIDE:
            	//配置参数LCW_Blind_Spot_Angle大于20是三角形盲区，单位°
            	//否则是矩形盲区单位m
            	if (g_V2V_Parameter.LCW_Blind_Spot_Angle > 20)
            		V2V_LCW_Section(HV, RV, RV_Coord, &CResult);
            	else
            		V2V_LCW_Section_Rectangle(HV, RV, RV_Coord, &CResult);
                break;
            default:
                break;

            }
        }
        else if (HV.ForwardNode == RV.ForwardNode
                 && HV.BackNode != RV.BackNode)
        {
            //不同路段驶向同一节点
            V2V_ICW_Section(HV, RV, &CResult);

        }
        else if (HV.ForwardNode == RV.BackNode
                 && HV.BackNode == RV.ForwardNode)
        {
            //相同路段逆向行驶
            if (CResult.Location == FRONT_LOC_Left && RV_Coord.x_ > -5)
                V2V_DNPW_Section(HV, RV, &CResult);
        }
        else
        {
            //无风险
            goto SAFE;
        }
    }

    CResult.RelBearing = RV_Coord.x_;

SAFE:
    return CResult;
}

/*******************************************************
 * 函数名 :  V2V_BCW_Section()
 * 功能描述: 同车道后方碰撞预警识别
 ********************************************************/

void V2V_BCW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In BCW_Section********************************//\n");
#endif

    //计算车距， RV相对于自车的相对方位
    double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                RV.Longitude);
    double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                                      RV.Longitude, HV.Heading);

    //计算碰撞时间，车距时间
    double TTC = FG_GetTTC(RV.Speed, HV.Speed, Dis, HV.VehicleLength,
                           RV.VehicleLength);

    if (TTC < g_V2V_Parameter.FCW_TTC_threshold_value)
    {
        //前向碰撞
        pResult->wStr = FCW_Warning;
        pResult->wPrio = HIGH_PRIO;
        pResult->wLvl = FG_GetWarnLvl(TTC, V2V_FCW_TTC);

    }

    pResult->TTC = TTC;
    pResult->Distance = Dis;
    pResult->RelBearing = RelBearing;

}

/*******************************************************
 * 函数名 :  V2V_FCW_Section()
 * 功能描述: 同车道前向碰撞预警识别
 ********************************************************/

void V2V_FCW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In FCW_Section********************************//\n");
#endif

    //计算车距， RV相对于自车的相对方位
    double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                RV.Longitude);
    double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                                      RV.Longitude, HV.Heading);

    //计算碰撞时间，车距时间
    double TTC = FG_GetTTC(HV.Speed, RV.Speed, Dis, HV.VehicleLength,
                           RV.VehicleLength);
    double THW = FG_GetTHW(HV.Speed, Dis, HV.VehicleLength, RV.VehicleLength);
    if(THW == 0){}
    if (TTC < g_V2V_Parameter.FCW_TTC_threshold_value)
    {
        //前向碰撞
        pResult->wStr = FCW_Warning;
        pResult->wPrio = HIGH_PRIO;
        pResult->wLvl = FG_GetWarnLvl(TTC, V2V_FCW_TTC);

    }
    else if (RV.BreakPadel == 2)
    {
        //紧急刹车
        pResult->wStr = EBW_Warning;
        pResult->wPrio = MEDIUM_PRIO;
        pResult->wLvl = FG_GetWarnLvl(0, V2V_MAY_DANGER);
    }
    else
    {
        if (THW < g_V2V_Parameter.FCW_THW_threshold_value)
        {
            //跟车过近
            pResult->wStr = CVF_Warning;
            pResult->wPrio = MEDIUM_PRIO;
            pResult->wLvl = FG_GetWarnLvl(THW, V2V_FCW_THW);
        }
        else
        {

        }
    }

    if (pResult->wStr >= 0)
    {
        pResult->TTC = TTC;
        pResult->Distance = Dis - (HV.VehicleLength + RV.VehicleLength)/2;
        pResult->RelBearing = RelBearing;
    }
    else
    {

        //如果前方车道没有危险，判断前方路段是否有异常车辆
//        V2V_AVW_Section(HV, RV, pResult);

    }

}

int V2V_ICW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In ICW_Section********************************//\n");
#endif
    double CPA[3];
    //不同车道交汇碰撞预警
    double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                RV.Longitude);
    double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                                      RV.Longitude, HV.Heading);

    FG_GetClosePoint(HV.Latitude, HV.Longitude, HV.Speed, HV.Heading,
                     RV.Latitude, RV.Longitude, RV.Speed, RV.Heading, CPA);
    double DCPA = CPA[0];
    double TTC = CPA[1];
    double CrossAngle = CPA[2];

    int WarnFlag = IntersctionWarn(Dis, DCPA, RelBearing, CrossAngle, HV, RV);

    //左转辅助
    if ((HV.Light & leftTurnSignalOn) == leftTurnSignalOn)
    {
        if (Dis < 80 + 10 * RV.Speed)
        {

            pResult->wStr = LTA_Warning;
            pResult->wPrio = MEDIUM_PRIO;
            pResult->wLvl = FG_GetWarnLvl(TTC, V2V_ICW_TTC);
            if (WarnFlag == 1
                    && TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
            {
                pResult->wPrio = HIGH_PRIO;
            }
        }

        pResult->TTC = TTC;
        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;

        return 0;
    }

    //交叉碰撞
    if (WarnFlag == 1 && TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
    {
        pResult->wStr = ICW_Warning;

        pResult->wPrio = HIGH_PRIO;
        pResult->wLvl = FG_GetWarnLvl(TTC, V2V_ICW_TTC);

        pResult->TTC = TTC;
        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;

    }
    return 0;
}

void V2V_DNPW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In DNPW_Section********************************//\n");
#endif
    if ((HV.Light & leftTurnSignalOn) == leftTurnSignalOn)
    {
        double CPA[3];
        //不同车道交汇碰撞预警
        double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                    RV.Longitude);
        double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude,
                                          RV.Latitude, RV.Longitude, HV.Heading);

        FG_GetClosePoint(HV.Latitude, HV.Longitude, HV.Speed, HV.Heading,
                         RV.Latitude, RV.Longitude, RV.Speed, RV.Heading, CPA);
        double DCPA = CPA[0];
        double TTC = CPA[1];
        double CrossAngle = CPA[2];

        int WarnFlag = IntersctionWarn(Dis, DCPA, RelBearing, CrossAngle, HV,
                                       RV);

//通过距离时间判断
        if (Dis < 80 + 10 * RV.Speed)
        {
            pResult->wStr = DNPW_Waring;

            pResult->wPrio = MEDIUM_PRIO;
            pResult->wLvl = FG_GetWarnLvl(TTC, V2V_ICW_TTC);

            pResult->TTC = TTC;
            pResult->Distance = Dis;
            pResult->RelBearing = RelBearing;

//有碰撞风险
            if (WarnFlag == 1
                    && TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
            {
                pResult->wPrio = HIGH_PRIO;
            }
        }
    }
}

void V2V_LCW_Section(tVehData HV, tVehData RV, Point RV_Coord,
                     tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In LCW_Section********************************//\n");
#endif

    Point HV_BlindSpot[3], RV_EndPoint[4];
    int i, result;
//	tResult_Bsm CResult = { -1, Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT, LVL_DEFAULT, -1, -1, -1 };

//邻车顶点坐标
    RV_EndPoint[0].x_ = RV_Coord.x_ + RV.VehicleWidth / 2;
    RV_EndPoint[0].y_ = RV_Coord.y_ + RV.VehicleLength / 2;

    RV_EndPoint[1].x_ = RV_Coord.x_ - RV.VehicleWidth / 2;
    RV_EndPoint[1].y_ = RV_Coord.y_ + RV.VehicleLength / 2;

    RV_EndPoint[2].x_ = RV_Coord.x_ - RV.VehicleWidth / 2;
    RV_EndPoint[2].y_ = RV_Coord.y_ - RV.VehicleLength / 2;

    RV_EndPoint[3].x_ = RV_Coord.x_ + RV.VehicleWidth / 2;
    RV_EndPoint[3].y_ = RV_Coord.y_ - RV.VehicleLength / 2;

    if (RV_Coord.x_ > 0)
    {
        //自车右侧盲区区域
        HV_BlindSpot[0].x_ = HV.VehicleWidth / 2;
        HV_BlindSpot[0].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        HV_BlindSpot[1].x_ = g_V2V_Parameter.LCW_Blind_Spot_Width
                             + HV.VehicleWidth / 2;
        HV_BlindSpot[1].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        HV_BlindSpot[2].x_ = g_V2V_Parameter.LCW_Blind_Spot_Width
                             + HV.VehicleWidth / 2;
        ;
        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             * tan(FG_Angletoradian(g_V2V_Parameter.LCW_Blind_Spot_Angle))
                             + g_V2V_Parameter.LCW_Blind_Spot_ToMirror;
    }
    else
    {
        //自车左侧盲区区域
        HV_BlindSpot[0].x_ = -HV.VehicleWidth / 2;
        HV_BlindSpot[0].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        HV_BlindSpot[1].x_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             - HV.VehicleWidth / 2;
        HV_BlindSpot[1].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        HV_BlindSpot[2].x_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             - HV.VehicleWidth / 2;

        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             * tan(FG_Angletoradian(g_V2V_Parameter.LCW_Blind_Spot_Angle))
                             + g_V2V_Parameter.LCW_Blind_Spot_ToMirror;
    }

#if PRINFT_OPT
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[0] X = %f, Y = %f\n", RV_EndPoint[0].x_, RV_EndPoint[0].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[1] X = %f, Y = %f\n", RV_EndPoint[1].x_, RV_EndPoint[1].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[2] X = %f, Y = %f\n", RV_EndPoint[2].x_, RV_EndPoint[2].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[3] X = %f, Y = %f\n", RV_EndPoint[3].x_, RV_EndPoint[3].y_);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[0] X = %f, Y = %f\n", HV_BlindSpot[0].x_, HV_BlindSpot[0].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[1] X = %f, Y = %f\n", HV_BlindSpot[1].x_, HV_BlindSpot[1].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[2] X = %f, Y = %f\n", HV_BlindSpot[2].x_, HV_BlindSpot[2].y_);
#endif
    //邻车四个顶点是否都在盲区内
//	for (i = 3; i >= 0; i--) {
//		result = IsInTriangle(HV_BlindSpot[0], HV_BlindSpot[1], HV_BlindSpot[2],
//				RV_EndPoint[i]);
//
//		if (result == 0)
//			continue;
//		else
//			break;
//	}

//邻车二个顶点是否都在盲区内
    for (i = 0; i < 2; i++)
    {
        result = IsInTriangle(HV_BlindSpot[0], HV_BlindSpot[1], HV_BlindSpot[2],
                              RV_EndPoint[i]);

        if (result == 0)
            continue;
        else
            break;
    }

    if (i == 2)
    {

        double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                    RV.Longitude);
        double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude,
                                          RV.Latitude, RV.Longitude, HV.Heading);

        pResult->wStr = BSW_Warning;
        pResult->wPrio = LOW_PRIO;
        pResult->wLvl = LOW_LVL;

        if ((RV_Coord.x_ > 0
                && ((HV.Light & rightTurnSignalOn) == rightTurnSignalOn))
                || (RV_Coord.x_ < 0
                    && ((HV.Light & leftTurnSignalOn) == leftTurnSignalOn)))
        {

            pResult->wStr = LCW_Warning;

            //盲区变道时碰撞风险识别
            double CPA[3];
            FG_GetClosePoint(HV.Latitude, HV.Longitude, HV.Speed, HV.Heading,
                             RV.Latitude, RV.Longitude, RV.Speed, RV.Heading, CPA);

            pResult->wPrio = MEDIUM_PRIO;
            pResult->wLvl = FG_GetWarnLvl(CPA[1], V2V_ICW_TTC);
            pResult->TTC = CPA[1];

            if (pResult->TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
            {
                pResult->wPrio = HIGH_PRIO;
            }
        }

        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;

    }
    else
    {

    }

}

void V2V_LCW_Section_Rectangle(tVehData HV, tVehData RV, Point RV_Coord,
                     tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//***********************Get In LCW_Section********************************//\n");
#endif

    Point HV_BlindSpot[4], RV_EndPoint[4];
    int i, result;
//	tResult_Bsm CResult = { -1, Type_DEFAULT, STR_DEFAULT, PRIO_DEFAULT, LVL_DEFAULT, -1, -1, -1 };

//邻车顶点坐标
    RV_EndPoint[0].x_ = RV_Coord.x_ + RV.VehicleWidth / 2;
    RV_EndPoint[0].y_ = RV_Coord.y_ + RV.VehicleLength / 2;

    RV_EndPoint[1].x_ = RV_Coord.x_ - RV.VehicleWidth / 2;
    RV_EndPoint[1].y_ = RV_Coord.y_ + RV.VehicleLength / 2;

    RV_EndPoint[2].x_ = RV_Coord.x_ - RV.VehicleWidth / 2;
    RV_EndPoint[2].y_ = RV_Coord.y_ - RV.VehicleLength / 2;

    RV_EndPoint[3].x_ = RV_Coord.x_ + RV.VehicleWidth / 2;
    RV_EndPoint[3].y_ = RV_Coord.y_ - RV.VehicleLength / 2;

    if (RV_Coord.x_ > 0)
    {
        //自车右侧盲区区域
    	//左上角
        HV_BlindSpot[0].x_ = HV.VehicleWidth / 2;
        HV_BlindSpot[0].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        //右上角
        HV_BlindSpot[1].x_ = g_V2V_Parameter.LCW_Blind_Spot_Width
                             + HV.VehicleWidth / 2;
        HV_BlindSpot[1].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        //左下角
        HV_BlindSpot[2].x_ = HV.VehicleWidth / 2;
        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Angle;

        //右下角
        HV_BlindSpot[3].x_ = HV.VehicleWidth / 2 + g_V2V_Parameter.LCW_Blind_Spot_Width;
        HV_BlindSpot[3].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Angle;

  /*
        HV_BlindSpot[2].x_ = g_V2V_Parameter.LCW_Blind_Spot_Width
                             + HV.VehicleWidth / 2;
        ;
        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             * tan(FG_Angletoradian(g_V2V_Parameter.LCW_Blind_Spot_Angle))
                             + g_V2V_Parameter.LCW_Blind_Spot_ToMirror;
  */
    }
    else
    {
        //自车左侧盲区区域

    	//右上角
        HV_BlindSpot[0].x_ = -HV.VehicleWidth / 2;
        HV_BlindSpot[0].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        //左上角
        HV_BlindSpot[1].x_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             - HV.VehicleWidth / 2;
        HV_BlindSpot[1].y_ = g_V2V_Parameter.LCW_Blind_Spot_ToMirror;

        //右下角
        HV_BlindSpot[2].x_ = -HV.VehicleWidth / 2;
        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Angle;

        //左下角
        HV_BlindSpot[3].x_ = -HV.VehicleWidth / 2 - g_V2V_Parameter.LCW_Blind_Spot_Width;
        HV_BlindSpot[3].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Angle;


 /*
        HV_BlindSpot[2].x_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             - HV.VehicleWidth / 2;

        HV_BlindSpot[2].y_ = -g_V2V_Parameter.LCW_Blind_Spot_Width
                             * tan(FG_Angletoradian(g_V2V_Parameter.LCW_Blind_Spot_Angle))
                             + g_V2V_Parameter.LCW_Blind_Spot_ToMirror;
   */
    }

#if PRINFT_OPT
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[0] X = %f, Y = %f\n", RV_EndPoint[0].x_, RV_EndPoint[0].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[1] X = %f, Y = %f\n", RV_EndPoint[1].x_, RV_EndPoint[1].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[2] X = %f, Y = %f\n", RV_EndPoint[2].x_, RV_EndPoint[2].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~RV_EndPoint[3] X = %f, Y = %f\n", RV_EndPoint[3].x_, RV_EndPoint[3].y_);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[0] X = %f, Y = %f\n", HV_BlindSpot[0].x_, HV_BlindSpot[0].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[1] X = %f, Y = %f\n", HV_BlindSpot[1].x_, HV_BlindSpot[1].y_);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~HV_BlindSpot[2] X = %f, Y = %f\n", HV_BlindSpot[2].x_, HV_BlindSpot[2].y_);
#endif
    //邻车四个顶点是否都在盲区内
//	for (i = 3; i >= 0; i--) {
//		result = IsInTriangle(HV_BlindSpot[0], HV_BlindSpot[1], HV_BlindSpot[2],
//				RV_EndPoint[i]);
//
//		if (result == 0)
//			continue;
//		else
//			break;
//	}

//邻车二个顶点是否都在盲区内
    for (i = 0; i < 2; i++)
    {
        result = IsInTriangle(HV_BlindSpot[0], HV_BlindSpot[1], HV_BlindSpot[2],
                              RV_EndPoint[i]);

        if (result == 0)
            continue;
        else
            break;
    }

    if (i == 2)
    {

        double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                    RV.Longitude);
        double RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude,
                                          RV.Latitude, RV.Longitude, HV.Heading);

        pResult->wStr = BSW_Warning;
        pResult->wPrio = LOW_PRIO;
        pResult->wLvl = LOW_LVL;

        if ((RV_Coord.x_ > 0
                && ((HV.Light & rightTurnSignalOn) == rightTurnSignalOn))
                || (RV_Coord.x_ < 0
                    && ((HV.Light & leftTurnSignalOn) == leftTurnSignalOn)))
        {

            pResult->wStr = LCW_Warning;

            //盲区变道时碰撞风险识别
            double CPA[3];
            FG_GetClosePoint(HV.Latitude, HV.Longitude, HV.Speed, HV.Heading,
                             RV.Latitude, RV.Longitude, RV.Speed, RV.Heading, CPA);

            pResult->wPrio = MEDIUM_PRIO;
            pResult->wLvl = FG_GetWarnLvl(CPA[1], V2V_ICW_TTC);
            pResult->TTC = CPA[1];

            if (pResult->TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
            {
                pResult->wPrio = HIGH_PRIO;
            }
        }

        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;

    }
    else
    {

    }

}


void V2V_AVW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

#if PRINFT_OPT
    printf("//\n***********************Get In AVW_Section********************************//\n");
#endif
    printf("uint16_t VehicleClass = %d\n", RV.VehicleClass);
    double Dis, RelBearing;

    Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude, RV.Longitude);
    RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                               RV.Longitude, HV.Heading);


        if (((RV.Light & hazardSignalOn) == hazardSignalOn))
//        if (RV.VehicleClass == 16 && Dis < 150)
        {
        	pResult->wStr = AVW_Warning;
        }
        else
            pResult->wStr = STR_DEFAULT;


    pResult->wPrio = LOW_PRIO;
    pResult->wLvl = LOW_LVL;

    pResult->Distance = Dis;
    pResult->RelBearing = RelBearing;

}

void V2V_EVW_Section(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{

    double Dis, RelBearing;

    if (RV.VehicleClass == TYpeOther || RV.VehicleClass == Fire_Light
            || RV.VehicleClass == Fire_Heavy
            || RV.VehicleClass == Fire_Paramedic
            || RV.VehicleClass == Fire_Ambulance
            || RV.VehicleClass == Police_Light
            || RV.VehicleClass == Police_Heavy
            || RV.VehicleClass == Other_Responder
            || RV.VehicleClass == Other_Ambulance)
    {

#if PRINFT_OPT
        printf("//***********************Get In EVW_Section********************************//\n");
#endif
        Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                             RV.Longitude);
        RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                                   RV.Longitude, HV.Heading);

        pResult->wStr = EVW_Warning;
        pResult->wPrio = LOW_PRIO;
        pResult->wLvl = LOW_LVL;

        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;
    }
}

void V2V_CLW_Warning(tVehData HV, tVehData RV, tResult_Bsm *pResult)
{
    double Dis, RelBearing;

    if (RV.AuxBreakSystem == 2)
    {

        Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                             RV.Longitude);
        RelBearing = FG_GetRelBear(HV.Latitude, HV.Longitude, RV.Latitude,
                                   RV.Longitude, HV.Heading);

        pResult->wStr = CLW_Warning;
        pResult->wPrio = LOW_PRIO;
        pResult->wLvl = LOW_LVL;

        pResult->Distance = Dis;
        pResult->RelBearing = RelBearing;
    }
}

/*******************************************************
 * 函数名 :  double PointScalar(Axis axis, double point[1][2])
 * 功能描述: 一点在投影轴上的标量
 * 输入参数: axis	投影轴
 point	点坐标
 * 输出参数: *
 * 返回值 : double
 * 作者 : 黄翔宇
 * 创建日期: 2017/4/6 11:26:50
 * 修改记录 :
 ********************************************************/
double PointScalar(Axis axis, double point[1][2])
{
    double x0 = (point[0][0] * axis.x + point[0][1] * axis.y)
                / (axis.x * axis.x + axis.y * axis.y) * axis.x;
    double y0 = (point[0][0] * axis.x + point[0][1] * axis.y)
                / (axis.x * axis.x + axis.y * axis.y) * axis.y;

    return x0 * axis.x + y0 * axis.y;
}
/*******************************************************
 * 函数名 :  void ScalarCompare(Axis axis, rectangle Rec, double *max, double *min)
 * 功能描述: 计算矩形四个顶点在投影轴上的标量，输出最大值和最小值
 * 输入参数: axis   投影轴向量结构体
 Rec	矩形顶点坐标结构体
 * 输出参数: max	标量最大值
 min	标量最小值
 * 返回值 : void
 * 作者 : 黄翔宇
 * 创建日期: 2017/4/6 11:24:10
 * 修改记录 :
 ********************************************************/
void ScalarCompare(Axis axis, rectangle Rec, double *max, double *min)
{
    double PS[4] = { 0 };

    int i = 0;
    PS[0] = PointScalar(axis, Rec.LL);
    PS[1] = PointScalar(axis, Rec.LR);
    PS[2] = PointScalar(axis, Rec.UL);
    PS[3] = PointScalar(axis, Rec.UR);

    *max = PS[0];
    *min = PS[0];

    for (i = 1; i < 4; i++)
    {
        if (PS[i] > *max)
            *max = PS[i];
        if (PS[i] < *min)
            *min = PS[i];
    }
}

/*******************************************************
 * 函数名 :  int IntersctionWarn(double p_dDistance, double DCPA, double p_dRelBearing, double p_dRelCourse)
 * 功能描述: 判断交汇最近点时两车是否碰撞
 * 输入参数: p_dDistance			当前车距
 DCPA					最近点时车距
 p_dRelBearing			相对方位角
 p_dRelCourse			相对方向角
 * 输出参数: *
 * 返回值 :  int	0    最近点时两车没有碰撞
 1    最近点时两车会碰撞
 * 作者 : 黄翔宇
 * 创建日期: 2017/4/6 11:20:15
 * 修改记录 :
 ********************************************************/
int IntersctionWarn(double p_dDistance, double DCPA, double p_dRelBearing,
                    double p_dRelCourse, tVehData HV, tVehData RV)
{
    double CenterB_X, CenterB_Y;
    rectangle REC_A, REC_B;
    Axis axis1, axis2, axis3, axis4;

    //以自车中心为原点，自车四个顶点坐标
    REC_A.UL[0][0] = -(HV.VehicleWidth
                       + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.UL[0][1] = (HV.VehicleLength
                      + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.UR[0][0] =
        (HV.VehicleWidth + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.UR[0][1] = (HV.VehicleLength
                      + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.LL[0][0] = -(HV.VehicleWidth
                       + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.LL[0][1] = -(HV.VehicleLength
                       + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.LR[0][0] =
        (HV.VehicleWidth + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;
    REC_A.LR[0][1] = -(HV.VehicleLength
                       + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)) / 2;

    GetCPA(p_dDistance, DCPA, p_dRelBearing, p_dRelCourse, &CenterB_X,
           &CenterB_Y);

    //计算邻车四个点坐标

    double angle1 =
        FG_RadiantoAngle(
            atan2(
                (HV.VehicleWidth
                 + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)),
                (HV.VehicleLength
                 + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh))));
    double angle2 = FG_Angletoradian(90 - (angle1 + p_dRelCourse - 180));
    double angle3 = FG_Angletoradian(90 - (p_dRelCourse - 180 - angle1));
    double temp1 = pow(
                       (RV.VehicleLength + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh))
                       * (RV.VehicleLength
                          + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh))
                       + (RV.VehicleWidth
                          + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh))
                       * (RV.VehicleWidth
                          + (2 * g_V2V_Parameter.ICW_TTC_SafeDisVeh)),
                       0.5) / 2;

    REC_B.UR[0][0] = CenterB_X - temp1 * cos(angle2);
    REC_B.UR[0][1] = CenterB_Y - temp1 * sin(angle2);

    REC_B.LL[0][0] = 2 * CenterB_X - REC_B.UR[0][0];
    REC_B.LL[0][1] = 2 * CenterB_Y - REC_B.UR[0][1];

    REC_B.LR[0][0] = CenterB_X + temp1 * cos(angle3);
    REC_B.LR[0][1] = CenterB_Y + temp1 * sin(angle3);

    REC_B.UL[0][0] = 2 * CenterB_X - REC_B.LR[0][0];
    REC_B.UL[0][1] = 2 * CenterB_Y - REC_B.LR[0][1];

    //计算四个投影轴
    axis1.x = REC_A.UL[0][0] - REC_A.LL[0][0];
    axis1.y = REC_A.UL[0][1] - REC_A.LL[0][1];

    axis2.x = REC_A.UL[0][0] - REC_A.UR[0][0];
    axis2.y = REC_A.UL[0][1] - REC_A.UR[0][1];

    axis3.x = REC_B.LR[0][0] - REC_B.UR[0][0];
    axis3.y = REC_B.LR[0][1] - REC_B.UR[0][1];

    axis4.x = REC_B.LR[0][0] - REC_B.LL[0][0];
    axis4.y = REC_B.LR[0][1] - REC_B.LL[0][1];

    double max_A, min_A;
    double max_B, min_B;

    ScalarCompare(axis1, REC_A, &max_A, &min_A);
    ScalarCompare(axis1, REC_B, &max_B, &min_B);

    if (max_A < min_B || min_A > max_B)
    {
        return 0;
    }
    else
    {
        ScalarCompare(axis2, REC_A, &max_A, &min_A);
        ScalarCompare(axis2, REC_B, &max_B, &min_B);
        if (max_A < min_B || min_A > max_B)
        {
            return 0;
        }
        else
        {
            ScalarCompare(axis3, REC_A, &max_A, &min_A);
            ScalarCompare(axis3, REC_B, &max_B, &min_B);
            if (max_A < min_B || min_A > max_B)
            {
                return 0;
            }
            else
            {
                ScalarCompare(axis4, REC_A, &max_A, &min_A);
                ScalarCompare(axis4, REC_B, &max_B, &min_B);
                if (max_A < min_B || min_A > max_B)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

/*******************************************************
 * 函数名 :  void GetCPA(double D, double R, double Angle, double Course, double *X, double *Y)
 * 功能描述: 获取交汇两车交汇最近点坐标
 * 输入参数: double D   车距
 double R	最近点距离DCPA
 double Angle  相对方位
 double Course 相对行驶方向
 * 输出参数:    double &X, double &Y
 * 返回值 : void
 * 作者 : 黄翔宇
 * 创建日期: 2017/4/9 21:40:04
 * 修改记录 :
 ********************************************************/
void GetCPA(double D, double R, double Angle, double Course, double *X,
            double *Y)
{
    double Point[1][2] = { { 0.0, 0.0 } };

    //点的坐标

    Point[0][0] = D * sin(FG_Angletoradian(Angle));
    Point[0][1] = D * cos(FG_Angletoradian(Angle));

    double t = R / D;
    double Fx = Point[0][0] * t;
    double Fy = Point[0][1] * t;

    double a = acos(t);

    double x1 = Fx * cos(a) - Fy * sin(a);
    double y1 = Fx * sin(a) + Fy * cos(a);
    double x2 = Fx * cos(-a) - Fy * sin(-a);
    double y2 = Fx * sin(-a) + Fy * cos(-a);

    double K1 = (Point[0][1] - y1) / (Point[0][0] - x1);
    double K2 = (Point[0][1] - y2) / (Point[0][0] - x2);

    double temp1 = FG_RadiantoAngle(atan2(K1, 1));
    if (temp1 < 0)
        temp1 = temp1 + 360;
    double temp2 = FG_RadiantoAngle(atan2(K2, 1));
    if (temp2 < 0)
        temp2 = temp2 + 360;

    if (fabs(Course - temp1) < fabs(Course - temp2))
    {
        *X = x1;
        *Y = y1;
    }
    else
    {
        *X = x2;
        *Y = y2;
    }
}

/*
 * 根据HV当前车速时间和车速，判断RV是否在危险范围内
 * 返回：0 ---进行危险识别
 * 		1 ---安全，不做处理
 */
int IsDangerPossible(tVehData HV, tVehData RV)
{

    double Dis = FG_Getdistance(HV.Latitude, HV.Longitude, RV.Latitude,
                                RV.Longitude);

    double Time = Dis / HV.Speed;

    if (HV.Speed > g_V2V_Parameter.isDanger_hv_speed_low
            && Time < g_V2V_Parameter.isDanger_hv_warning_time)
        return 0;
    else
        return 1;

}

/*
 * 基于RV相对于HV的位置进行危险类型判断：前方---异常车辆、
 * 								  正前方---前向碰撞、
 * 								  后方---紧急车辆、
 * 								  侧后方---盲区提示
 */
int CaseDetect(tVehData HV, tVehData RV, Point RV_Coord)
{

    double x = RV_Coord.x_;
    double y = RV_Coord.y_;

    double Safe_LateralOffset = (HV.VehicleWidth + RV.VehicleWidth) / 2
                                + g_V2V_Parameter.VehLOC_SafeDis_Value;

    if (y >= 0)
    {
        return (x < -Safe_LateralOffset || x > Safe_LateralOffset) ?
               FRONT_LOC : FRONT_LOC_DIRECT;
    }
    else
    {
        return (x < -Safe_LateralOffset || x > Safe_LateralOffset) ?
               BACK_LOC_SIDE : BACK_LOC;
    }

}

/*
 * 判断RV相对于HV的位置：正前方、左前方、右前方、正后方、左边后方、右后方
 */
int CalVehicleLocation(tVehData HV, tVehData RV, Point RV_Coord)
{

    double x = RV_Coord.x_;
    double y = RV_Coord.y_;

    double Safe_LateralOffset = (HV.VehicleWidth + RV.VehicleWidth) / 2
                                + g_V2V_Parameter.VehLOC_SafeDis_Value;

#if PRINFT_OPT
    printf("Safe_LateralOffset = %f\n", Safe_LateralOffset);
#endif

    if (y >= 0)
    {
        if (x >= Safe_LateralOffset)
            return FRONT_LOC_Right;
        else if (x > -Safe_LateralOffset && x < Safe_LateralOffset)
            return FRONT_LOC_DIRECT;
        else
            return FRONT_LOC_Left;
    }
    else
    {
        if (x >= Safe_LateralOffset)
            return BACK_LOC_Right;
        else if (x > -Safe_LateralOffset && x < Safe_LateralOffset)
            return BACK_LOC;
        else
            return BACK_LOC_Left;
    }

}

double FG_GetTHW(double p_dVObj, double p_dDistance, double HV_Length,
                 double RV_Length)
{
    double THW = (p_dDistance - (HV_Length + RV_Length) / 2) / p_dVObj;
    if (THW < 0)
        THW = g_V2V_Parameter.FCW_THW_threshold_value;
    return THW;
}

double FG_GetTTC(double p_dVobj, double p_dVtag, double p_dDistance,
                 double HV_Length, double RV_Length)
{
    double TTC = (p_dDistance - (HV_Length + RV_Length) / 2)
                 / (p_dVobj - p_dVtag);
    if (TTC < 0)
        TTC = g_V2V_Parameter.FCW_TTC_threshold_value;
    return TTC;
}

/**********************************************
 * 函数名 :   double* FG_GetClosePoint(double p_dLatObj, double p_dlngObj, double p_dVObj, double p_dCObj, double p_dLatTag, double p_dlngTag,double p_dVTag, double p_dCTag)
 * 功能描述:  计算两车到达最近点的时间和距离
 * 输入参数:  p_dLatObj  自车纬度坐标
 p_dlngObj  自车经度坐标
 p_dLatTag  目标车纬度坐标
 p_dlngTag  目标车经度坐标
 p_dVObj	 自车车速
 p_dVTag	 目标车车速
 p_dCObj    自车航向角
 p_dCTag	 目标车航向角
 * 输出参数:  *
 * 返回值 :	  *CPA   两车到达最近点的时间*CPA[1]和距离*CPA[0],相对行驶方向*CPA[2]
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 11:18:03
 * 修改记录： 170222  最近距离减去车身长度修正
 **********************************************/
int FG_GetClosePoint(double p_dLatObj, double p_dlngObj, double p_dVObj,
                     double p_dCObj, double p_dLatTag, double p_dlngTag, double p_dVTag,
                     double p_dCTag, double CPA[])
{
    double DCPA, TCPA;
    double Temp = 0, Temp1 = 0, Temp2 = 0;
    double p_dRelativeCourese = 0;
    double p_dRC = 0;

    double p_dDistance = FG_Getdistance(p_dLatObj, p_dlngObj, p_dLatTag,
                                        p_dlngTag);
    double p_dTrueBearing = FG_GetTrueBear(p_dLatObj, p_dlngObj, p_dLatTag,
                                           p_dlngTag);
    double p_dRelativeBearing = p_dTrueBearing - p_dCObj;
    if (p_dRelativeBearing < 0)
        p_dRelativeBearing += 360;
    Temp2 = FG_Angletoradian(p_dTrueBearing);

    double dCourseObj = FG_Angletoradian(p_dCObj);
    double dCourseTag = FG_Angletoradian(p_dCTag);

    double dVobj_X = p_dVObj * sin(dCourseObj);
    double dVobj_Y = p_dVObj * cos(dCourseObj);
    double dVTag_X = p_dVTag * sin(dCourseTag);
    double dVTag_Y = p_dVTag * cos(dCourseTag);
    double dDeltaV_X = dVTag_X - dVobj_X;
    double dDeltaV_Y = dVTag_Y - dVobj_Y;
    double dRleativeV = pow(dDeltaV_X * dDeltaV_X + dDeltaV_Y * dDeltaV_Y, 0.5);

    if (dDeltaV_Y == 0)
    {
        if (dDeltaV_X > 0)
            p_dRelativeCourese = 90;
        else if (dDeltaV_X < 0)
            p_dRelativeCourese = 270;
        else
            p_dRelativeCourese = 0;
    }
    else
    {
        if (dDeltaV_Y < 0)
            Temp = PI;
        if (dDeltaV_Y > 0)
        {
            if (dDeltaV_X >= 0)
                Temp = 0;
            else
                Temp = 2 * PI;
        }
        Temp1 = atan(dDeltaV_X / dDeltaV_Y) + Temp;
        p_dRelativeCourese = FG_RadiantoAngle(Temp1);
    }

    if (p_dRelativeCourese >= p_dCObj)
    {
        p_dRC = p_dRelativeCourese - p_dCObj;
    }
    else
    {
        p_dRC = -p_dCObj + p_dRelativeCourese + 360;
    }

    if (((p_dRelativeBearing >= 0 && p_dRelativeBearing <= 90)
            && (p_dRC >= 180 && p_dRC <= 360))
            || ((p_dRelativeBearing >= 90 && p_dRelativeBearing <= 180)
                && (p_dRC >= 270 && p_dRC <= 360))
            || ((p_dRelativeBearing >= 180 && p_dRelativeBearing <= 270)
                && (p_dRC >= 0 && p_dRC <= 90))
            || ((p_dRelativeBearing >= 270 && p_dRelativeBearing <= 360)
                && (p_dRC >= 0 && p_dRC <= 180)))
    {

        DCPA = p_dDistance * fabs(sin(Temp1 - Temp2 - PI));

        TCPA = p_dDistance * fabs(cos(Temp1 - Temp2 - PI)) / dRleativeV;
        if (dRleativeV == 0)
            TCPA = 1000;
    }
    else
    {
        DCPA = 10000;
        TCPA = 10000;
    }

    CPA[0] = DCPA;
    CPA[1] = TCPA;
    CPA[2] = p_dRC;
    return 0;
}

WARN_LVL FG_GetWarnLvl(double TTC, char Type)
{

    switch (Type)
    {
    case V2V_FCW_TTC:
        if ((TTC < g_V2V_Parameter.FCW_TTC_threshold_value)
                && (TTC >= g_V2V_Parameter.FCW_TTC_threshold_value * 2 / 3))
            return LOW_LVL;
        else if ((TTC < g_V2V_Parameter.FCW_TTC_threshold_value * 2 / 3)
                 && (TTC >= g_V2V_Parameter.FCW_TTC_threshold_value * 1 / 3))
            return MEDIUM_LVL;
        else if (TTC < g_V2V_Parameter.FCW_TTC_threshold_value * 1 / 3)
            return HIGH_LVL;
        else
            return LVL_DEFAULT;
        break;
    case V2V_FCW_THW:
        if ((TTC < g_V2V_Parameter.FCW_THW_threshold_value)
                && (TTC >= g_V2V_Parameter.FCW_THW_threshold_value * 2 / 3))
            return LOW_LVL;
        else if ((TTC < g_V2V_Parameter.FCW_THW_threshold_value * 2 / 3)
                 && (TTC >= g_V2V_Parameter.FCW_THW_threshold_value * 1 / 3))
            return MEDIUM_LVL;
        else if (TTC < g_V2V_Parameter.FCW_THW_threshold_value * 1 / 3)
            return HIGH_LVL;
        else
            return LVL_DEFAULT;
        break;

    case V2V_ICW_TTC:
        if ((TTC < g_V2V_Parameter.ICW_TTC_threshold_value)
                && (TTC >= g_V2V_Parameter.ICW_TTC_threshold_value * 2 / 3))
            return LOW_LVL;
        else if ((TTC < g_V2V_Parameter.ICW_TTC_threshold_value * 2 / 3)
                 && (TTC >= g_V2V_Parameter.ICW_TTC_threshold_value * 1 / 3))
            return MEDIUM_LVL;
        else if (TTC < g_V2V_Parameter.ICW_TTC_threshold_value * 1 / 3)
            return HIGH_LVL;
        else
            return LVL_DEFAULT;
        break;
    case V2V_MAY_DANGER:
        return MEDIUM_LVL;
        break;
    default:
        break;
    }
    return LVL_DEFAULT;
}

double FG_GetRelCourse(tVehData HV, tVehData RV)
{

    double p_dRelativeCourese;

    if (HV.Heading > 180)
        HV.Heading = HV.Heading - 360;

    if (RV.Heading > 180)
        RV.Heading = RV.Heading - 360;

    p_dRelativeCourese = fabs(HV.Heading - RV.Heading);

    return p_dRelativeCourese;
}
