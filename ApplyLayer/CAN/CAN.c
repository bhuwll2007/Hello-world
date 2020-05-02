#include "CAN.h"

/****************************************************
 * 函数名称:void OutData(CANData fix)
 * 功能描述:打印CAN数据
 * 输入参数:CANData fix CAN数据结构体
 * 输出参数:无
 * 返 回 值:无
 * 创建日期:
 ****************************************************/
//void OutData(CANData fix)
//{
//    printf("transmission = %ld\n", fix.transmission);
//    printf("speed = %ld\n", fix.speed);
//    printf("heading = %ld\n", fix.heading);
//    printf("angle = %ld\n", fix.angle);
//    printf("vehicleClass = %ld\n", fix.vehicleClass);
//}

/****************************************************
 * 函数名称:InitCAN()
 * 功能描述:初始化CAN接口
 * 输入参数:无
 * 输出参数:无
 * 返 回 值:CAN接口句柄
 * 创建日期:
 ****************************************************/
int InitCAN()
{
    int sock = CAN_Open("can0");
    if(sock == 1);
    return 1;
}

/****************************************************
 * 函数名称:int CloseCAN()
 * 功能描述:关闭CAN接口
 * 输入参数:无
 * 输出参数:无
 * 返 回 值:关闭CAN接口状态
 * 创建日期:
 ****************************************************/
int CloseCAN()
{
    int nRet = CAN_Close();
    return nRet;
}

