#ifndef __APPLYLAYER_CAN_CAN_H_
#define __APPLYLAYER_CAN_CAN_H_

#include "../../Include/BaseInterface_Include.h"
#include "CAN.h"

/****************************************************
 * 函数名称:InitCAN()
 * 功能描述:初始化CAN接口
 * 输入参数:无
 * 输出参数:无
 * 返 回 值:CAN接口句柄
 * 创建日期:
 ****************************************************/
int InitCAN();

/****************************************************
 * 函数名称:int CloseCAN()
 * 功能描述:关闭CAN接口
 * 输入参数:无
 * 输出参数:无
 * 返 回 值:关闭CAN接口状态
 * 创建日期:
 ****************************************************/
int CloseCAN();

#endif //#ifndef __APPLYLAYER_CAN_H_
